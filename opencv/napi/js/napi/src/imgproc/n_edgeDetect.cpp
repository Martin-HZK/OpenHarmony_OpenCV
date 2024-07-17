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

namespace OHOS {
namespace OpencvNapi {
struct ParametersMatInfo {
    MatInfo mat;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfoEdgeDetect {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    ParametersMatInfo params;
    CallbackPromiseInfo info;
    MatInfo outMat;
};

napi_value ParseParametersEdgeDetect(const napi_env &env, const napi_callback_info &info, ParametersMatInfo &paras)
{
    OPENCV_LOGI("Enter ParseParametersEdgeDetect");

    size_t argc = 2;
    napi_value argv[2] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM1) {
        OPENCV_LOGE("Wrong number of arguments.");
        return nullptr;
    }

    Common::GetMatInfo(env, argv[PARAM0], paras.mat);
    // argv[1]: callback
    if (argc >= PARAM2) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM1], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM1], 1, &paras.callback);
    }

    return Common::NapiGetNull(env);
}

void AsyncCompleteCallbackEdgeDetect(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("Enter AsyncCompleteCallbackEdgeDetect.");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data.");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoEdgeDetect *>(data);
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
    FreeMemory<AsyncCallbackInfoEdgeDetect>(asynccallbackinfo);
}

napi_value n_edgeDetect(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_edgeDetect");

    ParametersMatInfo params;
    if (ParseParametersEdgeDetect(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfoEdgeDetect *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfoEdgeDetect {.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }

    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback,
                                       asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "edgeDetect", NAPI_AUTO_LENGTH, &resourceName);
    // Asynchronous function call
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            OPENCV_LOGI("lambda function of napi_create_async_work start.");

            AsyncCallbackInfoEdgeDetect *asynccallbackinfo = static_cast<AsyncCallbackInfoEdgeDetect *>(data);
            if (asynccallbackinfo) {
                Mat gray, blurImage, edge1, edge2, cedge;
                int edgeThresh = 1;
                int edgeThreshScharr = 1;

                cv::Mat image(asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols,
                              asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data);
                cedge.create(image.size(), image.type());
                cvtColor(image, gray, COLOR_BGR2GRAY);
                int width = 3;
                int height = 3;
                int mulsize = 3;
                blur(gray, blurImage, Size(width, height));
                // Run the edge detector on grayscale
                Canny(blurImage, edge1, edgeThresh, edgeThresh * mulsize);
                cedge = Scalar::all(0);
                image.copyTo(cedge, edge1);

                // Canny detector with scharr
                Mat dx, dy;
                int dxsize = 1;
                int dyszie = 0;
                Scharr(blurImage, dx, CV_16S, dxsize, dyszie);
                Scharr(blurImage, dy, CV_16S, dyszie, dxsize);

                Canny(dx, dy, edge2, edgeThreshScharr, edgeThreshScharr * mulsize);
                /// Using Canny's output as a mask, we display our result
                cedge = Scalar::all(0);
                image.copyTo(cedge, edge2);

#if defined OPENCV_NAPI_DEBUG
                bool ret = imwrite("/data/storage/el2/base/haps/entry/files/cedge2.jpg", cedge);
                OPENCV_LOGE("Canny result imwrite ret: %{public}d", ret);
#endif
                if (!Common::Copy2NapiMat(cedge, asynccallbackinfo->outMat)) {
                    OPENCV_LOGE("Copy2NapiMat failed.");
                }
            }
        },
        AsyncCompleteCallbackEdgeDetect,
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