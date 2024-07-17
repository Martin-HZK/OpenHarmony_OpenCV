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

//#include <aki/jsbind.h>
#include "common.h"

using namespace std;
using namespace cv;
static const char *TAG = "[opencv_qrcodeIdentify]";

//JSBIND_GLOBAL() 
//{ 
//    JSBIND_FUNCTION(SayHello);
//    //JSBIND_FUNCTION(img2Gray);
//}
//
//JSBIND_ADDON(entry) // 注册 AKI 插件名为: entry

struct ParametersInfo {
    Mat mat;
    bool detectOnly = false;
    bool multiQR = false;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfoQrcode {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    ParametersInfo params;
    CallbackPromiseInfo info;
    bool detected = false;
    vector<string> decodeInfos;
};

bool WrapJsQRCodeInfo(napi_env env, bool detected, std::vector<std::string>& dectedInfos, napi_value& result)
{
    napi_value value = nullptr;
    // readonly detected?: boolean
    napi_get_boolean(env, detected, &value);
    napi_set_named_property(env, result, "detected", value);

    napi_value decodeArray = nullptr;
    napi_status result_status = napi_create_array(env, &decodeArray); // 创建一个napi数组
    if (result_status != napi_ok) {
        napi_get_undefined(env, &result);
        return false;
    }

    for (int i = 0; i < dectedInfos.size(); i++) {
        napi_value decodeStr = nullptr;
        napi_create_string_utf8(env, dectedInfos[i].c_str(), NAPI_AUTO_LENGTH, &decodeStr);
        napi_set_element(env, decodeArray, i, decodeStr); // 添加一个napi元素到napi数组
    }
    napi_set_named_property(env, result, "decodes", decodeArray);

    return true;
}


void runQRCode(QRCodeDetector &qrcode, const Mat &input, AsyncCallbackInfoQrcode &callbackInfo,
               vector<Point> &corners)
{
    bool modeMultiQR = callbackInfo.params.multiQR;
    bool detectOnly = callbackInfo.params.detectOnly;
    callbackInfo.decodeInfos.clear();
    if (!modeMultiQR) {
        if (!detectOnly) {
            string decode_info1 = qrcode.detectAndDecode(input, corners);
            if (!decode_info1.empty()) {
                callbackInfo.detected = true;
                callbackInfo.decodeInfos.push_back(decode_info1);
                OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "qrcode info：%{public}s", decode_info1.c_str());
            }
        } else {
            bool detection_result = qrcode.detect(input, corners);
            callbackInfo.detected = detection_result;
            OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "qrcode detect：%{public}d", detection_result);
        }
    } else {
        if (!detectOnly) {
            bool result_detection = qrcode.detectAndDecodeMulti(input, callbackInfo.decodeInfos, corners);
            callbackInfo.detected = result_detection;
            OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "qrcode detect：%{public}d", result_detection);
            for (auto &info : callbackInfo.decodeInfos) {
                OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "qrcode info：%{public}s", info.c_str());
            }
        }
        else {
            bool result_detection = qrcode.detectMulti(input, corners);
            callbackInfo.detected = result_detection;
            OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "qrcode detect：%{public}d", result_detection);
        }
    }
}

napi_value SetQrcode(const napi_env &env, bool detected, const vector<string> &decodeInfos, napi_value &result) {
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "Enter SetQrcode.");

    napi_value value = nullptr;
    // readonly detected?: boolean
    napi_get_boolean(env, detected, &value);
    napi_set_named_property(env, result, "detected", value);

    napi_value decodeArray = nullptr;
    napi_status result_status = napi_create_array(env, &decodeArray);
    if (result_status != napi_ok) {
        napi_get_undefined(env, &result);
        return NapiGetBoolean(env, false);
    }

    for (int i = 0; i < decodeInfos.size(); i++) {
        napi_value decodeStr = nullptr;
        napi_create_string_utf8(env, decodeInfos[i].c_str(), NAPI_AUTO_LENGTH, &decodeStr);
        napi_set_element(env, decodeArray, i, decodeStr); // 添加一个napi元素到napi数组
    }
    napi_set_named_property(env, result, "decodes", decodeArray);

    return NapiGetBoolean(env, true);
}

void AsyncCompleteCallbackQrcode(napi_env env, napi_status status, void *data) {
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "Enter AsyncCompleteCallbackCvtColor.");
    if (!data) {
        OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "Invalid async callback data.");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoQrcode *>(data);
    napi_value result = NapiGetNull(env);

    asynccallbackinfo->info.errorCode = ERR_OK;
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
        if (asynccallbackinfo->params.mat.data == nullptr) {
            asynccallbackinfo->info.errorCode = ERROR;
        } else {
            napi_create_object(env, &result);
            if (!SetQrcode(env, asynccallbackinfo->detected, asynccallbackinfo->decodeInfos, result)) {
                asynccallbackinfo->info.errorCode = ERROR;
                result = NapiGetNull(env);
            }
        }
    }
    ReturnCallbackPromise(env, asynccallbackinfo->info, result);
    if (asynccallbackinfo->info.callback != nullptr) {
        napi_delete_reference(env, asynccallbackinfo->info.callback);
    }
    napi_delete_async_work(env, asynccallbackinfo->asyncWork);
    FreeMemory<AsyncCallbackInfoQrcode>(asynccallbackinfo);
}

bool ParseParametersQrcode(const napi_env &env, const napi_callback_info &info, ParametersInfo &paras) {
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "Enter ParseParametersQrcode.");
    paras.detectOnly = false;
    paras.multiQR = false;

    size_t argc = 3;
    napi_value argv[3] = {nullptr};

    napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

    size_t strSize;
    char strBuf[256];
    napi_get_value_string_utf8(env, argv[1], strBuf, sizeof(strBuf), &strSize);
    std::string fileDir(strBuf, strSize);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "fileDir：%{public}s", fileDir.c_str());

    napi_get_value_string_utf8(env, argv[2], strBuf, sizeof(strBuf), &strSize);
    std::string fileName(strBuf, strSize);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "fileName：%{public}s", fileName.c_str());
    
    bool ret = GetMatFromRawFile(env, argv[0], fileDir, fileName, paras.mat);
#ifdef OHOS_OPENCV_SAMPLE_DEBUG
    bool retWrt = imwrite("/data/storage/el2/base/haps/entry/files/srcImage.jpg", paras.mat);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "imwrite：%{public}d", retWrt);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "input Mat size: %{public}d, cols：%{public}d, rows：%{public}d",
                 paras.mat.total(), paras.mat.cols, paras.mat.rows);
#endif
    return ret;
}

napi_value QRCodeIdentification(napi_env env, napi_callback_info info) {
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "QRCodeIdentification Begin");
    napi_value result = NapiGetNull(env);
    //Mat srcImage;
    ParametersInfo params;
    if (!ParseParametersQrcode(env, info, params)) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "ParseParametersQrcode failed!.");
        return result;
    }

    AsyncCallbackInfoQrcode *asynccallbackinfo =
        new (std::nothrow) AsyncCallbackInfoQrcode{.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return JSParaError(env, params.callback);
    }
    napi_value promise = nullptr;
    PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback, asynccallbackinfo->info, promise);
    
    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "qrcode", NAPI_AUTO_LENGTH, &resourceName);
    // Asynchronous function call
    napi_create_async_work(
        env, nullptr, resourceName,
        [](napi_env env, void *data) {
            OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "lambda function of napi_create_async_work start.");
            AsyncCallbackInfoQrcode *asynccallbackinfo = static_cast<AsyncCallbackInfoQrcode *>(data);
            if (asynccallbackinfo) {
                QRCodeDetector qrcode;
                vector<Point> corners;
                runQRCode(qrcode, asynccallbackinfo->params.mat, *asynccallbackinfo, corners);
            }
        },
        AsyncCompleteCallbackQrcode, (void *)asynccallbackinfo, &(asynccallbackinfo->asyncWork));

    //NAPI_CALL(env, napi_queue_async_work(env, asynccallbackinfo->asyncWork));
    if (napi_queue_async_work(env, asynccallbackinfo->asyncWork) != 0) {
        return NapiGetNull(env);
    }
    if (asynccallbackinfo->info.isCallback) {
        return NapiGetNull(env);
    } else {
        return promise;
    }
}

