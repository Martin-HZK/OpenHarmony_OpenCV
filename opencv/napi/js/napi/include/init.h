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

#ifndef OPENCV_JS_NAPI_INCLUDE_INIT_H
#define OPENCV_JS_NAPI_INCLUDE_INIT_H

#include "napi/native_api.h"
#include "napi/native_node_api.h"

#if defined OPENCV_NAPI_SUPPORT_CORE
#include "n_core.h"
#endif

#if defined OPENCV_NAPI_SUPPORT_IMGCODECS
#include "n_imgcodecs.h"
#endif

#if defined OPENCV_NAPI_SUPPORT_IMGPROC
#include "n_imgproc.h"
#endif

#if defined OPENCV_NAPI_SUPPORT_OBJDETECT
#include "n_objdetect.h"
#endif

namespace OHOS {
namespace OpencvNapi {
#ifdef __cplusplus
extern "C" {
#endif

__attribute__((constructor)) void RegisterModule(void);
napi_value OpencvInit(napi_env env, napi_value exports);
static napi_value Init(napi_env env, napi_value exports);

#ifdef __cplusplus
}
#endif

/*
 * Module define
 */
napi_module _module = {

    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "opencv_napi",
    .nm_priv = ((void *)0),
    .reserved = {0}

};
}  // namespace OpencvNapi
}  // namespace OHOS
#endif  // OPENCV_JS_NAPI_INCLUDE_INIT_H
