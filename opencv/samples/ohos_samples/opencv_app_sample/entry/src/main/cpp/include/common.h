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

#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <stdio.h>
#include <js_native_api.h>
#include <js_native_api_types.h>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include "hilog/log.h"
#include "napi/native_api.h"
#include "rawfile/raw_file_manager.h"
#include "rawfile/raw_file.h"
#include "rawfile/raw_dir.h"

#define GLOBAL_RESMGR (0xFFEE)
constexpr int32_t RGB_565 = 2;
constexpr int32_t RGBA_8888 = 3;

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

constexpr int32_t ARGS_ONE = 1;
constexpr int32_t ARGS_TWO = 2;
constexpr int32_t ONLY_CALLBACK_MAX_PARA = 1;
constexpr int32_t ONLY_CALLBACK_MIN_PARA = 0;

struct CallbackPromiseInfo {
    napi_ref callback = nullptr;
    napi_deferred deferred = nullptr;
    bool isCallback = false;
    int32_t errorCode = 0;
};

template <typename T> void FreeMemory(T *p) {
    if (p == nullptr) {
        return;
    }
    delete p;
    p = nullptr;
}

template <typename T> void FreeMemoryArray(T *p) {
    if (p == nullptr) {
        return;
    }
    delete[] p;
    p = nullptr;
}
#define NAPI_RETVAL_NOTHING
#define NAPI_CALL_BASE(env, theCall, retVal) \
    do {                                     \
        if ((theCall) != 0) {                \ 
            return retVal;                   \
        }                                    \
    } while (0)

#define NAPI_CALL(env, theCall) NAPI_CALL_BASE(env, theCall, nullptr)
#define NAPI_CALL_RETURN_VOID(env, theCall) NAPI_CALL_BASE(env, theCall, NAPI_RETVAL_NOTHING)

extern bool GetMatFromRawFile(napi_env env, napi_value jsResMgr, const std::string &rawfileDir, const std::string &fileName, cv::Mat &srcImage);
extern bool cvtMat2Pixel(cv::InputArray _src, cv::OutputArray &_dst, int code);
extern napi_value NapiGetNull(napi_env env);
extern uint32_t GetMatDataBuffSize(const cv::Mat &mat);
extern bool CreateArrayBuffer(napi_env env, uint8_t *src, size_t srcLen, napi_value *res);
extern napi_value NapiGetUndefined(napi_env env);
extern napi_value GetCallbackErrorValue(napi_env env, int32_t errCode);
extern napi_value NapiGetBoolean(napi_env env, const bool &isValue);
extern uint32_t GetMatDataBuffSize(const cv::Mat &mat);
extern void SetCallback(const napi_env &env, const napi_ref &callbackIn, const int32_t &errorCode, const napi_value &result);
extern void SetPromise(const napi_env &env, const napi_deferred &deferred, const int32_t &errorCode, const napi_value &result);
extern void ReturnCallbackPromise(const napi_env &env, const CallbackPromiseInfo &info, const napi_value &result);
extern napi_value JSParaError(const napi_env &env, const napi_ref &callback);
extern void PaddingCallbackPromiseInfo(const napi_env &env, const napi_ref &callback, CallbackPromiseInfo &info, napi_value &promise);
extern bool WrapJsPixelInfoInfo(napi_env env, cv::Mat &outMat, napi_value &result);

#endif // _COMMON_H_
