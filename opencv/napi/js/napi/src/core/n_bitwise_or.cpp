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

#include "n_core.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv_log_wrapper.h"

using namespace OHOS::Opencv;
using namespace cv;
using namespace std;

namespace OHOS {
namespace OpencvNapi {
struct InputParametersInfo {
    MatInfo mat;
    MatInfo secondMat;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfo {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    InputParametersInfo params;
    CallbackPromiseInfo info;
    MatInfo outMat;
};

napi_value ParseParametersBitwiseOr(const napi_env &env, const napi_callback_info &info, InputParametersInfo &paras)
{
    OPENCV_LOGI("Enter ParseParametersBitwiseOr\n");

    size_t argc = 3;
    napi_value argv[3] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM2) {
        OPENCV_LOGE("Wrong number[%{public}d] of arguments.", argc);
        return nullptr;
    }

    Common::GetMatInfo(env, argv[PARAM0], paras.mat);
    Common::GetMatInfo(env, argv[PARAM1], paras.secondMat);
    // argv[7]: callback
    if (argc >= PARAM3) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM2], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type, Function expected.");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM2], 1, &paras.callback);
    }

    return Common::NapiGetNull(env);
}

void AsyncCompleteCallbackBitwiseOr(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("Enter AsyncCompleteCallbackBitwiseOr");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data\n");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfo *>(data);
    napi_value result = Common::NapiGetNull(env);

    asynccallbackinfo->info.errorCode = ERR_OK;
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
        if (asynccallbackinfo->outMat.data == nullptr) {
            asynccallbackinfo->info.errorCode = ERROR;
            OPENCV_LOGE("out mat data is null.");
        } else {
            napi_create_object(env, &result);
            if (!Common::SetMat(env, asynccallbackinfo->outMat.rows, asynccallbackinfo->outMat.cols,
                                asynccallbackinfo->outMat.type, asynccallbackinfo->outMat.bufferSize,
                                asynccallbackinfo->outMat.data, result)) {
                asynccallbackinfo->info.errorCode = ERROR;
                result = Common::NapiGetNull(env);
                OPENCV_LOGE("Common::SetMat failed.");
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
    FreeMemoryArray<char>((char*)(asynccallbackinfo->params.secondMat.data));
    FreeMemory<AsyncCallbackInfo>(asynccallbackinfo);

    OPENCV_LOGI("AsyncCompleteCallbackBitwiseOr end.");
}

napi_value n_bitwise_or(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_bitwise_or");

    InputParametersInfo params;
    if (ParseParametersBitwiseOr(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfo *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfo {.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }

    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback,
                                       asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "bitwise_or", NAPI_AUTO_LENGTH, &resourceName);
    // Asynchronous function call
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            OPENCV_LOGI("Enter lambe function of napi_create_async_work");

            AsyncCallbackInfo *asynccallbackinfo = static_cast<AsyncCallbackInfo *>(data);
            if (asynccallbackinfo) {
                OPENCV_LOGI("Input mat info: rows=%{public}d, cols=%{public}d, type=%{public}d,"
                            "buffsize=%{public}d",
                            asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols,
                            asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.bufferSize);
                
                OPENCV_LOGI("Input second mat info: rows=%{public}d, cols=%{public}d,"
                            "type=%{public}d, buffsize=%{public}d",
                            asynccallbackinfo->params.secondMat.rows, asynccallbackinfo->params.secondMat.cols,
                            asynccallbackinfo->params.secondMat.type, asynccallbackinfo->params.secondMat.bufferSize);

                Mat image(asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols,
                          asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data);
                Mat secImage(asynccallbackinfo->params.secondMat.rows, asynccallbackinfo->params.secondMat.cols,
                             asynccallbackinfo->params.secondMat.type, asynccallbackinfo->params.secondMat.data);
                Mat outMat;
                bitwise_or(image, secImage, outMat);
                
#if defined OPENCV_NAPI_DEBUG
                bool ret = imwrite("/data/storage/el2/base/haps/entry/files/bitwise_or.jpg", outMat);
                OPENCV_LOGE("bitwise_and result imwrite ret: %{public}d", ret);
#endif

                if (!Common::Copy2NapiMat(outMat, asynccallbackinfo->outMat)) {
                    OPENCV_LOGE("Copy2NapiMat failed.");
                }
            }
        },
        AsyncCompleteCallbackBitwiseOr,
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