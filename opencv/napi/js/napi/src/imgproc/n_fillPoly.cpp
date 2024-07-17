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
    std::vector<PointInfo> pts;
    ScalarInfo color;
    int lineType;
    int shift;
    PointInfo offset;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfo {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    InputParametersInfo params;
    CallbackPromiseInfo info;
    MatInfo outMat;
};

napi_value GetPointArrayInfo(const napi_env &env, const napi_value &value, std::vector<PointInfo> &pts)
{
    OPENCV_LOGI("Enter GetPointArrayInfo");

    napi_value element = nullptr;
    bool isArray = false;
    uint32_t array_len = 0;
    // Methods to work with Arrays
    napi_is_array(env, value, &isArray);
    if (!isArray) {
        OPENCV_LOGE("Wrong argument type, Array expected.");
        return nullptr;
    }

    napi_get_array_length(env, value, &array_len);
    if (array_len == 0) {
        OPENCV_LOGE("Array length invalid, great zero expected.");
        return nullptr;
    }

    for (int index = 0; index < array_len; index++) {
        PointInfo pt = {0, 0};
        napi_get_element(env, value, index, &element);
        Common::GetPointInfo(env, element, pt);
        pts.push_back(pt);
    }
    return Common::NapiGetNull(env);
}

napi_value ParseParametersFillPoly(const napi_env &env, const napi_callback_info &info, InputParametersInfo &paras)
{
    OPENCV_LOGI("Enter ParseParametersFillPoly.");

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
    GetPointArrayInfo(env, argv[PARAM1], paras.pts);
    Common::GetScalarInfo(env, argv[PARAM2], paras.color);
    Common::GetInt32Value(env, argv[PARAM3], paras.lineType);
    Common::GetInt32Value(env, argv[PARAM4], paras.shift);
    Common::GetPointInfo(env, argv[PARAM5], paras.offset);
    // callback
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

void AsyncCompleteCallbackFillPoly(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("Enter AsyncCompleteCallbackFillPoly.");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data.");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfo *>(data);
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
    FreeMemory<AsyncCallbackInfo>(asynccallbackinfo);
}


napi_value n_fillPoly(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_fillPoly.");

    InputParametersInfo params;
    if (ParseParametersFillPoly(env, info, params) == nullptr) {
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
    napi_create_string_latin1(env, "fillPoly", NAPI_AUTO_LENGTH, &resourceName);
    // Asynchronous function call
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            OPENCV_LOGI("lambda function of napi_create_async_work start.");

            AsyncCallbackInfo *asynccallbackinfo = static_cast<AsyncCallbackInfo *>(data);
            if (asynccallbackinfo) {
                cv::Mat image(asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols,
                              asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data);
                Mat outMat;
                image.copyTo(outMat);
                std::vector<cv::Point> pts;
                for (int i = 0; i < asynccallbackinfo->params.pts.size(); i++) {
                    cv::Point pt(asynccallbackinfo->params.pts[i].x, asynccallbackinfo->params.pts[i].y);
                    pts.push_back(pt);
                }
                Scalar color(asynccallbackinfo->params.color.v0, asynccallbackinfo->params.color.v1,
                             asynccallbackinfo->params.color.v2);
                Point offset(asynccallbackinfo->params.offset.x, asynccallbackinfo->params.offset.y);

                cv::fillPoly(outMat, pts, color, asynccallbackinfo->params.lineType,
                             asynccallbackinfo->params.shift, offset);
#if defined OPENCV_NAPI_DEBUG
                bool ret = imwrite("/data/storage/el2/base/haps/entry/files/pyrDown.jpg", outMat);
                OPENCV_LOGE("resize result imwrite ret: %{public}d", ret);
#endif
                if (!Common::Copy2NapiMat(outMat, asynccallbackinfo->outMat)) {
                    OPENCV_LOGE("Copy2NapiMat failed.");
                }
            }
        },
        AsyncCompleteCallbackFillPoly,
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