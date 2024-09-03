# Integrate OpenCV to Open Harmony4.1_release

 
Pre-requisite：rk3568, OpenHarmony4.1 standard system，Compile on Ubuntu20.04 LTS

**Current configuration _<u>only support OH in 32 bit!!!</u>_**
1. Download OpenCV Library

Link：[OpenHarmony-SIG/third_party_opencv](https://gitee.com/openharmony-sig/third_party_opencv)

2.  Add the library to the third_party directory

Put opencv under the `third_party` directory. Keep cautious about the directory name!

Modify `vendor/hihope/RK3568/config.json`. Add new component, opncv, under thirdparty subsystem and its corresponding information.
``` json
{
      "subsystem": "thirdparty",
      "components": [
        {
          "component": "libuv",
          "features": [
            "use_ffrt = true"
          ]
        },
        {
          "component": "wpa_supplicant",
          "features": [
            "wpa_supplicant_driver_nl80211 = true"
          ]
        },
        {
          "component": "opencv",
          "features":[]
        }
      ]
    },
```

3. Compile the code

Compile tutorial: [Tutorial](https://docs.openharmony.cn/pages/v4.0/zh-cn/device-dev/device-dev-guide.md)

4. Debugging

Bug1

Exception: //third_party/opencv/3rdparty/ffmpeg/libavcodec:opencv_ffmpeg_avcodec depend part //third_party/opencv/3rdparty/ffmpeg/libavutil:libopencv_avutil, need set part deps opencv info to /home/hzk/Documents/OH4.1/OpenHarmony-v4.1-Release/OpenHarmony/build/bundle.json.

Solution: modify the build/bundle.json
```json
 "deps": {
      "components": [],
        "third_party": [
        "musl",
        "markupsafe",
        "jinja2",
        "e2fsprogs",
        "f2fs-tools",
        "opencv"
      ]
    },
    "build": {
      "sub_component": [
        "//build/common:common_packages",
        "//build/rust:default",
        "//third_party/f2fs-tools:f2fs-tools_host_toolchain",
        "//third_party/opencv/3rdparty/ffmpeg/libavutil:libopencv_avutil",
        "//third_party/opencv/3rdparty/ffmpeg/libswresample:libopencv_swresample"
      ],
    }
```
Bug2

[OHOS ERROR] Exception: //third_party/opencv/napi:opencv_napi depend part //third_party/bounds_checking_function:libsec_static, need set part deps bounds_checking_function info to /home/hzk/Documents/OH4.1/OpenHarmony-v4.1-Release/OpenHarmony/third_party/opencv/bundle.json.

Solution：add `bounds_checking_function`and `libuv` to 'opencv/bundle.json', clear the derivative path in 'third_party' section
 
```json
"deps": {
        "components": [
            "ability_base",
            "ability_runtime",
            "access_token",
            "bundle_framework",
            "c_utils",
            "certificate_manager",
            "common_event_service",
            "hisysevent",
            "hilog",
            "huks",
            "ipc",
            "napi",
            "netmanager_base",
            "safwk",
            "samgr",
            "data_share",
            "hdf_core",
            "hicollie",
            "relational_store",
            "bounds_checking_function",
            "libuv"
        ],
        "third_party": [
            "bounds_checking_function",
            "libuv"
        ]
      },
```
---

For details and mandarian tutorial, please refer to my blog on [csdn](https://blog.csdn.net/weixin_64726009/article/details/140493002?spm=1001.2014.3001.5502).
