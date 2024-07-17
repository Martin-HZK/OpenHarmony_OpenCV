#include "n_imgproc.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv_log_wrapper.h"

using namespace OHOS::Opencv;

namespace OHOS {
namespace OpencvNapi {

struct ParametersEqualizeHist {
    MatInfo mat;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfoEqualizeHist {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    ParametersEqualizeHist params;
    CallbackPromiseInfo info;
    MatInfo outMat;
};

napi_value ParseParametersEqualizeHist(const napi_env &env, const napi_callback_info &info, ParametersEqualizeHist &paras)
{
    OPENCV_LOGI("enter ParseParametersEqualizeHist\n");

    size_t argc = 2;
    napi_value argv[2] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM1) {
        OPENCV_LOGE("Wrong number of arguments\n");
        return nullptr;
    }

    Common::GetMatInfo( env, argv[PARAM0], paras.mat );
    if (argc >= PARAM2) {
        NAPI_CALL(env, napi_typeof(env, argv[PARAM1], &valuetype));
        if (valuetype != napi_function) {
            OPENCV_LOGE("Wrong argument type. Function expected.");
            return nullptr;
        }
        napi_create_reference(env, argv[PARAM1], 1, &paras.callback);
    }
    return Common::NapiGetNull(env);
}

void AsyncCompleteCallbackEqualizeHist(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGI("enter AsyncCompleteCallbackEqualizeHist\n");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data\n");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoEqualizeHist *>(data);
    napi_value result = Common::NapiGetNull(env);

    asynccallbackinfo->info.errorCode = ERR_OK;
    if (asynccallbackinfo->info.errorCode == ERR_OK) {
        if ( asynccallbackinfo->outMat.data == nullptr ) {
            asynccallbackinfo->info.errorCode = ERROR;
        } else {
            napi_create_object(env, &result);
            if ( !Common::SetMat(env, asynccallbackinfo->outMat.rows, asynccallbackinfo->outMat.cols, asynccallbackinfo->outMat.type, asynccallbackinfo->outMat.bufferSize, asynccallbackinfo->outMat.data, result) ) {
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
    FreeMemory<AsyncCallbackInfoEqualizeHist>(asynccallbackinfo);
}

napi_value n_equalizeHist(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_equalizeHist");
    ParametersEqualizeHist params;
    if (ParseParametersEqualizeHist(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfoEqualizeHist *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfoEqualizeHist {.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }

    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback, asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "equalizeHist", NAPI_AUTO_LENGTH, &resourceName);
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            OPENCV_LOGI("n_equalizeHist napi_create_async_work start");
            AsyncCallbackInfoEqualizeHist *asynccallbackinfo = static_cast<AsyncCallbackInfoEqualizeHist *>(data);
            if (asynccallbackinfo) {
                cv::Mat inMat( asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols,
                               asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data );
                cv::Mat outMat;
                cv::equalizeHist(inMat, outMat);

                if (!Common::Copy2NapiMat(outMat, asynccallbackinfo->outMat)) {
                    OPENCV_LOGE("Copy2NapiMat failed.");
                }
            }
        },
        AsyncCompleteCallbackEqualizeHist,
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