#include "n_imgproc.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv_log_wrapper.h"

using namespace OHOS::Opencv;

namespace OHOS {
namespace OpencvNapi {

struct ParamsRectangle {
    MatInfo mat;
    ParamsScalar scalar;
    std::vector<ParamsRect> v_rects;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfoRectangle {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    ParamsRectangle params;
    CallbackPromiseInfo info;
    MatInfo outMat;
};

napi_value ParseParametersRectangle(const napi_env &env, const napi_callback_info &info, ParamsRectangle &paras)
{
    OPENCV_LOGI("enter ParseParametersRectangle");

    size_t argc = 4;
    napi_value argv[4] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM3) {
        OPENCV_LOGE("Wrong number of arguments\n");
        return nullptr;
    }
    
    Common::GetMatInfo(env, argv[PARAM0], paras.mat);
    Common::GetRects(env, argv[PARAM1], paras.v_rects);
    Common::GetScalar(env, argv[PARAM2], paras.scalar);
    
    if (argc >= PARAM4) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM3], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM3], 1, &paras.callback);
    }
    return Common::NapiGetNull(env);
}

void AsyncCompleteCallbackRectangle(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("enter AsyncCompleteCallbackRectangle");
    if (!data) {
        OPENCV_LOGE("Invalid async callback data\n");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoRectangle *>(data);
    napi_value result = Common::NapiGetNull(env);

    asynccallbackinfo->info.errorCode = ERR_OK;
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
        if (asynccallbackinfo->outMat.data == nullptr ) {
            asynccallbackinfo->info.errorCode = ERROR;
        } else {
            napi_create_object(env, &result);
            if (!Common::SetMat(env, asynccallbackinfo->outMat.rows, asynccallbackinfo->outMat.cols, asynccallbackinfo->outMat.type, asynccallbackinfo->outMat.bufferSize, asynccallbackinfo->outMat.data, result)) {
                asynccallbackinfo->info.errorCode = ERROR;
                result = Common::NapiGetNull(env);
            }
        }
    }
    Common::ReturnCallbackPromise(env, asynccallbackinfo->info, result);
    if (asynccallbackinfo->info.callback != nullptr) {
        napi_delete_reference(env, asynccallbackinfo->info.callback);
    }
    napi_delete_async_work(env, asynccallbackinfo->asyncWork);
    FreeMemoryArray<char>((char*)asynccallbackinfo->outMat.data);
    FreeMemoryArray<char>((char*)(asynccallbackinfo->params.mat.data));
    FreeMemory<AsyncCallbackInfoRectangle>(asynccallbackinfo);
}

napi_value n_rectangle(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_rectangle");
    ParamsRectangle params;
    if (ParseParametersRectangle(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfoRectangle *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfoRectangle {.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }

    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback, asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "rectangle", NAPI_AUTO_LENGTH, &resourceName);
    // Asynchronous function call
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            AsyncCallbackInfoRectangle *asynccallbackinfo = static_cast<AsyncCallbackInfoRectangle *>(data);
            if ( asynccallbackinfo ) {       
                cv::Mat inOutMat(asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols, asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data);
                cv::Rect rect;
                cv::Scalar scalar = cv::Scalar(asynccallbackinfo->params.scalar.scalar_x, asynccallbackinfo->params.scalar.scalar_y, asynccallbackinfo->params.scalar.scalar_z);
                for (size_t i = 0; i < asynccallbackinfo->params.v_rects.size(); i++) {
                    rect = cv::Rect(asynccallbackinfo->params.v_rects[i].x, asynccallbackinfo->params.v_rects[i].y, asynccallbackinfo->params.v_rects[i].width,
                        asynccallbackinfo->params.v_rects[i].height);
                    cv::rectangle(inOutMat, rect, scalar);
                }
                if (!Common::Copy2NapiMat(inOutMat, asynccallbackinfo->outMat)) {
                    OPENCV_LOGE("Copy2NapiMat failed.");
                }
            }
        },
        AsyncCompleteCallbackRectangle,
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