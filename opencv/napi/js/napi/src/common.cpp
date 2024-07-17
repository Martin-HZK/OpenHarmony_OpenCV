/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#include <fstream>
#include <cstdio>
#include <cstdarg>
#include <cinttypes>
#include <string>
#include "opencv_log_wrapper.h"
#include "opencv2/imgcodecs.hpp"
#include "securec.h"

using namespace OHOS::Opencv;

namespace OHOS {
namespace OpencvNapi {
Common::Common()
{}

Common::~Common()
{}

napi_value Common::NapiGetBoolean(napi_env env, const bool &isValue)
{
    napi_value result = nullptr;
    napi_get_boolean(env, isValue, &result);
    return result;
}

napi_value Common::NapiGetNull(napi_env env)
{
    napi_value result = nullptr;
    napi_get_null(env, &result);
    return result;
}

napi_value Common::NapiGetUndefined(napi_env env)
{
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
    return result;
}

napi_value Common::GetCallbackErrorValue(napi_env env, int32_t errCode)
{
    napi_value result = nullptr;
    napi_value eCode = nullptr;
    NAPI_CALL(env, napi_create_int32(env, errCode, &eCode));
    NAPI_CALL(env, napi_create_object(env, &result));
    NAPI_CALL(env, napi_set_named_property(env, result, "code", eCode));
    return result;
}

void Common::PaddingCallbackPromiseInfo(
    const napi_env &env, const napi_ref &callback, CallbackPromiseInfo &info, napi_value &promise)
{
    OPENCV_LOGI("enter");

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

void Common::ReturnCallbackPromise(const napi_env &env, const CallbackPromiseInfo &info, const napi_value &result)
{
    OPENCV_LOGI("Enter ReturnCallbackPromise errorCode=%{public}d", info.errorCode);

    if (info.isCallback) {
        SetCallback(env, info.callback, info.errorCode, result);
    } else {
        SetPromise(env, info.deferred, info.errorCode, result);
    }

    OPENCV_LOGI("ReturnCallbackPromise end");
}

void Common::SetCallback(
    const napi_env &env, const napi_ref &callbackIn, const int32_t &errorCode, const napi_value &result)
{
    OPENCV_LOGI("Enter SetCallback with error code");
    napi_value undefined = nullptr;
    napi_get_undefined(env, &undefined);

    napi_value callback = nullptr;
    napi_value resultout = nullptr;
    napi_get_reference_value(env, callbackIn, &callback);
    if (callback == nullptr) {
        OPENCV_LOGE("get reference of callback failed");
        return;
    }
    napi_value results[ARGS_TWO] = {nullptr};
    results[PARAM0] = GetCallbackErrorValue(env, errorCode);
    results[PARAM1] = result;

    try {
        NAPI_CALL_RETURN_VOID(env, napi_call_function(env, undefined,
                                                      callback, ARGS_TWO, &results[PARAM0], &resultout));
    }
    catch(std::exception e) {
        OPENCV_LOGE("napi_call_function callback exception");
    }

    OPENCV_LOGI("SetCallback with error code end");
}

void Common::SetCallback(
    const napi_env &env, const napi_ref &callbackIn, const napi_value &result)
{
    OPENCV_LOGI("Enter SetCallback without error code");
    napi_value undefined = nullptr;
    napi_get_undefined(env, &undefined);

    napi_value callback = nullptr;
    napi_value resultout = nullptr;
    napi_get_reference_value(env, callbackIn, &callback);
    if (callback == nullptr) {
        OPENCV_LOGE("get reference of callback failed");
        return;
    }
    NAPI_CALL_RETURN_VOID(env, napi_call_function(env, undefined, callback, ARGS_ONE, &result, &resultout));

    OPENCV_LOGI("SetCallback without error code");
}

void Common::SetPromise(
    const napi_env &env, const napi_deferred &deferred, const int32_t &errorCode, const napi_value &result)
{
    OPENCV_LOGI("Enter SetPromise");

    if (errorCode == ERR_OK) {
        napi_resolve_deferred(env, deferred, result);
    } else {
        napi_reject_deferred(env, deferred, GetCallbackErrorValue(env, errorCode));
    }

    OPENCV_LOGI("SetPromise end");
}

napi_value Common::JSParaError(const napi_env &env, const napi_ref &callback)
{
    if (callback) {
        return Common::NapiGetNull(env);
    }
    napi_value promise = nullptr;
    napi_deferred deferred = nullptr;
    napi_create_promise(env, &deferred, &promise);
    SetPromise(env, deferred, ERROR, Common::NapiGetNull(env));
    return promise;
}

napi_value Common::ParseParaOnlyCallback(const napi_env &env, const napi_callback_info &info, napi_ref &callback)
{
    OPENCV_LOGI("Enter ParseParaOnlyCallback");
 
    size_t argc = ONLY_CALLBACK_MAX_PARA;
    napi_value argv[ONLY_CALLBACK_MAX_PARA] = {nullptr};
    napi_value thisVar = nullptr;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < ONLY_CALLBACK_MIN_PARA) {
        OPENCV_LOGE("Wrong number of arguments");
        return nullptr;
    }

    // argv[0]:callback
    napi_valuetype valuetype = napi_undefined;
    if (argc >= ONLY_CALLBACK_MAX_PARA) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM0], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.\n");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM0], 1, &callback);
    }

    return Common::NapiGetNull(env);
}

static bool CreateArrayBuffer(napi_env env, uint8_t* src, size_t srcLen, napi_value *res)
{
    OPENCV_LOGI("Enter CreateArrayBuffer");

    if (src == nullptr || srcLen == 0) {
        return false;
    }
    void *nativePtr = nullptr;
    if (napi_create_arraybuffer(env, srcLen, &nativePtr, res) != napi_ok || nativePtr == nullptr) {
        return false;
    }
    if (memcpy_s(nativePtr, srcLen, src, srcLen) != ERR_OK) {
        OPENCV_LOGE("memcpy_s failed.");
        return false;
    }
    return true;
}

napi_value Common::SetMat(const napi_env &env, const cv::Mat& mat, napi_value &result)
{
    OPENCV_LOGI("Enter SetMat");

    napi_value value = nullptr;
    // readonly rows?: number
    napi_create_int32(env, mat.rows, &value);
    napi_set_named_property(env, result, "rows", value);
    // readonly cols?: number
    napi_create_int32(env, mat.cols, &value);
    napi_set_named_property(env, result, "cols", value);
    // readonly type?: number
    napi_create_int32(env, mat.type(), &value);
    napi_set_named_property(env, result, "type", value);

    // readonly byteBuffer: ArrayBuffer
    uint32_t bufferSize = Common::GetMatDataBuffSize(mat);

    OPENCV_LOGI("Mat data bufferSize: %{public}d", bufferSize);

    void *buffer = mat.data;

    napi_value array;
    if (!CreateArrayBuffer(env, static_cast<uint8_t*>(buffer), bufferSize, &array)) {
        OPENCV_LOGE("napi_create_arraybuffer failed!");
        napi_get_undefined(env, &result);
    } else {
        OPENCV_LOGI("napi_create_arraybuffer sucessful!");
        napi_set_named_property(env, result, "byteBuffer", array);
    }

    return NapiGetBoolean(env, true);
}

napi_value Common::SetMat(const napi_env &env, int rows, int cols,
                          int type, int bufferSize, void* data, napi_value &result)
{
    OPENCV_LOGI("Enter SetMat");

    napi_value value = nullptr;
    // readonly rows?: number
    napi_create_int32(env, rows, &value);
    napi_set_named_property(env, result, "rows", value);
    // readonly cols?: number
    napi_create_int32(env, cols, &value);
    napi_set_named_property(env, result, "cols", value);
    // readonly type?: number
    napi_create_int32(env, type, &value);
    napi_set_named_property(env, result, "type", value);

    // readonly byteBuffer: ArrayBuffer
    void *buffer = data;
    napi_value array;
    if (!CreateArrayBuffer(env, static_cast<uint8_t*>(buffer), bufferSize, &array)) {
        napi_get_undefined(env, &result);
    } else {
        napi_set_named_property(env, result, "byteBuffer", array);
    }

    return NapiGetBoolean(env, true);
}

napi_value Common::GetMatRows(const napi_env &env, const napi_value &value, int &rows)
{
    OPENCV_LOGI("Enter GetMatRows");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "rows", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "rows", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("GetMatRows Wrong argument type. Number expected.");
            return nullptr;
        }

        napi_get_value_int32(env, result, &rows);
    }

    return NapiGetNull(env);
}

napi_value Common::GetMatCols(const napi_env &env, const napi_value &value, int &cols)
{
    OPENCV_LOGI("Enter GetMatCols");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "cols", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "cols", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("GetMatCols Wrong argument type. Number expected.");
            return nullptr;
        }
        napi_get_value_int32(env, result, &cols);
    }

    return NapiGetNull(env);
}

napi_value Common::GetMatType(const napi_env &env, const napi_value &value, int &type)
{
    OPENCV_LOGI("Enter GetMatType");
    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "type", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "type", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("GetMatType Wrong argument type. Number expected.");
            return nullptr;
        }
        napi_get_value_int32(env, result, &type);
    }

    return NapiGetNull(env);
}

napi_value Common::GetMatData(const napi_env &env, const napi_value &value, void* data)
{
    OPENCV_LOGI("Enter GetMatData");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;
    size_t bufferSize = 0;
    napi_status status;

    NAPI_CALL(env, napi_has_named_property(env, value, "byteBuffer", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "byteBuffer", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        status = napi_get_arraybuffer_info(env, result, &data, &bufferSize);
    }
    return NapiGetNull(env);
}

napi_value Common::GetMatInfo(const napi_env &env, const napi_value &value, MatInfo &matInfo)
{
    OPENCV_LOGI("Enter GetMatInfo");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "rows", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "rows", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of mat rows. Number expected.");
            return nullptr;
        }
        napi_get_value_int32(env, result, &(matInfo.rows));
    }

    NAPI_CALL(env, napi_has_named_property(env, value, "cols", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "cols", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of mat cols. Number expected.");
            return nullptr;
        }
        napi_get_value_int32(env, result, &(matInfo.cols));
    }

    NAPI_CALL(env, napi_has_named_property(env, value, "type", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "type", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of mat type. Number expected.");
            return nullptr;
        }
        napi_get_value_int32(env, result, &(matInfo.type));
    }
    
    NAPI_CALL(env, napi_has_named_property(env, value, "byteBuffer", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "byteBuffer", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        void *data = nullptr;
        size_t buffSize = 0;
        napi_get_arraybuffer_info(env, result, &data, &buffSize);
        if (buffSize == 0) {
            OPENCV_LOGE("Get arraybuffer size is zero.");
            return nullptr;
        }
        matInfo.data = new char[buffSize];
        if (!matInfo.data) {
            OPENCV_LOGE("Alloc memory failed.");
            return nullptr;
        }
        memcpy_s(matInfo.data, buffSize, data, buffSize);
        matInfo.bufferSize = buffSize;
    }

    return NapiGetNull(env);
}

napi_value Common::GetBooleanValue(const napi_env &env, const napi_value &value, bool &boolValue)
{
    OPENCV_LOGI("Enter GetBooleanValue");

    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_typeof(env, value, &valuetype));
    if (valuetype != napi_boolean) {
        OPENCV_LOGE("GetBooleanValue Wrong argument type. boolean expected.");
        return nullptr;
    }

    napi_get_value_bool(env, value, &boolValue);

    return NapiGetNull(env);
}

napi_value Common::GetInt32Value(const napi_env &env, const napi_value &value, int32_t &intValue)
{
    OPENCV_LOGI("Enter GetInt32Value");

    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_typeof(env, value, &valuetype));
    if (valuetype != napi_number) {
        OPENCV_LOGE("GetBooleanValue Wrong argument type. number expected.");
        return nullptr;
    }
    napi_get_value_int32(env, value, &intValue);

    return NapiGetNull(env);
}

napi_value Common::GetDoubleValue(const napi_env &env, const napi_value &value, double_t &doubleValue)
{
    OPENCV_LOGI("Enter GetDoubleValue");

    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_typeof(env, value, &valuetype));
    if (valuetype != napi_number) {
        OPENCV_LOGE("GetBooleanValue Wrong argument type. number expected.");
        return nullptr;
    }
    napi_get_value_double(env, value, &doubleValue);

    return NapiGetNull(env);
}

uint32_t Common::GetMatDataBuffSize(const Mat &mat)
{
    OPENCV_LOGI("Enter GetMatDataBuffSize");

    uint32_t dataBufferSize = mat.step[0] * mat.rows;

    return dataBufferSize;
}
bool Common::Copy2NapiMat(const Mat &inMat, MatInfo &napiMat)
{
    napiMat.rows = inMat.rows;
    napiMat.cols = inMat.cols;
    napiMat.type = inMat.type();
    uint32_t size = Common::GetMatDataBuffSize(inMat);
    OPENCV_LOGI("Mat data buffer size: %{public}d", size);
    if (size == 0) {
        OPENCV_LOGE("Mat size[%{public}d] is zero, please check!", size);
        return false;
    }
    napiMat.bufferSize = size;
    napiMat.data = new char[size];
    if (napiMat.data == nullptr) {
        OPENCV_LOGE("Alloc memory[%{public}d] failed.", size);
        return false;
    }
    memcpy_s(napiMat.data, size, inMat.data, size);
    return true;
}

napi_value Common::GetPointInfo(const napi_env &env, const napi_value &value, PointInfo &pt)
{
    OPENCV_LOGI("Enter GetPointInfo");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "x", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "x", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Point x. Number expected.");
            return nullptr;
        }

        napi_get_value_int32(env, result, &(pt.x));
    }

    NAPI_CALL(env, napi_has_named_property(env, value, "y", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "y", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Point y. Number expected.");
            return nullptr;
        }

        napi_get_value_int32(env, result, &(pt.y));
    }

    return NapiGetNull(env);
}

napi_value Common::GetScalarInfo(const napi_env &env, const napi_value &value, ScalarInfo &scalar)
{
    OPENCV_LOGI("Enter GetScalarInfo");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "v0", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "v0", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Scalar v0. Number expected.");
            return nullptr;
        }
        napi_get_value_double(env, result, &(scalar.v0));
    }

    NAPI_CALL(env, napi_has_named_property(env, value, "v1", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "v1", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Scalar v1. Number expected.");
            return nullptr;
        }
        napi_get_value_double(env, result, &(scalar.v1));
    }
    
    NAPI_CALL(env, napi_has_named_property(env, value, "v2", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "v2", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Scalar v2. Number expected.");
            return nullptr;
        }
        napi_get_value_double(env, result, &(scalar.v2));
    }

    return NapiGetNull(env);
}

napi_value Common::GetPoint2fInfo(const napi_env &env, const napi_value &value, Point2fInfo &pt)
{
    OPENCV_LOGI("Enter GetPoint2fInfo");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "x", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "x", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));

        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Point2f x. Number expected.");
            return nullptr;
        }

        napi_get_value_double(env, result, &(pt.x));
    }

    NAPI_CALL(env, napi_has_named_property(env, value, "y", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "y", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Point2f y. Number expected.");
            return nullptr;
        }
        napi_get_value_double(env, result, &(pt.y));
    }

    return NapiGetNull(env);
}

napi_value Common::GetSizeInfo(const napi_env &env, const napi_value &value, SizeInfo &sz)
{
    OPENCV_LOGI("Enter GetSizeInfo");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "width", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "width", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Size width. Number expected.");
            return nullptr;
        }
        napi_get_value_int32(env, result, &(sz.width));
    }

    NAPI_CALL(env, napi_has_named_property(env, value, "height", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "height", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Size height. Number expected.");
            return nullptr;
        }
        napi_get_value_int32(env, result, &(sz.height));
    }

    return NapiGetNull(env);
}

napi_value Common::GetSize2fInfo(const napi_env &env, const napi_value &value, Size2fInfo &sz)
{
    OPENCV_LOGI("Enter GetSizeInfo");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "width", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "width", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));

        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Size width. Number expected.");
            return nullptr;
        }

        napi_get_value_double(env, result, &(sz.width));
    }

    NAPI_CALL(env, napi_has_named_property(env, value, "height", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "height", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("Wrong argument type of Size height. Number expected.");
            return nullptr;
        }
        napi_get_value_double(env, result, &(sz.height));
    }

    return NapiGetNull(env);
}

napi_value Common::GetStringInfo(const napi_env &env, const napi_value &value, std::string &str)
{
    OPENCV_LOGI("Enter GetStringInfo");

    napi_valuetype valuetype = napi_undefined;
    size_t bufLength = 0;

    NAPI_CALL(env, napi_typeof(env, value, &valuetype));
    if (valuetype != napi_string) {
        OPENCV_LOGE("Wrong argument type, string expected.");
        return nullptr;
    }

    napi_status status = napi_get_value_string_utf8(env, value, nullptr, 0, &bufLength);
    if (status == napi_ok && bufLength > 0) {
        char *tmpChr = (char *) malloc((bufLength + 1) * sizeof(char));
        if (tmpChr != nullptr) {
            status = napi_get_value_string_utf8(env, value, tmpChr, bufLength + 1, &bufLength);
            if (status != napi_ok) {
                OPENCV_LOGE("Get string value failed, ret:%d", status);
                free(tmpChr);
                tmpChr = nullptr;
                return nullptr;
            }
            str = std::string(tmpChr);
            free(tmpChr);
            tmpChr = nullptr;
        }
    }

    return NapiGetNull(env);
}

napi_value Common::SetRect(const napi_env &env, const cv::Rect& rect, napi_value &result)
{
    OPENCV_LOGI("enter SetRect");

    napi_value value = nullptr;
    napi_create_int32(env, rect.x, &value);
    napi_set_named_property(env, result, "x", value);

    napi_create_int32(env, rect.y, &value);
    napi_set_named_property(env, result, "y", value);

    napi_create_int32(env, rect.height, &value);
    napi_set_named_property(env, result, "height", value);

    napi_create_int32(env, rect.width, &value);
    napi_set_named_property(env, result, "width", value);

    return NapiGetBoolean(env, true);
}

napi_value Common::GetRect(const napi_env &env, const napi_value &value, ParamsRect &rect)
{
    OPENCV_LOGI("enter GetRect");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "x", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "x", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("GetMatRows Wrong argument type. Number expected.");
            return nullptr;
        }
        
        napi_get_value_int32(env, result, &(rect.x));
        OPENCV_LOGI("enter GetRect rect.x = %{public}d", rect.x);
    } 

    NAPI_CALL(env, napi_has_named_property(env, value, "y", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "y", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("GetMatRows Wrong argument type. Number expected.");
            return nullptr;
        }
        
        napi_get_value_int32(env, result, &(rect.y));
        OPENCV_LOGI("enter GetRect rect.y = %{public}d", rect.y);
    } 
    
    NAPI_CALL(env, napi_has_named_property(env, value, "height", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "height", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("GetMatRows Wrong argument type. Number expected.");
            return nullptr;
        }
        
        napi_get_value_int32(env,  result, &(rect.height));
        OPENCV_LOGI("enter GetRect rect.height=%{public}d",rect.height);
    } 
    
    // readonly width?: number
    NAPI_CALL(env, napi_has_named_property(env, value, "width", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "width", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("GetMatRows Wrong argument type. Number expected.");
            return nullptr;
        }
        
        napi_get_value_int32(env, result, &(rect.width));
        OPENCV_LOGI("enter GetRect rect.width = %{public}d", rect.width);
    } 
    
    return NapiGetNull(env);
}

napi_value Common::GetRects(const napi_env &env, const napi_value &value, std::vector<ParamsRect> &v_rects)
{
    OPENCV_LOGI("enter GetRects");

    napi_valuetype valuetype = napi_undefined;

    bool isArray = false;
    napi_is_array(env, value, &isArray);
    NAPI_ASSERT(env, isArray, "Wrong argument type. Array expected.");
    uint32_t length = 0;
    napi_get_array_length(env, value, &length);
    NAPI_ASSERT(env, length > 0, "The array is empty.");
    for (size_t i = 0; i < length; i++) {
        napi_value nRect = nullptr;
        napi_get_element(env, value, i, &nRect);
        NAPI_CALL(env, napi_typeof(env, nRect, &valuetype));
        NAPI_ASSERT(env, valuetype == napi_object, "Wrong argument type. Object expected.");
        ParamsRect rect;
        if (!Common::GetRect(env, nRect, rect)) {
            return nullptr;
        }
        v_rects.emplace_back(rect);
    }

    return Common::NapiGetNull(env);
}

napi_value Common::GetScalar(const napi_env &env, const napi_value &value, ParamsScalar &scalar)
{
    OPENCV_LOGI("enter GetScalar");

    napi_valuetype valuetype = napi_undefined;
    napi_value result = nullptr;
    bool hasProperty = false;

    NAPI_CALL(env, napi_has_named_property(env, value, "x", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "x", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("GetMatRows Wrong argument type. Number expected.");
            return nullptr;
        }
        
        napi_get_value_int32(env, result, &(scalar.scalar_x));     
        OPENCV_LOGI("enter GetRect scalar.scalar_x = %{public}d", scalar.scalar_x);
    } 

    NAPI_CALL(env, napi_has_named_property(env, value, "y", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "y", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("GetMatRows Wrong argument type. Number expected.");
            return nullptr;
        }
        
        napi_get_value_int32(env, result, &(scalar.scalar_y));    
        OPENCV_LOGI("enter GetRect scalar.scalar_y = %{public}d", scalar.scalar_y);
    } 

    NAPI_CALL(env, napi_has_named_property(env, value, "z", &hasProperty));
    if (hasProperty) {
        napi_get_named_property(env, value, "z", &result);
        NAPI_CALL(env, napi_typeof(env, result, &valuetype));
        if (valuetype != napi_number) {
            OPENCV_LOGE("GetMatRows Wrong argument type. Number expected.");
            return nullptr;
        }
        
        napi_get_value_int32(env, result, &(scalar.scalar_z));    
        OPENCV_LOGI("enter GetRect scalar.scalar_z=%{public}d", scalar.scalar_z);
    } 

    return Common::NapiGetNull(env);
}
}  // namespace OpencvNapi
}  // namespace OHOS
