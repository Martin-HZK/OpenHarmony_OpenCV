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

#ifndef _OPENCV_NAPI_INTERFACE_H_
#define _OPENCV_NAPI_INTERFACE_H_

#include "napi/native_api.h"

napi_value Img2Gray(napi_env env, napi_callback_info info);
napi_value QRCodeIdentification(napi_env env, napi_callback_info info);
napi_value EdgeDetection(napi_env env, napi_callback_info info);
napi_value FaceDetect(napi_env env, napi_callback_info info);
#endif // _OPENCV_NAPI_INTERFACE_H_
