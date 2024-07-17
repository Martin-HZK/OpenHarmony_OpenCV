#include "n_imgproc.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv_log_wrapper.h"

using namespace OHOS::Opencv;

namespace OHOS {
namespace OpencvNapi {
struct ParametersMat2PixelData {
    MatInfo mat;
    int PixelMapFormat;
    napi_ref callback = nullptr;
};

struct AsyncCallbackInfoMat2Pixel {
    napi_env env = nullptr;
    napi_async_work asyncWork = nullptr;
    ParametersMat2PixelData params;
    CallbackPromiseInfo info;
    MatInfo outMat;
};

const int RGB_565 = 2;
const int RGBA_8888 = 3;

napi_value ParseParametersMat2PixelData(const napi_env &env, const napi_callback_info &info, ParametersMat2PixelData &paras)
{
    OPENCV_LOGI("enter ParseParametersMat2PixelData");

    size_t argc = 3;
    napi_value argv[3] = {nullptr};
    napi_value thisVar = nullptr;
    napi_valuetype valuetype = napi_undefined;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, NULL));
    if (argc < PARAM3) {
        OPENCV_LOGE("Wrong number of arguments\n");
        return nullptr;
    }
    
    Common::GetMatInfo( env, argv[PARAM0], paras.mat );
    Common::GetInt32Value( env, argv[PARAM1], paras.PixelMapFormat );

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

void AsyncCompleteCallbackMat2Pixel(napi_env env, napi_status status, void *data)
{
    OPENCV_LOGE("enter AsyncCompleteCallbackMat2Pixel");

    if (!data) {
        OPENCV_LOGE("Invalid async callback data\n");
        return;
    }
    auto asynccallbackinfo = static_cast<AsyncCallbackInfoMat2Pixel *>(data);
    napi_value result = Common::NapiGetNull(env);

    asynccallbackinfo->info.errorCode = ERR_OK;
    if ( asynccallbackinfo->info.errorCode == ERR_OK ) {
        if ( asynccallbackinfo->outMat.data == nullptr ) {
            asynccallbackinfo->info.errorCode = ERROR;
        } else {
            napi_create_object(env, &result);
            if ( !Common::SetMat(env, asynccallbackinfo->outMat.rows, asynccallbackinfo->outMat.cols, asynccallbackinfo->outMat.type, asynccallbackinfo->outMat.bufferSize, asynccallbackinfo->outMat.data, result )) {
                asynccallbackinfo->info.errorCode = ERROR;
                result = Common::NapiGetNull(env);
            }
        }
    }
    Common::ReturnCallbackPromise(env, asynccallbackinfo->info, result);
    if ( asynccallbackinfo->info.callback != nullptr ) {
        napi_delete_reference(env, asynccallbackinfo->info.callback);
    }
    napi_delete_async_work(env, asynccallbackinfo->asyncWork);
    FreeMemoryArray<char>((char*)asynccallbackinfo->outMat.data);
    FreeMemoryArray<char>((char*)(asynccallbackinfo->params.mat.data));
    FreeMemory<AsyncCallbackInfoMat2Pixel>(asynccallbackinfo);
}

bool cvtMat2Pixel(InputArray _src, OutputArray& _dst, int code)
{
    if ( RGBA_8888 == code ) {
        switch ( _src.getMat_().type() ) {
            case CV_8UC1:
                cv::cvtColor( _src, _dst, COLOR_GRAY2RGBA );
                break;
            case  CV_8UC3:
                cv::cvtColor( _src, _dst, COLOR_RGB2RGBA );
                break;
            case CV_8UC4:
            default:
                _src.copyTo(_dst);
        }
    }
    else if ( RGB_565 == code ) {
        switch ( _src.getMat_().type() ) {
            case CV_8UC1:
                cv::cvtColor( _src, _dst, COLOR_GRAY2BGR565 );
                break;
            case  CV_8UC3:
                cv::cvtColor( _src, _dst, COLOR_RGB2BGR565 );
                break;
            case CV_8UC4:
            default:
                _src.copyTo(_dst);
        }
    }
    else {
        return false;
    }
    return true;
}

napi_value n_mat2pixeldata(napi_env env, napi_callback_info info)
{
    OPENCV_LOGI("Enter n_mat2colordata");
    ParametersMat2PixelData params;
    if (ParseParametersMat2PixelData(env, info, params) == nullptr) {
        return Common::NapiGetUndefined(env);
    }

    AsyncCallbackInfoMat2Pixel *asynccallbackinfo = new (std::nothrow)
        AsyncCallbackInfoMat2Pixel {.env = env, .asyncWork = nullptr, .params = params};
    if (!asynccallbackinfo) {
        return Common::JSParaError(env, params.callback);
    }

    napi_value promise = nullptr;
    Common::PaddingCallbackPromiseInfo(env, asynccallbackinfo->params.callback, asynccallbackinfo->info, promise);

    napi_value resourceName = nullptr;
    napi_create_string_latin1(env, "mat2pixeldata", NAPI_AUTO_LENGTH, &resourceName);
    
    napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            AsyncCallbackInfoMat2Pixel *asynccallbackinfo = static_cast<AsyncCallbackInfoMat2Pixel *>(data);
            if (asynccallbackinfo) {
                cv::Mat inMat( asynccallbackinfo->params.mat.rows, asynccallbackinfo->params.mat.cols, asynccallbackinfo->params.mat.type, asynccallbackinfo->params.mat.data );
                cv::Mat outMat;
                cvtMat2Pixel(inMat, outMat, asynccallbackinfo->params.PixelMapFormat);
                if (!Common::Copy2NapiMat(outMat, asynccallbackinfo->outMat)) {
                    OPENCV_LOGE("Copy2NapiMat failed.");
                }
            }
        },
        AsyncCompleteCallbackMat2Pixel,
        (void*)asynccallbackinfo,
        &(asynccallbackinfo->asyncWork));

    NAPI_CALL(env, napi_queue_async_work(env, asynccallbackinfo->asyncWork));

    if (asynccallbackinfo->info.isCallback) {
        return Common::NapiGetNull(env);
    } else {
        return promise;
    }
}
} //OpencvNapi
} //OHOS
