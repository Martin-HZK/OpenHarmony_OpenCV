/*
 * Copyright (c) 2024 Shenzhen Kaihong Digital Industry Development Co., Ltd.
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

// #include <aki/jsbind.h>
#include <iostream>
#include <fstream>
#include "common.h"

using namespace cv;
static const char *TAG = "[opencv_edgeDetection]";

napi_value EdgeDetection(napi_env env, napi_callback_info info) {
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "EdgeDetection Begin");
    napi_value result = NapiGetNull(env);
    size_t argc = 3;
    napi_value argv[3] = {nullptr};

    napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

    size_t strSize;
    char strBuf[256];
    napi_get_value_string_utf8(env, argv[1], strBuf, sizeof(strBuf), &strSize);
    std::string fileDir(strBuf, strSize);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "fileDir：%{public}s", fileDir.c_str());

    napi_get_value_string_utf8(env, argv[2], strBuf, sizeof(strBuf), &strSize);
    std::string fileName(strBuf, strSize);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "fileName：%{public}s", fileName.c_str());

    Mat srcImage;
    if (!GetMatFromRawFile(env, argv[0], fileDir, fileName, srcImage)) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "Get Mat from rawfile failed!.");
        return result;
    }

    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG,
                 "srcImage total：%{public}d, cols: %{public}d, rows: %{public}d, channel: %{public}d",
                 srcImage.total(), srcImage.cols, srcImage.rows, srcImage.channels());

    Mat cannyImage;
    Canny(srcImage, cannyImage, 50, 20);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "Canny end");

    // 將图像转换为pixelMap格式
    Mat outMat;
    cvtMat2Pixel(cannyImage, outMat, RGBA_8888);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "outMat size: %{public}d, cols：%{public}d, rows：%{public}d",
                 outMat.total(), outMat.cols, outMat.rows);

    napi_create_object(env, &result);
    bool retVal = WrapJsPixelInfoInfo(env, outMat, result);
    if (!retVal) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "WrapJsInfo failed!.");
    }

    return result;
}
