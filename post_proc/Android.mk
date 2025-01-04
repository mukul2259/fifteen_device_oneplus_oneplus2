
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

ifneq ($(strip $(AUDIO_FEATURE_ENABLED_PROXY_DEVICE)),false)
    LOCAL_CFLAGS += -DAFE_PROXY_ENABLED
endif

LOCAL_SRC_FILES:= \
	bundle.c \
	equalizer.c \
	bass_boost.c \
	virtualizer.c \
	reverb.c \
	effect_api.c \
	effect_util.c

LOCAL_CFLAGS+= -O2 -fvisibility=hidden
LOCAL_CFLAGS += -Wno-sign-compare -Wno-unused-variable -Wno-unused-label -Wno-format

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	liblog \
	libtinyalsa

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE_RELATIVE_PATH := soundfx
LOCAL_MODULE:= libqcompostprocbundle
LOCAL_VENDOR_MODULE := true

LOCAL_C_INCLUDES := \
	external/tinyalsa/include \
        hardware/libhardware/include \
	$(call include-path-for, audio-effects)

LOCAL_HEADER_LIBRARIES := generated_kernel_headers libsystem_headers libaudio_system_headers

include $(BUILD_SHARED_LIBRARY)
