/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/touch/BnKeySwapper.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

class KeySwapper : public BnKeySwapper {
  public:
    KeySwapper() : has_key_swapper_(!access(kControlPath, R_OK | W_OK)) {}
    ::ndk::ScopedAStatus getEnabled(bool* _aidl_return) override;
    ::ndk::ScopedAStatus setEnabled(bool enabled) override;

  private:
    constexpr static const char kControlPath[] = "/proc/s1302/key_rep";
    bool has_key_swapper_;
};

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
