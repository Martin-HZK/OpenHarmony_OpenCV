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

#ifndef OPENCV_JS_NAPI_INCLUDE_COMMON_H
#define OPENCV_JS_NAPI_INCLUDE_COMMON_H

#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "opencv2/core/core.hpp"

namespace OHOS {
namespace OpencvNapi {
using namespace cv;

constexpr int32_t STR_MAX_SIZE = 200;
constexpr int32_t LONG_STR_MAX_SIZE = 1024;
constexpr int32_t ERR_OK = 0;
constexpr int8_t NO_ERROR = 0;
constexpr int8_t ERROR = -1;
constexpr uint8_t PARAM0 = 0;
constexpr uint8_t PARAM1 = 1;
constexpr uint8_t PARAM2 = 2;
constexpr uint8_t PARAM3 = 3;
constexpr uint8_t PARAM4 = 4;
constexpr uint8_t PARAM5 = 5;
constexpr uint8_t PARAM6 = 6;
constexpr uint8_t PARAM7 = 7;
constexpr uint8_t PARAM8 = 8;
constexpr uint8_t PARAM9 = 9;
constexpr uint8_t PARAM10 = 10;
constexpr uint8_t PARAM11 = 11;
constexpr uint8_t PARAM12 = 12;

struct CallbackPromiseInfo {
    napi_ref callback = nullptr;
    napi_deferred deferred = nullptr;
    bool isCallback = false;
    int32_t errorCode = 0;
};

// NAPI Mat
struct MatInfo {
    int rows = 0;
    int cols = 0;
    int type = 0;
    void* data = nullptr;
    size_t bufferSize = 0;
};
struct ParamsRect {
    int x;
    int y;
    int height;
    int width;
};
struct ParamsScalar {
    int scalar_x;
    int scalar_y;
    int scalar_z;
};
// NAPI Point
struct PointInfo {
    int x = 0;
    int y = 0;
};

// NAPI Scalar
struct ScalarInfo {
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
};

// NAPI Point2f
struct Point2fInfo {
    double x = 0.0;
    double y = 0.0;
};

// NAPI Size
struct SizeInfo {
    int width = 0;
    int height = 0;
};

struct Size2fInfo {
    double width = 0.0;
    double height = 0.0;
};

struct Hierarchy4iInfo {
    int v0;
    int v1;
    int v2;
    int v3;
};

struct ContoursInfo {
    std::vector<std::vector<PointInfo>> contours;
    std::vector<Hierarchy4iInfo> hierarchy;
};

struct Line2fInfo {
    float rho;
    float theta;
};

class Common {
    Common();

    ~Common();

public:
    /**
     * @brief Gets a napi value that is used to represent specified bool value
     *
     * @param env Indicates the environment that the API is invoked under
     * @param isValue Indicates a bool value
     * @return Returns a napi value that is used to represent specified bool value
     */
    static napi_value NapiGetBoolean(napi_env env, const bool &isValue);

    /**
     * @brief Gets the napi value that is used to represent the null object
     *
     * @param env Indicates the environment that the API is invoked under
     * @return Returns the napi value that is used to represent the null object
     */
    static napi_value NapiGetNull(napi_env env);

    /**
     * @brief Gets the napi value that is used to represent the undefined object
     *
     * @param env Indicates the environment that the API is invoked under
     * @return Returns the napi value that is used to represent the undefined object
     */
    static napi_value NapiGetUndefined(napi_env env);

    /**
     * @brief Gets a napi value with specified error code for callback
     *
     * @param env Indicates the environment that the API is invoked under
     * @param errCode Indicates specified err code
     * @return Returns a napi value with specified error code for callback
     */
    static napi_value GetCallbackErrorValue(napi_env env, int32_t errCode);

    /**
     * @brief Pads the CallbackPromiseInfo struct
     *
     * @param env Indicates the environment that the API is invoked under
     * @param callback Indicates a napi_ref for callback
     * @param info Indicates the CallbackPromiseInfo struct to be padded
     * @param promise Indicates the promise to be created when the callback is null
     */
    static void PaddingCallbackPromiseInfo(
        const napi_env &env, const napi_ref &callback, CallbackPromiseInfo &info, napi_value &promise);

    /**
     * @brief Gets the returned result by the CallbackPromiseInfo struct
     *
     * @param env Indicates the environment that the API is invoked under
     * @param info Indicates the CallbackPromiseInfo struct
     * @param result Indicates the returned result
     */
    static void ReturnCallbackPromise(const napi_env &env, const CallbackPromiseInfo &info, const napi_value &result);

    /**
     * @brief Calls the callback with the result and error code
     *
     * @param env Indicates the environment that the API is invoked under
     * @param callbackIn Indicates the callback to be called
     * @param errCode Indicates the error code returned by the callback
     * @param result Indicates the result returned by the callback
     */
    static void SetCallback(
        const napi_env &env, const napi_ref &callbackIn, const int32_t &errorCode, const napi_value &result);

    /**
     * @brief Calls the callback with the result
     *
     * @param env Indicates the environment that the API is invoked under
     * @param callbackIn Indicates the callback to be called
     * @param result Indicates the result returned by the callback
     */
    static void SetCallback(
        const napi_env &env, const napi_ref &callbackIn, const napi_value &result);

    /**
     * @brief Processes the promise with the result and error code
     *
     * @param env Indicates the environment that the API is invoked under
     * @param deferred Indicates the deferred object whose associated promise to resolve
     * @param errorCode Indicates the error code returned by the callback
     * @param result Indicates the result returned by the callback
     */
    static void SetPromise(
        const napi_env &env, const napi_deferred &deferred, const int32_t &errorCode, const napi_value &result);

    /**
     * @brief Gets the returned result by the callback when an error occurs
     *
     * @param env Indicates the environment that the API is invoked under
     * @param callback Indicates a napi_ref for callback
     * @return Returns the null object
     */
    static napi_value JSParaError(const napi_env &env, const napi_ref &callback);

    /**
     * @brief Parses a single parameter for callback
     *
     * @param env Indicates the environment that the API is invoked under
     * @param info Indicates the callback info passed into the callback function
     * @param callback Indicates the napi_ref for the callback parameter
     * @return Returns the null object if success, returns the null value otherwise
     */
    static napi_value ParseParaOnlyCallback(const napi_env &env, const napi_callback_info &info, napi_ref &callback);
        /**
     * @brief Sets a js object by specified Mat object
     *
     * @param env Indicates the environment that the API is invoked under
     * @param mat Indicates a Mat object to be converted
     * @param result Indicates a js object to be set
     * @return Returns the null object if success, returns the null value otherwise
     */
    static napi_value SetMat(const napi_env &env, const cv::Mat& mat, napi_value &result);

    static napi_value SetMat(const napi_env &env, int rows, int cols,
                             int type, int bufferSize, void* data, napi_value &result);
    static napi_value SetRect(const napi_env &env, const cv::Rect& rect, napi_value &result);

    static napi_value GetMatRows(const napi_env &env, const napi_value &value, int &rows);
    static napi_value GetMatCols(const napi_env &env, const napi_value &value, int &cols);
    static napi_value GetMatType(const napi_env &env, const napi_value &value, int &type);
    static napi_value GetMatData(const napi_env &env, const napi_value &value, void* data);
    static napi_value GetBooleanValue(const napi_env &env, const napi_value &value, bool &boolValue);
    static napi_value GetInt32Value(const napi_env &env, const napi_value &value, int32_t &intValue);
    static napi_value GetDoubleValue(const napi_env &env, const napi_value &value, double_t &doubleValue);
    static uint32_t GetMatDataBuffSize(const Mat& mat);
    static napi_value GetMatInfo(const napi_env &env, const napi_value &value, MatInfo &matInfo);
    static bool Copy2NapiMat(const Mat &inMat, MatInfo &napiMat);
    static napi_value GetPointInfo(const napi_env &env, const napi_value &value, PointInfo &pt);
    static napi_value GetScalarInfo(const napi_env &env, const napi_value &value, ScalarInfo &scalar);
    static napi_value GetPoint2fInfo(const napi_env &env, const napi_value &value, Point2fInfo &pt);
    static napi_value GetSizeInfo(const napi_env &env, const napi_value &value, SizeInfo &sz);
    static napi_value GetSize2fInfo(const napi_env &env, const napi_value &value, Size2fInfo &sz);
    static napi_value GetStringInfo(const napi_env &env, const napi_value &value, std::string &str);
    static napi_value GetRect(const napi_env &env, const napi_value &value, ParamsRect &rect);
    static napi_value GetRects(const napi_env &env, const napi_value &value, std::vector<ParamsRect> &v_rects);
    static napi_value GetScalar(const napi_env &env, const napi_value &value, ParamsScalar &scalar);
private:
    static const int32_t ARGS_ONE = 1;
    static const int32_t ARGS_TWO = 2;
    static const int32_t ONLY_CALLBACK_MAX_PARA = 1;
    static const int32_t ONLY_CALLBACK_MIN_PARA = 0;
};

template <typename T>
void FreeMemory(T *p)
{
    if (p == nullptr) {
        return;
    }
    delete p;
    p = nullptr;
}

template <typename T>
void FreeMemoryArray(T *p)
{
    if (p == nullptr) {
        return;
    }
    delete[] p;
    p = nullptr;
}
} // OpencvNapi
} // OHOS
#endif  // OPENCV_JS_NAPI_INCLUDE_COMMON_H
