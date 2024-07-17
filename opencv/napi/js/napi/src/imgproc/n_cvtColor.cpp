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

#include "n_imgproc.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv_log_wrapper.h"

using namespace OHOS::Opencv;

namespace OHOS {
namespace OpencvNapi {
struct ParametersMatInfo {
    MatInfo mat;
    int type = COLOR_BGR2GRAY;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfoCvtColor {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    ParametersMatInfo params;
    CallbackPromiseInfo info;
    MatInfo outMat;
};

napi_value ParseParametersCvtColor(const napi_env &env, const napi_callback_info &info, ParametersMatInfo &paras)
{
    OPENCV_LOGI("Enter ParseParametersCvtColor");

    size_t argc = 3;
    napi_value argv[3] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM2) {
        OPENCV_LOGE("Wrong number of arguments");
        return nullptr;
    }

    Common::GetMatInfo(env, argv[PARAM0], paras.mat);
    Common::GetInt32Value(env, argv[PARAM1], paras.type);
    // argv[1]: callback
    if (argc >= PARAM3) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM2], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM2], 1, &paras.callback);
    }

    return Common::NapiGetNull(env);
}

void AsyncCompleteCallbackCvtColor(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("Enter AsyncCompleteCallbackCvtColor");
    if (!data) {
        OPENCV_LOGE("Invalid async callback data\n");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoCvtColor *>(data);
    napi_value result = Common::NapiGetNull(env);

    asynccallbackinfo->info.errorCode = ERR_OK;
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
        if (asynccallbackinfo->outMat.data == nullptr) {
            asynccallbackinfo->info.errorCode = ERROR;
        } else {
            napi_create_object(env, &result);
            if (!Common::SetMat(env, asynccallbackinfo->outMat.rows, asynccallbackinfo->outMat.cols,
                                asynccallbackinfo->outMat.type, asynccallbackinfo->outMat.bufferSize,
                                asynccallbackinfo->outMat.data, result)) {
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
    FreeMemoryArray<char>((char*)asynccallbackinfo->outMat.data);
    FreeMemoryArray<char>((char*)(asynccallbackinfo->params.mat.data));
    FreeMemory<AsyncCallbackInfoCvtColor>(asynccallbackinfo);
}

napi_value n_cvtColor(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_cvtColor");

    ParametersMatInfo params;
    if (ParseParametersCvtColor(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfoCvtColor *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfoCvtColor {.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }

    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback,
                                       asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "cvtColor", NAPI_AUTO_LENGTH, &resourceName);
    // Asynchronous function call
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            OPENCV_LOGI("lambda function of napi_create_async_work start");

            AsyncCallbackInfoCvtColor *asynccallbackinfo = static_cast<AsyncCallbackInfoCvtColor *>(data);

            if (asynccallbackinfo) {
                cv::Mat inMat(asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols,
                              asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data);
                cv::Mat outMat;
                cv::cvtColor(inMat, outMat, asynccallbackinfo->params.type);

                if (!Common::Copy2NapiMat(outMat, asynccallbackinfo->outMat)) {
                    OPENCV_LOGE("Copy2NapiMat failed.");
                }
            }
        },
        AsyncCompleteCallbackCvtColor,
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