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

#ifndef OPENCV_NAPI_OBJDETECT_H
#define OPENCV_NAPI_OBJDETECT_H

#include "common.h"

namespace OHOS {
namespace OpencvNapi {
napi_value n_qrcode(napi_env env, napi_callback_info info);
napi_value n_detectMultiScale(napi_env env, napi_callback_info info);
}  // namespace OpencvNapi
}  // namespace OHOS
#endif  // OPENCV_NAPI_OBJDETECT_H
