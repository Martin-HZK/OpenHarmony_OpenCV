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

#ifndef OPENCV_NAPI_LOG_WRAPPER_H
#define OPENCV_NAPI_LOG_WRAPPER_H

#include <inttypes.h>
#include <string>
#include "hilog/log.h"

namespace OHOS {
namespace Opencv {
#ifndef EVENT_LOG_DOMAIN
#define EVENT_LOG_DOMAIN 0xD001200
#endif
#ifndef EVENT_LOG_TAG
#define EVENT_LOG_TAG "Ces"
#endif

enum class OpencvLogLevel { DEBUG = 0, INFO, WARN, ERROR, FATAL };

static constexpr OHOS::HiviewDFX::HiLogLabel Event_LABEL = {LOG_CORE, EVENT_LOG_DOMAIN, EVENT_LOG_TAG};

class OpencvLogWrapper {
public:
    /**
     * Judges the level of the log.
     *
     * @param level Indicates the level of the log.
     * @return Returns true if success; false otherwise.
     */
    static bool JudgeLevel(const OpencvLogLevel &level);

    /**
     * Sets the level of the log.
     *
     * @param level Indicates the level of the log.
     */
    static void SetLogLevel(const OpencvLogLevel &level)
    {
        level_ = level;
    }

    /**
     * Gets the level of the log.
     *
     * @return Returns the level of the log.
     */
    static const OpencvLogLevel &GetLogLevel()
    {
        return level_;
    }

    /**
     * Gets the brief name of the file.
     *
     * @param str Indicates the full name of the file.
     * @return Returns the full name of the file.
     */
    static std::string GetBriefFileName(const char *str);

private:
    static OpencvLogLevel level_;
};

#define PRINT_LOG(LEVEL, Level, fmt, ...)                    \
    if (OpencvLogWrapper::JudgeLevel(OpencvLogLevel::LEVEL))   \
    OHOS::HiviewDFX::HiLog::Level(Event_LABEL,               \
        "[%{public}s:(%{public}s):%{public}d] " fmt,         \
        OpencvLogWrapper::GetBriefFileName(__FILE__).c_str(), \
        __FUNCTION__,                                        \
        __LINE__,                                            \
        ##__VA_ARGS__)

#define OPENCV_LOGD(fmt, ...) PRINT_LOG(DEBUG, Debug, fmt, ##__VA_ARGS__)
#define OPENCV_LOGI(fmt, ...) PRINT_LOG(INFO, Info, fmt, ##__VA_ARGS__)
#define OPENCV_LOGW(fmt, ...) PRINT_LOG(WARN, Warn, fmt, ##__VA_ARGS__)
#define OPENCV_LOGE(fmt, ...) PRINT_LOG(ERROR, Error, fmt, ##__VA_ARGS__)
#define OPENCV_LOGF(fmt, ...) PRINT_LOG(FATAL, Fatal, fmt, ##__VA_ARGS__)
}  // namespace OpenCV
}  // namespace OHOS
#endif  // OPENCV_NAPI_LOG_WRAPPER_H