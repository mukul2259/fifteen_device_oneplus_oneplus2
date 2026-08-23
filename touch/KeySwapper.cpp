/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "KeySwapperService"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include "KeySwapper.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

::ndk::ScopedAStatus KeySwapper::getEnabled(bool* _aidl_return) {
    *_aidl_return = false;
    if (!has_key_swapper_) return ::ndk::ScopedAStatus::ok();

    std::string buf;
    if (!android::base::ReadFileToString(kControlPath, &buf)) {
        LOG(ERROR) << "Failed to read " << kControlPath;
        return ::ndk::ScopedAStatus::ok();
    }

    *_aidl_return = std::stoi(android::base::Trim(buf)) == 1;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus KeySwapper::setEnabled(bool enabled) {
    if (!has_key_swapper_) {
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (!android::base::WriteStringToFile(std::to_string(enabled), kControlPath)) {
        LOG(ERROR) << "Failed to write " << kControlPath;
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ::ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
