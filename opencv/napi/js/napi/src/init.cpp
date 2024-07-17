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

#include "init.h"
#include "opencv_log_wrapper.h"
using namespace OHOS::Opencv;

namespace OHOS {
namespace OpencvNapi {
using namespace cv;

EXTERN_C_START
napi_value OpencvInit(napi_env env, napi_value exports)
{
    napi_property_descriptor desc[] = {
#if defined OPENCV_NAPI_SUPPORT_IMGCODECS
        DECLARE_NAPI_FUNCTION("imRead", n_imRead),
        DECLARE_NAPI_FUNCTION("imWrite", n_imWrite),
#endif

#if defined OPENCV_NAPI_SUPPORT_IMGPROC
        DECLARE_NAPI_FUNCTION("cvtColor", n_cvtColor),
        DECLARE_NAPI_FUNCTION("resize", n_resize),
        DECLARE_NAPI_FUNCTION("warpAffine", n_warpAffine),
        DECLARE_NAPI_FUNCTION("edgeDetect", n_edgeDetect),
        DECLARE_NAPI_FUNCTION("equalizeHist", n_equalizeHist),
        DECLARE_NAPI_FUNCTION("rectangle", n_rectangle),
        DECLARE_NAPI_FUNCTION("mat2pixeldata", n_mat2pixeldata),
        DECLARE_NAPI_FUNCTION("threshold", n_threshold),
        DECLARE_NAPI_FUNCTION("adaptiveThreshold", n_adaptiveThreshold),
        DECLARE_NAPI_FUNCTION("warpPolar", n_warpPolar),
        DECLARE_NAPI_FUNCTION("circle", n_circle),
        DECLARE_NAPI_FUNCTION("line", n_line),
        DECLARE_NAPI_FUNCTION("ellipse", n_ellipse),
        DECLARE_NAPI_FUNCTION("putText", n_putText),
        DECLARE_NAPI_FUNCTION("pyrDown", n_pyrDown),
        DECLARE_NAPI_FUNCTION("pyrUp", n_pyrUp),
        DECLARE_NAPI_FUNCTION("fillPoly", n_fillPoly),
        DECLARE_NAPI_FUNCTION("filter2D", n_filter2D),
        DECLARE_NAPI_FUNCTION("blur", n_blur),
        DECLARE_NAPI_FUNCTION("boxFilter", n_boxFilter),
        DECLARE_NAPI_FUNCTION("sqrBoxFilter", n_sqrBoxFilter),
        DECLARE_NAPI_FUNCTION("GaussianBlur", n_GaussianBlur),
        DECLARE_NAPI_FUNCTION("bilateralFilter", n_bilateralFilter),
        DECLARE_NAPI_FUNCTION("medianBlur", n_medianBlur),
        DECLARE_NAPI_FUNCTION("Sobel", n_Sobel),
        DECLARE_NAPI_FUNCTION("Canny", n_Canny),
        DECLARE_NAPI_FUNCTION("Laplacian", n_Laplacian),
        DECLARE_NAPI_FUNCTION("Scharr", n_Scharr),
        DECLARE_NAPI_FUNCTION("findContours", n_findContours),
        DECLARE_NAPI_FUNCTION("HoughLines", n_HoughLines),
#endif

#if defined OPENCV_NAPI_SUPPORT_OBJDETECT
        DECLARE_NAPI_FUNCTION("qrcode", n_qrcode),
        DECLARE_NAPI_FUNCTION("detectMultiScale", n_detectMultiScale),
#endif

#if defined OPENCV_NAPI_SUPPORT_CORE
        DECLARE_NAPI_FUNCTION("copyMakeBorder", n_copyMakeBorder),
        DECLARE_NAPI_FUNCTION("vconcat", n_vconcat),
        DECLARE_NAPI_FUNCTION("hconcat", n_hconcat),
        DECLARE_NAPI_FUNCTION("flip", n_flip),
        DECLARE_NAPI_FUNCTION("bitwise_and", n_bitwise_and),
        DECLARE_NAPI_FUNCTION("bitwise_or", n_bitwise_or),
        DECLARE_NAPI_FUNCTION("bitwise_xor", n_bitwise_xor),
        DECLARE_NAPI_FUNCTION("bitwise_not", n_bitwise_not),
        DECLARE_NAPI_FUNCTION("convertScaleAbs", n_convertScaleAbs),
#endif
    };
    int count = sizeof(desc) / sizeof(desc[0]);
    OPENCV_LOGI("OpencvInit reg count: %{public}d", count);
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));

    return exports;
}

/*
 * Module export function
 */
static napi_value Init(napi_env env, napi_value exports)
{
    /*
     * Propertise define
     */
    OpencvInit(env, exports);
    return exports;
}

/*
 * Module register function
 */
__attribute__((constructor)) void RegisterModule(void)
{
    napi_module_register(&_module);
}
EXTERN_C_END
}  // namespace OpencvInit
}  // namespace OHOS
