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
#include "opencv2/core/utility.hpp"
#include "opencv_log_wrapper.h"

using namespace OHOS::Opencv;
using namespace cv;
using namespace std;

namespace OHOS {
namespace OpencvNapi {
struct InputParametersInfo {
    MatInfo mat;
    double maxval;
    int adaptiveMethod;
    int thresholdType;
    int blockSize;
    double C;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfoAdaptiveThreshold {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    InputParametersInfo params;
    CallbackPromiseInfo info;
    MatInfo outMat;
};

napi_value ParseParametersAdaptiveThreshold(const napi_env &env,
                                            const napi_callback_info &info,
                                            InputParametersInfo &paras)
{
    OPENCV_LOGI("Enter ParseParametersAdaptiveThreshold");

    size_t argc = 7;
    napi_value argv[7] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM6) {
        OPENCV_LOGE("Wrong number of arguments.");
        return nullptr;
    }

    Common::GetMatInfo(env, argv[PARAM0], paras.mat);
    Common::GetDoubleValue(env, argv[PARAM1], paras.maxval);
    Common::GetInt32Value(env, argv[PARAM2], paras.adaptiveMethod);
    Common::GetInt32Value(env, argv[PARAM3], paras.thresholdType);
    Common::GetInt32Value(env, argv[PARAM4], paras.blockSize);
    Common::GetDoubleValue(env, argv[PARAM5], paras.C);

    if (argc >= PARAM7) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM6], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM6], 1, &paras.callback);
    }

    return Common::NapiGetNull(env);
}

void AsyncCompleteCallbackAdaptiveThreshold(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("Enter AsyncCompleteCallbackAdaptiveThreshold.");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data.");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoAdaptiveThreshold *>(data);
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
    FreeMemory<AsyncCallbackInfoAdaptiveThreshold>(asynccallbackinfo);
}

napi_value n_adaptiveThreshold(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_threshold");

    InputParametersInfo params;
    if (ParseParametersAdaptiveThreshold(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfoAdaptiveThreshold *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfoAdaptiveThreshold {.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }

    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback,
                                       asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "adaptiveThreshold", NAPI_AUTO_LENGTH, &resourceName);
    // Asynchronous function call
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            OPENCV_LOGI("lambda function of napi_create_async_work start.");

            AsyncCallbackInfoAdaptiveThreshold *asynccallbackinfo =
                static_cast<AsyncCallbackInfoAdaptiveThreshold *>(data);
            if (asynccallbackinfo) {
                Mat image(asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols,
                          asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data);
                cvtColor(image, image, COLOR_BGR2GRAY);

                Mat destMat;
                adaptiveThreshold(image, destMat, asynccallbackinfo->params.maxval,
                                  asynccallbackinfo->params.adaptiveMethod, asynccallbackinfo->params.thresholdType,
                                  asynccallbackinfo->params.blockSize, asynccallbackinfo->params.C);

#if defined OPENCV_NAPI_DEBUG
                bool ret = imwrite("/data/storage/el2/base/haps/entry/files/threshold.jpg", destMat);
                OPENCV_LOGE("threshold result imwrite ret: %{public}d", ret);
#endif

                if (!Common::Copy2NapiMat(destMat, asynccallbackinfo->outMat)) {
                    OPENCV_LOGE("Copy2NapiMat failed.");
                }
            }
        },
        AsyncCompleteCallbackAdaptiveThreshold,
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