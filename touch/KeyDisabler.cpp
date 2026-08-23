/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "KeyDisablerService"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include "KeyDisabler.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

::ndk::ScopedAStatus KeyDisabler::getEnabled(bool* _aidl_return) {
    std::string buf;

    if (!android::base::ReadFileToString(kControlPath, &buf)) {
        LOG(ERROR) << "Failed to read " << kControlPath;
        *_aidl_return = false;
        return ::ndk::ScopedAStatus::ok();
    }

    *_aidl_return = std::stoi(android::base::Trim(buf)) == 1;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus KeyDisabler::setEnabled(bool enabled) {
    if (!android::base::WriteStringToFile((enabled ? "1" : "0"), kControlPath)) {
        LOG(ERROR) << "Failed to write " << kControlPath;
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ::ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
