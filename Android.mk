ifeq ($(call my-dir),$(call project-path-for,qcom-display))

TARGET_USES_SDE = false

display-hals := libgralloc libmemtrack libqservice libqdutils \
		libgenlock libhwcomposer libhdmi

ifeq ($(call is-vendor-board-platform,QCOM),true)
    include $(call all-named-subdir-makefiles,$(display-hals))
else
ifneq ($(filter msm% apq%,$(TARGET_BOARD_PLATFORM)),)
    include $(call all-named-subdir-makefiles,$(display-hals))
endif
endif

include $(CLEAR_VARS)
LOCAL_MODULE := display_headers
LOCAL_EXPORT_C_INCLUDE_DIRS := \
    $(display_top)/libexternal \
    $(display_top)/libhdmi \
    $(display_top)/libhwcomposer \
    $(display_top)/libqdutils \
    $(display_top)/libqservice \
    $(display_top)/libvirtual
include $(BUILD_HEADER_LIBRARY)

endif
