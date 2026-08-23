/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/touch/BnTouchscreenGesture.h>
#include <map>

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

class TouchscreenGesture : public BnTouchscreenGesture {
  public:
    ::ndk::ScopedAStatus getSupportedGestures(
            std::vector<::aidl::vendor::lineage::touch::Gesture>* _aidl_return) override;
    ::ndk::ScopedAStatus setGestureEnabled(
            const ::aidl::vendor::lineage::touch::Gesture& gesture, bool enabled) override;

  private:
    typedef struct {
        int32_t keycode;
        const char* name;
        const char* path;
    } GestureInfo;
    static const std::map<int32_t, GestureInfo> kGestureInfoMap;  // id -> info
};

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
