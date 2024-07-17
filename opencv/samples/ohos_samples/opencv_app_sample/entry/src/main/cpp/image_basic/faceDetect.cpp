/*
 * Copyright (c) 2024 Shenzhen Kaihong Digital Industry Development Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// #include <aki/jsbind.h>
#include <iostream>
#include <fstream>
#include "common.h"

using namespace cv;
static const char *TAG = "[opencv_faceDetect]";
CascadeClassifier faceCascade;

bool GetModelFromRawFile(napi_env env, napi_callback_info info,std::string fileName) {
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "GetModelFromRawFile Begin");
    
    size_t argc = 3;
    napi_value argv[3] = {nullptr};

    napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    napi_valuetype valueType;
    napi_typeof(env, argv[0], &valueType);

    NativeResourceManager *mNativeResMgr = OH_ResourceManager_InitNativeResourceManager(env, argv[0]);
    if (mNativeResMgr == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "Init native resource manager failed!.");
        return false;
    }
    RawDir *rawDir = OH_ResourceManager_OpenRawDir(mNativeResMgr, "");
    if (rawDir == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "Open raw file dir failed!.");
        OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);
        return false;
    }
    int count = OH_ResourceManager_GetRawFileCount(rawDir);
    
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "fileName：%{public}s", fileName.c_str());

    bool isFileExist = false;
    for (int i = 0; i < count; i++) {
        std::string name = OH_ResourceManager_GetRawFileName(rawDir, i);
        OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "name：%{public}s", name.c_str());
        if (name == fileName) {
            isFileExist = true;
            break;
        }
    }
    if (!isFileExist) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "Raw file directory not exist file: %{public}s.",
                     fileName.c_str());
        OH_ResourceManager_CloseRawDir(rawDir);
        OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);
        return false;
    }
    RawFile *rawFile = OH_ResourceManager_OpenRawFile(mNativeResMgr, fileName.c_str());
    if (rawFile == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "Open Raw file failed, file name: %{public}s.",
                     fileName.c_str());
        OH_ResourceManager_CloseRawDir(rawDir);
        OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);
        return false;
    }

    long rawFileSize = OH_ResourceManager_GetRawFileSize(rawFile);
    unsigned char *data = new unsigned char[rawFileSize];
    long rawFileOffset = OH_ResourceManager_ReadRawFile(rawFile, data, rawFileSize);
    if (rawFileOffset != rawFileSize) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG,
                     "Read rawfile size[%{public}ld] not equal to actual file size[%{public}ld]", rawFileOffset,
                     rawFileSize);
        delete[] data;
        OH_ResourceManager_CloseRawFile(rawFile);
        OH_ResourceManager_CloseRawDir(rawDir);
        OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);
        return false;
    }
    
    std::ofstream file("/data/storage/el2/base/haps/entry/files/haarcascade_frontalface_alt.xml");
    if (!file) {
        OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "failed to create file");
        return false;
    }
    file << data;
    file.close();

    delete[] data;
    OH_ResourceManager_CloseRawFile(rawFile);
    OH_ResourceManager_CloseRawDir(rawDir);
    OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);

    return true;
}

napi_value FaceDetect(napi_env env, napi_callback_info info) {
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "FaceDetect Begin");
    napi_value result = NapiGetNull(env);
    size_t argc = 3;
    napi_value argv[3] = {nullptr};

    napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

    size_t strSize;
    char strBuf[256];
    napi_get_value_string_utf8(env, argv[1], strBuf, sizeof(strBuf), &strSize);
    std::string fileDir(strBuf, strSize);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "fileDir：%{public}s", fileDir.c_str());

    napi_get_value_string_utf8(env, argv[2], strBuf, sizeof(strBuf), &strSize);
    std::string fileName(strBuf, strSize);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "fileName：%{public}s", fileName.c_str());
    
    Mat srcImage;
    if (!GetMatFromRawFile(env, argv[0], fileDir, fileName, srcImage)) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "Get Mat from rawfile failed!.");
        return result;
    }

    if (!GetModelFromRawFile(env, info, "haarcascade_frontalface_alt.xml")) {
        return result;
    }

    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG,
                 "srcImage total：%{public}d, cols: %{public}d, rows: %{public}d, channel: %{public}d",
                 srcImage.total(), srcImage.cols, srcImage.rows, srcImage.channels());

    Mat grayImage;
    cvtColor(srcImage, grayImage, COLOR_RGB2GRAY);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "cvtColor end");
    
    bool bLoad = faceCascade.load("/data/storage/el2/base/haps/entry/files/haarcascade_frontalface_alt.xml"); // 加载分类器，注意文件路径
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "faceCascade detectMultiScale begin");
    std::vector<Rect> faces;
    faceCascade.detectMultiScale(grayImage, faces, 1.2, 6, 0, Size(0, 0)); // 检测人脸
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "faceCascade detectMultiScale end");

    if (faces.size() > 0) {
        OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "face size：%{public}d, ", faces.size());
        for (int i = 0; i < faces.size(); i++) {
            rectangle(srcImage, Point(faces[i].x, faces[i].y),
            Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 255, 0), 4,8); // 框出人脸位置
        }
    }
    
    // 將图像转换为pixelMap格式
    Mat outMat;
    cvtMat2Pixel(srcImage, outMat, RGBA_8888);
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "outMat size: %{public}d, cols：%{public}d, rows：%{public}d",
                 outMat.total(), outMat.cols, outMat.rows);

    napi_create_object(env, &result);
    bool retVal = WrapJsPixelInfoInfo(env, outMat, result);
    if (!retVal) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, TAG, "WrapJsInfo failed!.");
    }

    return result;
}
