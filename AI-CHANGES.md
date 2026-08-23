# AI-Assisted Changes (lineage-22.1-ai)

All commits carry an `Assisted-by: ox-alpha (opencode)` trailer. Branches:

| Repo path | Branch | Base |
|---|---|---|
| device/oneplus/oneplus2 | `lineage-22.1-ai` | lineage-22.1 |
| hardware/qcom-caf/msm8994/display | `display-ai` | display-new |
| hardware/qcom-caf/msm8994/audio | `audio-ai` | audio |
| hardware/qcom-caf/msm8994/media | `media-ai` | media |

Local manifest: `.repo/local_manifests/oneplus2.xml` pins these branches.

**Build**: userdebug, release config **bp4a**
(`lunch lineage_oneplus2-bp4a-userdebug`). Helper script: `/build-oneplus2.sh`
in the LOS root (`build`, `logs`, `clean`; saves failures to
`build-logs/last-error.log`). Verified: full build + otapackage complete on
this tree.

## device/oneplus/oneplus2 -- lineage-22.1-ai

1. **Point boot image profile at 23.2 location** - lineage-23.2 moved the
   combined boot-image profile from frameworks/base/config to .../boot.
2. **Drop stale display module packages** - `.msm8994` display names and
   liboverlay no longer exist; ship `copybit.qcom`, `gralloc.qcom`,
   `hwcomposer.qcom` (ro.hardware=qcom) instead.
3. **Migrate touch HAL from HIDL to AIDL** - vendor.lineage.touch@1.0 was
   dropped upstream; KeyDisabler (/proc/s1302/virtual_key), KeySwapper
   (/proc/s1302/key_rep) and TouchscreenGesture (/proc/touchpanel/*) now use
   vendor.lineage.touch-V1-ndk. VINTF declaration lives in a
   vintf_fragments xml shipped by the service; kernel nodes unchanged.
4. **Drop stale lineage device FCM path** - lineage ships its framework
   compatibility matrix as a soong module now.
5. **Rename libaudioclient_shim** - hardware/lineage/compat defines the same
   module name (AudioTrack shims only); ours provides the AudioSystem
   error-callback shim used by RIL, renamed to libaudioclient_shim.oneplus2.
6. **wifi AIDL service, AIDL LiveDisplay, bt namespace** -
   android.hardware.wifi-service, vendor.lineage.livedisplay-service.sysfs,
   PRODUCT_SOONG_NAMESPACES for qcom-caf/bt.
7. **Declare empty soter attributes** - lineage's unconditional
   hal_client_domain(platform_app, vendor_hal_soter) needs the attributes to
   exist; msm8994 has no soter HAL.
8. **Vendor libbt-vendor build** - upstream repo wraps itself in a soong
   namespace and no longer compiles under this tree's warnings; carry a
   device-local copy with -Wno-bool-conversion instead of touching it.
9. **Media extensions + VANILLA_HAL camera** - USE_MEDIA_EXTENSIONS gates
   native-handle metadata paths (was set globally before). The OSS camera HAL
   was kanged without its full mm-camera header stack: longshot / frame-fd
   extensions reference constants whose framework plumbing (the lineage
   "eleven-camera-extension" topic) only ever landed on lineage-18.x and was
   never forward-ported, so VANILLA_HAL compiles those dead references out.
10. **VINTF fixes** - drop duplicate touch entry (service ships fragments);
    drop invented ILiveDisplayService entry (the AIDL service ships
    per-feature fragments); declare legacy memtrack@1.0 and power.stats@1.0
    in the device framework compatibility matrix.

## hardware/qcom-caf/msm8994/display -- display-ai

Rebuilt from the pre-April state after an audit showed the 2025-04 batch
contained misattributed deletions and a broken blueprint conversion.
Lineage picks restored with true authorship (cherry-pick -x), botched
conversions redone, plus msm8994-specific fixes:
- libmemtrack removal, libcopybit source removal, fps-calc debug removal
  (true upstream authors via -x)
- proper mk->bp conversion; legacy gralloc/overlay/hdmi/hwcomposer kept and
  converted; modules named .qcom
- C2D2 copybit restored as copybit.qcom with local copybit.h/c2d2.h (UBWC-era)
- display_intf_headers provided; libui/libhardware/kernel-header wiring;
  relaxed warning regime for the HWC1-era sources; CALC_INIT/CALC_FPS hooks
  dropped

## hardware/qcom-caf/msm8994/audio -- audio-ai

- AudioEffect reply overflow fix (Lakshman Chaluvaraju).
- sound_trigger_prop_intf.h pulled from the device include dir.
- Deliberately NOT taken: the msm8996 "U compile" fix (downgrades ACDB to
  v2; msm8994 blobs need v3).

## hardware/qcom-caf/msm8994/media -- media-ai

- Drop legacy mm-core makefile (duplicate libOmxCore).
- Fix libOmxVidcCommon/vdec/venc headers (qti_kernel_headers,
  libomxcommon_headers defined, libutils_headers, local include dirs) and
  restore vdec ts_parser.h/hevc_utils.h era headers.

## Workspace notes (not repo changes)

- packages/modules/UprobeStats: cherry-picked community fix
  (min_sdk_version 36 -> 35); upstream staging merge breaks 32-bit secondary
  arch analysis until LOS aligns it. A repo sync reverts this patch.
- system/sepolicy Android.bp patched locally so selinux_ignore_neverallows
  alone skips the Treble sepolicy tests (upstream tuple requires ASAN too).
  OP2 is pre-Treble and can never satisfy them as-is. Reverted by repo sync.
- Release config must be bp4a; ap4a predates SDK-36 awareness and fails aidl
  ndk variant analysis everywhere.

## Known follow-ups

- Forward-port the eleven-camera-extension topic to 23.2 if longshot /
  frame-fd camera features are wanted (frameworks/base + av work).
- Doze app modernization, overlay->RRO conversion, oppo/common retirement:
  optional cleanups discussed but deliberately not attempted here.
