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

#include "n_imgcodecs.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv_log_wrapper.h"

using namespace OHOS::Opencv;

namespace OHOS {
namespace OpencvNapi {
struct ParametersImWrite {
    std::string fileName = "";
    MatInfo mat;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfoImWrite {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    ParametersImWrite params;
    void* colorsBuffer;
    size_t colorsBufferSize;
    CallbackPromiseInfo info;
};


napi_value ParseParametersImWrite(const napi_env &env, const napi_callback_info &info, ParametersImWrite &paras)
{
    OPENCV_LOGI("Enter ParseParametersImWrite");

    size_t argc = 3;
    napi_value argv[3] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM2) {
        OPENCV_LOGE("Wrong number of arguments.");
        return nullptr;
    }

    NAPI_CALL(env, napi_typeof(env, argv[PARAM0], &valuetype));
    if (valuetype != napi_string) {
        OPENCV_LOGE("Wrong argument type. String expected.");
        return nullptr;
    }
    char str[STR_MAX_SIZE] = {0};
    size_t strLen = 0;
    NAPI_CALL(env, napi_get_value_string_utf8(env, argv[PARAM0], str, STR_MAX_SIZE - 1, &strLen));
    paras.fileName = str;

    Common::GetMatInfo(env, argv[PARAM1], paras.mat);

    // argv[2]: callback
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

void AsyncCompleteCallbackImWrite(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("Enter AsyncCompleteCallbackImWrite.");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoImWrite *>(data);
    napi_value result = Common::NapiGetNull(env);
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
        if (asynccallbackinfo->params.mat.data == nullptr) {
            asynccallbackinfo->info.errorCode = ERROR;
        } else {
            napi_create_object(env, &result);
        }
    }
    Common::ReturnCallbackPromise(env, asynccallbackinfo->info, result);
    if (asynccallbackinfo->info.callback != nullptr) {
        napi_delete_reference(env, asynccallbackinfo->info.callback);
    }
    napi_delete_async_work(env, asynccallbackinfo->asyncWork);
    FreeMemory<AsyncCallbackInfoImWrite>(asynccallbackinfo);
}

napi_value n_imWrite(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_imWrite");

    ParametersImWrite params;
    if (ParseParametersImWrite(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfoImWrite *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfoImWrite {.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }

    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback,
                                       asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "imWrite", NAPI_AUTO_LENGTH, &resourceName);
    // Asynchronous function call
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            OPENCV_LOGI("Enter lambda function of napi_create_async_work");

            AsyncCallbackInfoImWrite *asynccallbackinfo = static_cast<AsyncCallbackInfoImWrite *>(data);
            if (asynccallbackinfo) {
                cv::Mat inMat(asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols,
                              asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data);
                cv::imwrite(asynccallbackinfo->params.fileName, inMat);
            }
            else {
                OPENCV_LOGE("asynccallbackinfo is nullptr");
            }
        },
        AsyncCompleteCallbackImWrite,
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
