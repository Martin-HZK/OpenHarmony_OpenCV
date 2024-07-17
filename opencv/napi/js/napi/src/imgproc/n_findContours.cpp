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
#include "opencv2/core/cvdef.h"
#include "opencv2/core/types_c.h"
#include "opencv2/core/hal/interface.h"

using namespace OHOS::Opencv;
using namespace cv;
using namespace std;

namespace OHOS {
namespace OpencvNapi {
struct InputParametersInfo {
    MatInfo mat;
    int mode;
    int method;
    PointInfo offset;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfo {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    InputParametersInfo params;
    CallbackPromiseInfo info;
    ContoursInfo outContours;
};

napi_value SetOutContours(const napi_env &env, const ContoursInfo &contoursInfo, napi_value &result)
{
    OPENCV_LOGI("Enter SetOutContours.");

    uint32_t array_len = 0;
    napi_value value = nullptr;
    napi_value outContoursArray = nullptr;
    napi_value outHierarchyArry = nullptr;
    
    napi_status result_status = napi_create_array(env, &outContoursArray);    // 创建一个napi数组contours
    if (result_status != napi_ok) {
        napi_get_undefined(env, &result);
        return Common::NapiGetBoolean(env, false);
    }

    result_status = napi_create_array(env, &outHierarchyArry);    // 创建一个napi数组hierarchy
    if (result_status != napi_ok) {
        napi_get_undefined(env, &result);
        return Common::NapiGetBoolean(env, false);
    }

    for (int i = 0; i < contoursInfo.contours.size(); i++) {
        napi_value contourArray = nullptr;
        napi_create_array(env, &contourArray);
        for (int j = 0; j < contoursInfo.contours[i].size(); j++) {
            napi_value point = nullptr;
            napi_create_object(env, &point);

            napi_create_int32(env, contoursInfo.contours[i][j].x, &value);
            napi_set_named_property(env, point, "x", value);

            napi_create_int32(env, contoursInfo.contours[i][j].y, &value);
            napi_set_named_property(env, point, "y", value);

            napi_set_element(env, contourArray, j, point);
        }
        
        napi_get_array_length(env, contourArray, &array_len);

        napi_set_element(env, outContoursArray, i, contourArray); // 添加一个napi元素到napi数组

        napi_value hierarchy = nullptr;
        napi_create_object(env, &hierarchy);

        napi_create_int32(env, contoursInfo.hierarchy[i].v0, &value);
        napi_set_named_property(env, hierarchy, "v0", value);
        napi_create_int32(env, contoursInfo.hierarchy[i].v1, &value);
        napi_set_named_property(env, hierarchy, "v1", value);
        napi_create_int32(env, contoursInfo.hierarchy[i].v2, &value);
        napi_set_named_property(env, hierarchy, "v2", value);
        napi_create_int32(env, contoursInfo.hierarchy[i].v3, &value);
        napi_set_named_property(env, hierarchy, "v3", value);
        napi_set_element(env, outHierarchyArry, i, hierarchy); // 添加一个napi元素到napi数组
    }
    napi_set_named_property(env, result, "contours", outContoursArray);
    napi_set_named_property(env, result, "hierarchy", outHierarchyArry);

    napi_get_array_length(env, outContoursArray, &array_len);
    OPENCV_LOGI("SetOutContours contours size:%{public}d", array_len);

    napi_get_array_length(env, outHierarchyArry, &array_len);
    OPENCV_LOGI("SetOutContours hierachy size:%{public}d", array_len);

    return Common::NapiGetBoolean(env, true);
}

napi_value ParseParametersFindContours(const napi_env &env, const napi_callback_info &info, InputParametersInfo &paras)
{
    OPENCV_LOGI("Enter ParseParametersFindContours.");

    size_t argc = 5;
    napi_value argv[5] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM4) {
        OPENCV_LOGE("Wrong number of arguments.");
        return nullptr;
    }

    Common::GetMatInfo(env, argv[PARAM0], paras.mat);
    Common::GetInt32Value(env, argv[PARAM1], paras.mode);
    Common::GetInt32Value(env, argv[PARAM2], paras.method);
    Common::GetPointInfo(env, argv[PARAM3], paras.offset);
    // callback
    if (argc >= PARAM5) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM4], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM4], 1, &paras.callback);
    }

    return Common::NapiGetNull(env);
}

void AsyncCompleteCallbackFindContours(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("Enter AsyncCompleteCallbackFindContours.");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data.");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfo *>(data);
    napi_value result = Common::NapiGetNull(env);

    asynccallbackinfo->info.errorCode = ERR_OK;
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
        if (asynccallbackinfo->outContours.contours.size() == 0) {
            asynccallbackinfo->info.errorCode = ERROR;
        } else {
            napi_create_object(env, &result);
            if (!SetOutContours(env, asynccallbackinfo->outContours, result)) {
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
    FreeMemory<AsyncCallbackInfo>(asynccallbackinfo);
}

napi_value n_findContours(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_findContours.");

    InputParametersInfo params;
    if (ParseParametersFindContours(env, info, params) == nullptr) {
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
    napi_create_string_latin1(env, "findContours", NAPI_AUTO_LENGTH, &resourceName);
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
                
                cv::Mat grayImg, GaussImg;
                cv::cvtColor(image, grayImg, COLOR_BGR2GRAY);
                int width = 3;
                int height = 3;
                cv::GaussianBlur(grayImg, GaussImg, Size(width, height), 0);

                double threadhold1 = 100;
                double threadhold2 = 250;
                cv::Canny(GaussImg, GaussImg, threadhold1, threadhold2);

                vector<vector<Point>> contours;
                vector<Vec4i> hierarchy;
                Point offset(asynccallbackinfo->params.offset.x, asynccallbackinfo->params.offset.y);
                cv::findContours(GaussImg, contours, hierarchy, asynccallbackinfo->params.mode,
                                 asynccallbackinfo->params.method, offset);

                OPENCV_LOGI("contours[%{public}d] hierarchy[%{public}d]", contours.size(), hierarchy.size());

                for (int i = 0; i < contours.size(); i++) {
                    // contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
                    vector<PointInfo> pts;
                    for (int j = 0; j < contours[i].size(); j++) {
                        PointInfo pt = {contours[i][j].x, contours[i][j].y};
                        pts.push_back(pt);
                    }
                    asynccallbackinfo->outContours.contours.push_back(pts);
                    Hierarchy4iInfo hierInfo;
                    int v0 = 0;
                    int v1 = 1;
                    int v2 = 2;
                    int v3 = 3;
                    hierInfo.v0 = hierarchy[i][v0];
                    hierInfo.v1 = hierarchy[i][v1];
                    hierInfo.v2 = hierarchy[i][v2];
                    hierInfo.v3 = hierarchy[i][v3];

                    asynccallbackinfo->outContours.hierarchy.push_back(hierInfo);
                }

                OPENCV_LOGI("out contours[%{public}d] hierarchy[%{public}d]",
                            asynccallbackinfo->outContours.contours.size(),
                            asynccallbackinfo->outContours.hierarchy.size());

                OPENCV_LOGI("lambda func finish.");
            }
        },
        AsyncCompleteCallbackFindContours,
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