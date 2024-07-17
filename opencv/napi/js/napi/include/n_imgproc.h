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

#ifndef OPENCV_NAPI_IMGPROC_H
#define OPENCV_NAPI_IMGPROC_H

#include "common.h"

namespace OHOS {
namespace OpencvNapi {
napi_value n_cvtColor(napi_env env, napi_callback_info info);
napi_value n_edgeDetect(napi_env env, napi_callback_info info);
napi_value n_resize(napi_env env, napi_callback_info info);
napi_value n_warpAffine(napi_env env, napi_callback_info info);
napi_value n_threshold(napi_env env, napi_callback_info info);
napi_value n_adaptiveThreshold(napi_env env, napi_callback_info info);
napi_value n_warpPolar(napi_env env, napi_callback_info info);
napi_value n_circle(napi_env env, napi_callback_info info);
napi_value n_line(napi_env env, napi_callback_info info);
napi_value n_ellipse(napi_env env, napi_callback_info info);
napi_value n_putText(napi_env env, napi_callback_info info);
napi_value n_pyrDown(napi_env env, napi_callback_info info);
napi_value n_pyrUp(napi_env env, napi_callback_info info);
napi_value n_fillPoly(napi_env env, napi_callback_info info);
napi_value n_filter2D(napi_env env, napi_callback_info info);
napi_value n_blur(napi_env env, napi_callback_info info);
napi_value n_boxFilter(napi_env env, napi_callback_info info);
napi_value n_sqrBoxFilter(napi_env env, napi_callback_info info);
napi_value n_GaussianBlur(napi_env env, napi_callback_info info);
napi_value n_bilateralFilter(napi_env env, napi_callback_info info);
napi_value n_medianBlur(napi_env env, napi_callback_info info);
napi_value n_Sobel(napi_env env, napi_callback_info info);
napi_value n_Canny(napi_env env, napi_callback_info info);
napi_value n_Laplacian(napi_env env, napi_callback_info info);
napi_value n_Scharr(napi_env env, napi_callback_info info);
napi_value n_findContours(napi_env env, napi_callback_info info);
napi_value n_HoughLines(napi_env env, napi_callback_info info);
napi_value n_equalizeHist(napi_env env, napi_callback_info info);
napi_value n_mat2pixeldata(napi_env env, napi_callback_info info);
napi_value n_rectangle(napi_env env, napi_callback_info info);
}  // namespace OpencvNapi
}  // namespace OHOS
#endif  // OPENCV_NAPI_IMGPROC_H
