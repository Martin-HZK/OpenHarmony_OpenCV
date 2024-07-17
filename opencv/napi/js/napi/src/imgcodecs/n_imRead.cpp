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
struct ParametersMatGroup {
    std::string fileName = "";
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfoGetMat {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    std::string fileName;
    CallbackPromiseInfo info;
    cv::Mat mat; // 出参，返回值
};


napi_value ParseParametersImRead(const napi_env &env, const napi_callback_info &info, ParametersMatGroup &paras)
{
    OPENCV_LOGI("Enter ParseParameters\n");

    size_t argc = 2;
    napi_value argv[2] = {nullptr};
    napi_value thisVar = nullptr;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM1) {
        OPENCV_LOGE("Wrong number of arguments\n");
        return nullptr;
    }

    napi_valuetype valuetype = napi_undefined;
    // argv[0]: fileName: string
    NAPI_CALL(env, napi_typeof(env, argv[PARAM0], &valuetype));
    if (valuetype != napi_string) {
        OPENCV_LOGE("Wrong argument type. String expected.\n");
        return nullptr;
    }
    char str[STR_MAX_SIZE] = {0};
    size_t strLen = 0;
    NAPI_CALL(env, napi_get_value_string_utf8(env, argv[PARAM0], str, STR_MAX_SIZE - 1, &strLen));
    paras.fileName = str;

    // argv[1]: callback
    if (argc >= PARAM2) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM1], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.\n");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM1], 1, &paras.callback);
    }

    return Common::NapiGetNull(env);
}

void AsyncCompleteCallbackGetMat(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("Enter AsyncCompleteCallbackGetMat");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data\n");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoGetMat *>(data);
    napi_value result = Common::NapiGetNull(env);
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
        if (asynccallbackinfo->mat.empty()) {
            asynccallbackinfo->info.errorCode = ERROR;
        } else {
            napi_create_object(env, &result);

            if (!Common::SetMat(env, asynccallbackinfo->mat, result)) {
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
    FreeMemory<AsyncCallbackInfoGetMat>(asynccallbackinfo);

    OPENCV_LOGI("AsyncCompleteCallbackGetMat end.");
}

napi_value n_imRead(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_imRead");

    ParametersMatGroup params;
    if (ParseParametersImRead(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfoGetMat *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfoGetMat {.env = env, .asyncWork = nullptr, .fileName = params.fileName};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }
    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, params.callback, asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "imRead", NAPI_AUTO_LENGTH, &resourceName);
    // Asynchronous function call
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            OPENCV_LOGI("Enter lambda function of napi_create_async_work");

            AsyncCallbackInfoGetMat *asynccallbackinfo = static_cast<AsyncCallbackInfoGetMat *>(data);
            if (asynccallbackinfo) {
                Mat outMat = cv::imread(asynccallbackinfo->fileName);
                asynccallbackinfo->mat = outMat;
            }
        },
        AsyncCompleteCallbackGetMat,
        (void *)asynccallbackinfo,
        &asynccallbackinfo->asyncWork);

    NAPI_CALL(env, napi_queue_async_work(env, asynccallbackinfo->asyncWork));

    if (asynccallbackinfo->info.isCallback) {
        return Common::NapiGetNull(env);
    } else {
        return promise;
    }
}
} // OpencvNapi
} // OHOS