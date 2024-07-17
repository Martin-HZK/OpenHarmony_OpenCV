#include "n_objdetect.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/dnn.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv_log_wrapper.h"

using namespace OHOS::Opencv;
using namespace cv;
using namespace cv::dnn;
using namespace std;

namespace OHOS {
namespace OpencvNapi {
struct ParametersDetectMultiScale {
    std::string haarcascadeFileName = "";
    MatInfo mat;
    napi_ref callback = nullptr;
};
struct AsyncCallbackInfoDetectMultiScale {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    ParametersDetectMultiScale params;
    CallbackPromiseInfo info;
    std::vector<cv::Rect> v_outRect;
};

const size_t inWidth = 300;
const size_t inHeight = 300;
const double inScaleFactor = 1.0;
const float confidenceThreshold = 0.7;
const cv::Scalar meanVal(104.0, 177.0, 123.0);

void detectFaceOpenCVDNN(Net net, Mat &frameOpenCVDNN, string framework)
{
    int frameHeight = frameOpenCVDNN.rows;
    int frameWidth = frameOpenCVDNN.cols;

    cv::Mat inputBlob;
    if (framework == "caffe")
        inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, false, false);
    else
        inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, true, false);

    net.setInput(inputBlob, "data");
    cv::Mat detection = net.forward("detection_out");

    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) {
        float confidence = detectionMat.at<float>(i, 2);
        if (confidence > confidenceThreshold) {
            int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
            int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
            int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
            int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);

            cv::rectangle(frameOpenCVDNN, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2, 4);
        }
    }
}

napi_value ParseParametersDetectMultiScale(const napi_env &env, const napi_callback_info &info, ParametersDetectMultiScale &paras)
{
    OPENCV_LOGI("enter ParseParametersDetectMultiScale");

    size_t argc = 3;
    napi_value argv[3] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM3) {
        OPENCV_LOGE("Wrong number of arguments");
        return nullptr;
    }
    
    NAPI_CALL(env, napi_typeof(env, argv[PARAM0], &valuetype));
    if (valuetype != napi_string) {
        OPENCV_LOGE("Wrong argument type. String expected.\n");
        return nullptr;
    }
    char str[STR_MAX_SIZE] = {0};
    size_t strLen = 0;
    NAPI_CALL(env, napi_get_value_string_utf8(env, argv[PARAM0], str, STR_MAX_SIZE - 1, &strLen));
    paras.haarcascadeFileName = str;

    Common::GetMatInfo(env, argv[PARAM1], paras.mat);

    if (argc >= PARAM3) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM2], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM2], 1, &paras.callback);
    }

    return Common::NapiGetNull(env);
}

void AsyncCompleteCallbackDetectMultiScale(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("enter AsyncCompleteCallbackDetectMultiScale");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data\n");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoDetectMultiScale *>(data);
    napi_value result = Common::NapiGetNull(env);

    asynccallbackinfo->info.errorCode = ERR_OK;
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
            napi_value arr = nullptr;
            napi_create_array(env, &arr);
            size_t count = 0;
            for (auto vec : asynccallbackinfo->v_outRect) {
                if (vec.empty()) {
                    OPENCV_LOGE("Invalid v_outRect object ptr");
                    continue;
                }
                napi_value nRect = nullptr;
                napi_create_object(env, &nRect);
                if (!Common::SetRect(env, vec, nRect)) {
                    continue;
                }
                napi_set_element(env, arr, count, nRect);
                count++;
            }
            OPENCV_LOGI("getRects count = %{public}zu", count);
            result = arr;
            if ((count == 0) && (asynccallbackinfo->v_outRect.size() > 0)) {
                asynccallbackinfo->info.errorCode = ERROR;
                result = Common::NapiGetNull(env);
            }
    }
    Common::ReturnCallbackPromise(env, asynccallbackinfo->info, result);
    if (asynccallbackinfo->info.callback != nullptr) {
        napi_delete_reference(env, asynccallbackinfo->info.callback);
    }
    napi_delete_async_work(env, asynccallbackinfo->asyncWork);
    delete asynccallbackinfo;
    asynccallbackinfo = nullptr;
}

napi_value n_detectMultiScale(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_detectMultiScale");
    ParametersDetectMultiScale params;
    if (ParseParametersDetectMultiScale(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfoDetectMultiScale *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfoDetectMultiScale {.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }

    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback, asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "detectMultiScale", NAPI_AUTO_LENGTH, &resourceName);
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            AsyncCallbackInfoDetectMultiScale *asynccallbackinfo = static_cast<AsyncCallbackInfoDetectMultiScale *>(data);
            if ( asynccallbackinfo ) {  
                cv::Mat inMat(asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols, asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data);
                cv::CascadeClassifier face_cascade;
                if (!face_cascade.load(asynccallbackinfo->params.haarcascadeFileName)) {
                    OPENCV_LOGE("Load haarcascade_frontalface_alt failed!");
                }
                face_cascade.detectMultiScale(inMat, asynccallbackinfo->v_outRect);

                OPENCV_LOGI("n_detectMultiScale outRect size = %{public}d", asynccallbackinfo->v_outRect.size());
            }
        },
        AsyncCompleteCallbackDetectMultiScale,
        (void*)asynccallbackinfo,
        &(asynccallbackinfo->asyncWork));

    NAPI_CALL(env, napi_queue_async_work(env, asynccallbackinfo->asyncWork));

    if (asynccallbackinfo->info.isCallback) {
        return Common::NapiGetNull(env);
    } else {
        return promise;
    }
}
}  //OpencvNapi
}  //OHOS