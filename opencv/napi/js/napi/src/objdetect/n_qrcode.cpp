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

#include <string>
#include <iostream>
#include "n_objdetect.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv_log_wrapper.h"

using namespace std;
using namespace OHOS::Opencv;

namespace OHOS {
namespace OpencvNapi {
struct ParametersInfo {
    MatInfo mat;
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

napi_value ParseParametersQrcode(const napi_env &env, const napi_callback_info &info, ParametersInfo &paras)
{
    OPENCV_LOGI("Enter ParseParametersQrcode.");

    size_t argc = 4;
    napi_value argv[4] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM1) {
        OPENCV_LOGE("Wrong number of arguments.");
        return nullptr;
    }

    Common::GetMatInfo(env, argv[PARAM0], paras.mat);
    // argv[1]: detectOnly
    if (argc >= PARAM2) {
        Common::GetBooleanValue(env, argv[PARAM1], paras.detectOnly);
    }
    // arggv[2]: multiQR
    if (argc >= PARAM3) {
        Common::GetBooleanValue(env, argv[PARAM2], paras.multiQR);
    }
    // argv[3]: callback
    if (argc >= PARAM4) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM3], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM3], 1, &paras.callback);
    }

    return Common::NapiGetNull(env);
}

void runQRCode(QRCodeDetector& qrcode, const Mat& input,
               AsyncCallbackInfoQrcode &callbackInfo, vector<Point>& corners)
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
            }
        } else {
            bool detection_result = qrcode.detect(input, corners);
            callbackInfo.detected = detection_result;
        }
    } else {
        if (!detectOnly) {
            bool result_detection = qrcode.detectAndDecodeMulti(input, callbackInfo.decodeInfos, corners);
            callbackInfo.detected = result_detection;
        } else {
            bool result_detection = qrcode.detectMulti(input, corners);
            callbackInfo.detected = result_detection;
        }
    }
}

napi_value SetQrcode(const napi_env &env, bool detected, const vector<string> &decodeInfos, napi_value &result)
{
    OPENCV_LOGI("Enter SetQrcode.");

    napi_value value = nullptr;
    // readonly detected?: boolean
    napi_get_boolean(env, detected, &value);
    napi_set_named_property(env, result, "detected", value);

    napi_value decodeArray = nullptr;
    napi_status result_status = napi_create_array(env, &decodeArray);    // 创建一个napi数组
    if (result_status != napi_ok) {
        napi_get_undefined(env, &result);
        return Common::NapiGetBoolean(env, false);
    }
    OPENCV_LOGI("decodeInfos size:%{public}d", decodeInfos.size());
    for (int i = 0; i < decodeInfos.size(); i++) {
        OPENCV_LOGI("decodeInfos[%{public}d] info %{public}s", i, decodeInfos[i].c_str());
        napi_value decodeStr = nullptr;
        napi_create_string_utf8(env, decodeInfos[i].c_str(), NAPI_AUTO_LENGTH, &decodeStr);
        napi_set_element(env, decodeArray, i, decodeStr); // 添加一个napi元素到napi数组
    }
    napi_set_named_property(env, result, "decodes", decodeArray);

    return Common::NapiGetBoolean(env, true);
}

void AsyncCompleteCallbackQrcode(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGE("Enter AsyncCompleteCallbackCvtColor.");
    if (!data) {
        OPENCV_LOGE("Invalid async callback data.");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoQrcode *>(data);
    napi_value result = Common::NapiGetNull(env);

    asynccallbackinfo->info.errorCode = ERR_OK;
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
        if (asynccallbackinfo->params.mat.data == nullptr) {
            asynccallbackinfo->info.errorCode = ERROR;
        } else {
            napi_create_object(env, &result);
            if (!SetQrcode(env, asynccallbackinfo->detected, asynccallbackinfo->decodeInfos, result)) {
                asynccallbackinfo->info.errorCode = ERROR;
                result = Common::NapiGetNull(env);
            }
        }
    }
    Common::ReturnCallbackPromise(env, asynccallbackinfo->info, result);
    if (asynccallbackinfo->info.callback != nullptr) {
        napi_delete_reference(env, asynccallbackinfo->info.callback);
    }
    napi_delete_async_work(env, asynccallbackinfo->asyncWork);
    FreeMemoryArray<char>((char*)(asynccallbackinfo->params.mat.data));
    FreeMemory<AsyncCallbackInfoQrcode>(asynccallbackinfo);
}

napi_value n_qrcode(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_qrcode.");

    ParametersInfo params;
    if (ParseParametersQrcode(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfoQrcode *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfoQrcode {.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }
    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback,
                                       asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "qrcode", NAPI_AUTO_LENGTH, &resourceName);
    // Asynchronous function call
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            OPENCV_LOGE("lambda function of napi_create_async_work start.");
            AsyncCallbackInfoQrcode *asynccallbackinfo = static_cast<AsyncCallbackInfoQrcode *>(data);
            if (asynccallbackinfo) {
                cv::Mat inMat(asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols,
                              asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data);
                QRCodeDetector qrcode;
                vector<Point> corners;
                runQRCode(qrcode, inMat, *asynccallbackinfo, corners);
            }
        },
        AsyncCompleteCallbackQrcode,
        (void*)asynccallbackinfo,
        &(asynccallbackinfo->asyncWork));

    NAPI_CALL(env, napi_queue_async_work(env, asynccallbackinfo->asyncWork));

    if (asynccallbackinfo->info.isCallback) {
        return Common::NapiGetNull(env);
    } else {
        return promise;
    }
}
} // OpencvNapi
} // OHOS
