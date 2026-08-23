# AI-Assisted Changes (lineage-22.1-ai)

All changes below were made with AI assistance (`Assisted-by: ox-alpha (opencode)`
trailer on every commit). Branches:

| Repo path | Branch | Base |
|---|---|---|
| device/oneplus/oneplus2 | `lineage-22.1-ai` | lineage-22.1 |
| hardware/qcom-caf/msm8994/display | `display-ai` | display-new |
| hardware/qcom-caf/msm8994/audio | `audio-ai` | audio |
| hardware/qcom-caf/msm8994/media | `media-ai` | media |

Local manifest: `.repo/local_manifests/oneplus2.xml` pins these branches.
Release config for building: **`bp4a`** (`lunch lineage_oneplus2-bp4a-userdebug`).
Build helper: `/build-oneplus2.sh` in the LOS root.

## device/oneplus/oneplus2 -- lineage-22.1-ai

1. **Point boot image profile at 23.2 location** - lineage-23.2 moved
   `frameworks/base/config/boot-image-profile.txt` to `frameworks/base/boot/`.
2. **Drop stale display module packages** - copybit/gralloc/hwcomposer/memtrack
   `.msm8994` names and liboverlay no longer exist; add the real modules
   (`copybit.qcom`, `gralloc.qcom`, `hwcomposer.qcom`; ro.hardware=qcom).
3. **Migrate touch HAL from HIDL to AIDL** - vendor.lineage.touch@1.0 HIDL was
   dropped upstream; KeyDisabler (/proc/s1302/virtual_key), KeySwapper
   (/proc/s1302/key_rep) and TouchscreenGesture (/proc/touchpanel/*) now use
   `vendor.lineage.touch-V1-ndk`. VINTF declaration moved into a
   vintf_fragments xml shipped by the service; kernel nodes unchanged.
4. **Drop stale lineage device FCM path** - vendor/lineage moved
   device_framework_matrix.xml into hardware/lineage/interfaces as a soong
   module wired globally; referencing the removed xml broke the build.
5. **Rename libaudioclient_shim to avoid clash** -
   hardware/lineage/compat now defines `libaudioclient_shim` (AudioTrack
   shims); ours provides the AudioSystem error-callback shim used by RIL, so
   it is now `libaudioclient_shim.oneplus2`.
6. **23.2 compat: wifi AIDL service, AIDL LiveDisplay, bt namespace**
   - `android.hardware.wifi@1.0-service` -> `android.hardware.wifi-service`
   - HIDL LiveDisplay services -> `vendor.lineage.livedisplay-service.sysfs`
     (+ AIDL hal entry in manifest.xml)
   - `hardware/qcom-caf/bt/libbt-vendor` added to PRODUCT_SOONG_NAMESPACES so
     libbt-vendor resolves (the repo declares its own soong namespace).

## hardware/qcom-caf/msm8994/display -- display-ai

Rebuilt from the pre-April state after an audit showed the 2025-04 batch
contained misattributed deletions and a broken blueprint conversion:
lineage picks restored with true authorship (`cherry-pick -x`), botched
conversions redone, plus msm8994-specific fixes. Highlights:
- libmemtrack / libcopybit-sources removals (Zube Molokwu / Michael Bestas)
- proper mk->bp conversion (Naseer Ahmed + adaptation); legacy gralloc,
  overlay, hdmi, hwcomposer kept and converted; modules named `.qcom`
- fps-calculation debug code removal (Saurabh Shah)
- legacy C2D2 copybit restored as `copybit.qcom` (TARGET_USES_C2D_COMPOSITION)
- provides `display_intf_headers` for surfaceflinger; links plain `libui`

## hardware/qcom-caf/msm8994/audio -- audio-ai

- AudioEffect reply overflow fix (Lakshman Chaluvaraju).
- Audited against LineageOS unified msm8996: most candidate fixes were
  already incorporated; ACDB "U compile" fix deliberately NOT taken
  (targets acdb v2; msm8994 blobs need v3).

## hardware/qcom-caf/msm8994/media -- media-ai

- Drop legacy mm-core makefile (duplicate libOmxCore vs Android.bp).
- Fix libOmxVidcCommon headers: general_kernel_headers ->
  qti_kernel_headers; define libomxcommon_headers.

## Workspace notes (not repo changes)

- packages/modules/UprobeStats: cherry-picked community fix
  (min_sdk_version 36 -> 35, Inventor365) locally; upstream staging merge is
  broken for 32-bit secondary arch products until LOS aligns it. Re-sync may
  revert this patch.
- Correct release config is bp4a; ap4a predates SDK-36 awareness and fails
  aidl ndk variant analysis.
