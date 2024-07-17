/*
 * Copyright (c) 2024 Shenzhen Kaihong Digital Industry Development Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common.h"

using namespace cv;
static const char *TAG = "[opencv_napi]";

bool GetMatFromRawFile(napi_env env, napi_value jsResMgr, const std::string &rawfileDir, const std::string &fileName, Mat &srcImage)
{
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "GetMatFromRawFile Begin");
    NativeResourceManager *mNativeResMgr = OH_ResourceManager_InitNativeResourceManager(env, jsResMgr);
    if (mNativeResMgr == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "Init native resource manager failed!.");
        return false;
    }
    
    RawDir *rawDir = OH_ResourceManager_OpenRawDir(mNativeResMgr, rawfileDir.c_str());
    if (rawDir == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "Open raw file dir failed!.");
        OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);
        return false;
    }
    int count = OH_ResourceManager_GetRawFileCount(rawDir);
    
    bool isFileExist = false;
    for (int i = 0; i < count; i++) {
        std::string name = OH_ResourceManager_GetRawFileName(rawDir, i);
        OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "name：%{public}s", name.c_str());
        if (name == fileName) {
            isFileExist = true;
            break;
        }
    }
    if (!isFileExist) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "Raw file directory not exist file: %{public}s.", fileName.c_str());
        OH_ResourceManager_CloseRawDir(rawDir);
        OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);
        return false;
    }
    RawFile *rawFile = OH_ResourceManager_OpenRawFile(mNativeResMgr, fileName.c_str());
    if (rawFile == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "Open Raw file failed, file name: %{public}s.",
                     fileName.c_str());
        OH_ResourceManager_CloseRawDir(rawDir);
        OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);
        return false;
    }

    long rawFileSize = OH_ResourceManager_GetRawFileSize(rawFile);
    unsigned char *mediaData = new unsigned char[rawFileSize];
    long rawFileOffset = OH_ResourceManager_ReadRawFile(rawFile, mediaData, rawFileSize);
    if (rawFileOffset != rawFileSize) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG,
                     "Read rawfile size[%{public}ld] not equal to actual file size[%{public}ld]", rawFileOffset, rawFileSize);
        delete[] mediaData;
        OH_ResourceManager_CloseRawFile(rawFile);
        OH_ResourceManager_CloseRawDir(rawDir);
        OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);
        return false;
    }
    std::vector<unsigned char> fileData{mediaData, mediaData + rawFileSize};
    srcImage = imdecode(fileData, CV_LOAD_IMAGE_COLOR);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "fileData size: %{public}d, srcImage：%{public}d",
                 fileData.size(), srcImage.total());
    delete[] mediaData;
    OH_ResourceManager_CloseRawFile(rawFile);
    OH_ResourceManager_CloseRawDir(rawDir);
    OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);

    return true;
}

bool cvtMat2Pixel(InputArray _src, OutputArray &_dst, int code)
{
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG,
        "cvtMat2Pixel type %{public}d, code %{public}d", _src.getMat_().type(), code);
    if (RGBA_8888 == code) {
        switch (_src.getMat_().type()) {
            case CV_8UC1:
                cv::cvtColor(_src, _dst, COLOR_GRAY2RGBA);
                break;
            case CV_8UC3:
                cv::cvtColor(_src, _dst, COLOR_RGB2RGBA);
                break;
            case CV_8UC4:
            default:
                _src.copyTo(_dst);
        }
    } else if (RGB_565 == code) {
        switch (_src.getMat_().type()) {
            case CV_8UC1:
                cv::cvtColor(_src, _dst, COLOR_GRAY2BGR565);
                break;
            case CV_8UC3:
                cv::cvtColor(_src, _dst, COLOR_RGB2BGR565);
                break;
            case CV_8UC4:
            default:
                _src.copyTo(_dst);
        }
    } else {
        return false;
    }
    return true;
}

napi_value NapiGetNull(napi_env env)
{
    napi_value result = nullptr;
    napi_get_null(env, &result);
    return result;
}

napi_value NapiGetUndefined(napi_env env)
{
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
    return result;
}

napi_value GetCallbackErrorValue(napi_env env, int32_t errCode)
{
    napi_value result = nullptr;
    napi_value eCode = nullptr;
    NAPI_CALL(env, napi_create_int32(env, errCode, &eCode));
    NAPI_CALL(env, napi_create_object(env, &result));
    NAPI_CALL(env, napi_set_named_property(env, result, "code", eCode));
    return result;
}

napi_value NapiGetBoolean(napi_env env, const bool &isValue)
{
    napi_value result = nullptr;
    napi_get_boolean(env, isValue, &result);
    return result;
}

uint32_t GetMatDataBuffSize(const Mat &mat)
{
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "Enter GetMatDataBuffSize");
    uint32_t dataBufferSize = mat.step[0] * mat.rows;
    return dataBufferSize;
}

bool CreateArrayBuffer(napi_env env, uint8_t *src, size_t srcLen, napi_value *res)
{
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "Enter CreateArrayBuffer");

    if (src == nullptr || srcLen == 0) {
        OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "src == nullptr srcLen %{public}d", srcLen);
        return false;
    }
    void *nativePtr = nullptr;
    if (napi_create_arraybuffer(env, srcLen, &nativePtr, res) != napi_ok || nativePtr == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "napi_create_arraybuffer failed");
        return false;
    }
    memcpy(nativePtr, src, srcLen);
    return true;
}

void SetCallback(const napi_env &env, const napi_ref &callbackIn, const int32_t &errorCode, const napi_value &result)
{
    napi_value undefined = nullptr;
    napi_get_undefined(env, &undefined);

    napi_value callback = nullptr;
    napi_value resultout = nullptr;
    napi_get_reference_value(env, callbackIn, &callback);
    napi_value results[ARGS_TWO] = {nullptr};
    results[PARAM0] = GetCallbackErrorValue(env, errorCode);
    results[PARAM1] = result;

    try {
        NAPI_CALL_RETURN_VOID(env,
            napi_call_function(env, undefined, callback, ARGS_TWO, &results[PARAM0], &resultout));
    } catch (std::exception e) {
        OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "napi_call_function callback exception");
    }

    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "SetCallback end");
}

void SetPromise(const napi_env &env, const napi_deferred &deferred, const int32_t &errorCode,
    const napi_value &result)
{
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "Enter SetPromise");
    if (errorCode == ERR_OK) {
        napi_resolve_deferred(env, deferred, result);
    } else {
        napi_reject_deferred(env, deferred, GetCallbackErrorValue(env, errorCode));
    }
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "SetPromise end");
}

void ReturnCallbackPromise(const napi_env &env, const CallbackPromiseInfo &info, const napi_value &result)
{
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "Enter ReturnCallbackPromise errorCode=%d", info.errorCode);
    if (info.isCallback) {
        SetCallback(env, info.callback, info.errorCode, result);
    } else {
        SetPromise(env, info.deferred, info.errorCode, result);
    }
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "ReturnCallbackPromise end");
}

napi_value JSParaError(const napi_env &env, const napi_ref &callback) 
{
    if (callback) {
        return NapiGetNull(env);
    }
    napi_value promise = nullptr;
    napi_deferred deferred = nullptr;
    napi_create_promise(env, &deferred, &promise);
    SetPromise(env, deferred, ERROR, NapiGetNull(env));
    return promise;
}

void PaddingCallbackPromiseInfo(const napi_env &env, const napi_ref &callback, CallbackPromiseInfo &info,
    napi_value &promise)
{
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "enter PaddingCallbackPromiseInfo");
    if (callback) {
        info.callback = callback;
        info.isCallback = true;
    } else {
        napi_deferred deferred = nullptr;
        NAPI_CALL_RETURN_VOID(env, napi_create_promise(env, &deferred, &promise));
        info.deferred = deferred;
        info.isCallback = false;
    }
}

bool WrapJsPixelInfoInfo(napi_env env, cv::Mat &outMat, napi_value &result) {
    uint32_t buffSize = GetMatDataBuffSize(outMat);
    napi_value value = nullptr;
    napi_create_int32(env, buffSize, &value);
    napi_set_named_property(env, result, "buffSize", value);

    value = nullptr;
    napi_create_int32(env, outMat.cols, &value);
    napi_set_named_property(env, result, "cols", value);

    value = nullptr;
    napi_create_int32(env, outMat.rows, &value);
    napi_set_named_property(env, result, "rows", value);

    void *buffer = (void *)(outMat.data);
    napi_value array;
    if (!CreateArrayBuffer(env, static_cast<uint8_t *>(buffer), buffSize, &array)) {
        napi_get_undefined(env, &result);
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "CreateArrayBuffer failed!.");
        return false;
    } else {
        napi_set_named_property(env, result, "byteBuffer", array);
        OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "CreateArrayBuffer success!.");
        return true;
    }
}
