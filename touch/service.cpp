/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.oneplus2"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include "KeyDisabler.h"
#include "KeySwapper.h"
#include "TouchscreenGesture.h"

using ::aidl::vendor::lineage::touch::KeyDisabler;
using ::aidl::vendor::lineage::touch::KeySwapper;
using ::aidl::vendor::lineage::touch::TouchscreenGesture;

int main() {
    binder_status_t status = STATUS_OK;

    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<KeyDisabler> kd = ndk::SharedRefBase::make<KeyDisabler>();
    const std::string instanceKd = std::string(KeyDisabler::descriptor) + "/default";
    status = AServiceManager_addService(kd->asBinder().get(), instanceKd.c_str());
    CHECK_EQ(status, STATUS_OK) << "Failed to add service " << instanceKd << " " << status;

    std::shared_ptr<KeySwapper> ks = ndk::SharedRefBase::make<KeySwapper>();
    const std::string instanceKs = std::string(KeySwapper::descriptor) + "/default";
    status = AServiceManager_addService(ks->asBinder().get(), instanceKs.c_str());
    CHECK_EQ(status, STATUS_OK) << "Failed to add service " << instanceKs << " " << status;

    std::shared_ptr<TouchscreenGesture> tg = ndk::SharedRefBase::make<TouchscreenGesture>();
    const std::string instanceTg = std::string(TouchscreenGesture::descriptor) + "/default";
    status = AServiceManager_addService(tg->asBinder().get(), instanceTg.c_str());
    CHECK_EQ(status, STATUS_OK) << "Failed to add service " << instanceTg << " " << status;

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
