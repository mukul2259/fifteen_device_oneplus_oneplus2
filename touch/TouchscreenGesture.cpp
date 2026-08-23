/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "TouchscreenGestureService"

#include <android-base/file.h>
#include <android-base/logging.h>

#include "TouchscreenGesture.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

const std::map<int32_t, TouchscreenGesture::GestureInfo> TouchscreenGesture::kGestureInfoMap = {
	{0, {250, "down arrow", "/proc/touchpanel/draw_v"}},
	{1, {251, "up arrow", "/proc/touchpanel/draw_reversed_v"}},
	{2, {252, "right arrow", "/proc/touchpanel/draw_right_v"}},
	{3, {253, "left arrow", "/proc/touchpanel/draw_left_v"}},
        {4, {254, "letter o", "/proc/touchpanel/draw_circle"}},
        {5, {255, "two finger down swipe", "/proc/touchpanel/double_swipe"}},
        {6, {256, "one finger right swipe", "/proc/touchpanel/right_swipe"}},
        {7, {257, "one finger left swipe", "/proc/touchpanel/left_swipe"}},
        {8, {258, "one finger down swipe", "/proc/touchpanel/down_swipe"}},
        {9, {259, "one finger up swipe", "/proc/touchpanel/up_swipe"}},
};

::ndk::ScopedAStatus TouchscreenGesture::getSupportedGestures(
        std::vector<Gesture>* _aidl_return) {
    for (const auto& entry : kGestureInfoMap) {
        Gesture gesture;
        gesture.id = entry.first;
        gesture.name = entry.second.name;
        gesture.keycode = entry.second.keycode;
        _aidl_return->push_back(gesture);
    }

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TouchscreenGesture::setGestureEnabled(const Gesture& gesture,
                                                          bool enabled) {
    const auto entry = kGestureInfoMap.find(gesture.id);
    if (entry == kGestureInfoMap.end()) {
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (!android::base::WriteStringToFile(std::to_string(enabled), entry->second.path)) {
        LOG(ERROR) << "Failed to write " << entry->second.path;
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ::ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
