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
    double rho;
    double theta;
    int threshold;
    double srn = 0;
    double stn = 0;
    double min_theta = 0;
    double max_theta = CV_PI;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfo {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    InputParametersInfo params;
    CallbackPromiseInfo info;
    vector<Line2fInfo> outLines;
    MatInfo outMat;
};

napi_value SetHoughLinesOut(const napi_env &env, const vector<Line2fInfo> &linesInfo, napi_value &result)
{
    OPENCV_LOGI("Enter SetHoughLinesOut.");

    uint32_t array_len = 0;
    napi_value value = nullptr;
    napi_value hlArray = nullptr;
    napi_status result_status = napi_create_array(env, &hlArray);
    if (result_status != napi_ok) {
        napi_get_undefined(env, &result);
        return Common::NapiGetBoolean(env, false);
    }

    for (uint32_t i = 0; i < linesInfo.size(); i++) {
        napi_value point = nullptr;
        napi_create_object(env, &point);

        napi_create_double(env, linesInfo[i].rho, &value);
        napi_set_named_property(env, point, "rho", value);
        napi_create_double(env, linesInfo[i].theta, &value);
        napi_set_named_property(env, point, "theta", value);
        napi_set_element(env, hlArray, i, point);

        OPENCV_LOGI("houghLine[%{public}u] info: rho : %{public}f, theta: %{public}f",
            i, linesInfo[i].rho, linesInfo[i].theta);
    }
    napi_get_array_length(env, hlArray, &array_len);
    OPENCV_LOGI("SetHoughLinesOut array length:%{public}d", array_len);

    napi_set_named_property(env, result, "houghLine", hlArray);
    return Common::NapiGetBoolean(env, true);
}

napi_value ParseParametersHoughLines(const napi_env &env, const napi_callback_info &info, InputParametersInfo &paras)
{
    OPENCV_LOGI("Enter ParseParametersHoughLines.");

    size_t argc = 9;
    napi_value argv[9] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM8) {
        OPENCV_LOGE("Wrong number of arguments.");
        return nullptr;
    }

    Common::GetMatInfo(env, argv[PARAM0], paras.mat);
    Common::GetDoubleValue(env, argv[PARAM1], paras.rho);
    Common::GetDoubleValue(env, argv[PARAM2], paras.theta);
    Common::GetInt32Value(env, argv[PARAM3], paras.threshold);
    Common::GetDoubleValue(env, argv[PARAM4], paras.srn);
    Common::GetDoubleValue(env, argv[PARAM5], paras.stn);
    Common::GetDoubleValue(env, argv[PARAM6], paras.min_theta);
    Common::GetDoubleValue(env, argv[PARAM7], paras.max_theta);
    // callback
    if (argc >= PARAM9) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM8], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM8], 1, &paras.callback);
    }

    return Common::NapiGetNull(env);
}

void AsyncCompleteCallbackHoughLines(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("Enter AsyncCompleteCallbackHoughLines.");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data.");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfo *>(data);
    napi_value result = Common::NapiGetNull(env);

    asynccallbackinfo->info.errorCode = ERR_OK;
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
        if (asynccallbackinfo->outLines.size() == 0) {
            asynccallbackinfo->info.errorCode = ERROR;
        } else {
            napi_create_object(env, &result);
            if (!SetHoughLinesOut(env, asynccallbackinfo->outLines, result)) {
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

napi_value n_HoughLines(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_HoughLines.");

    InputParametersInfo params;
    if (ParseParametersHoughLines(env, info, params) == nullptr) {
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
    napi_create_string_latin1(env, "HoughLines", NAPI_AUTO_LENGTH, &resourceName);
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
                cv::Mat grayImg, houghLiImg;
                cv::cvtColor(image, grayImg, COLOR_BGR2GRAY);
                double threshold1 = 50;
                double threshold2 = 200;
                int apertureSize = 3;
                cv::Canny(grayImg, houghLiImg, threshold1, threshold2, apertureSize);
                vector<Vec2f> lines;
                
                cv::HoughLines(houghLiImg, lines, asynccallbackinfo->params.rho, asynccallbackinfo->params.theta,
                               asynccallbackinfo->params.threshold, asynccallbackinfo->params.srn,
                               asynccallbackinfo->params.stn, asynccallbackinfo->params.min_theta,
                               asynccallbackinfo->params.max_theta);

                OPENCV_LOGI("HoughLines result lines size: %{public}d", lines.size());
                cv::Mat outMat;
                image.copyTo(outMat);

                for (size_t i = 0; i < lines.size(); i++) {
                    float rho = lines[i][0];
                    float theta = lines[i][1];

                    Point pt1, pt2;
                    double a = cos(theta), b = sin(theta);
                    double x0 = a*rho, y0 = b*rho;
                    int seed = 1000;
                    pt1.x = cvRound(x0 + seed*(-b));
                    pt1.y = cvRound(y0 + seed*(a));
                    pt2.x = cvRound(x0 - seed*(-b));
                    pt2.y = cvRound(y0 - seed*(a));
                    double v1 = 0;
                    double v2 = 0;
                    double v3 = 255;
                    int thickness = 3;
                    line(outMat, pt1, pt2, Scalar(v1, v2, v3), thickness, LINE_AA);

                    Line2fInfo lineVec = {rho, theta};
                    asynccallbackinfo->outLines.push_back(lineVec);
                }

#if defined OPENCV_NAPI_DEBUG
                bool ret = imwrite("/data/storage/el2/base/haps/entry/files/HoughLines.jpg", outMat);
                OPENCV_LOGI("resize result imwrite ret: %{public}d", ret);
#endif
            }
        },
        AsyncCompleteCallbackHoughLines,
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
