

export enum ColorConversionCodes {
    COLOR_BGR2BGRA     = 0, //!< add alpha channel to RGB or BGR image
    COLOR_RGB2RGBA     = COLOR_BGR2BGRA,

    COLOR_BGRA2BGR     = 1, //!< remove alpha channel from RGB or BGR image
    COLOR_RGBA2RGB     = COLOR_BGRA2BGR,

    COLOR_BGR2RGBA     = 2, //!< convert between RGB and BGR color spaces (with or without alpha channel)
    COLOR_RGB2BGRA     = COLOR_BGR2RGBA,

    COLOR_RGBA2BGR     = 3,
    COLOR_BGRA2RGB     = COLOR_RGBA2BGR,

    COLOR_BGR2RGB      = 4,
    COLOR_RGB2BGR      = COLOR_BGR2RGB,

    COLOR_BGRA2RGBA    = 5,
    COLOR_RGBA2BGRA    = COLOR_BGRA2RGBA,

    COLOR_BGR2GRAY     = 6, //!< convert between RGB/BGR and grayscale, @ref color_convert_rgb_gray "color conversions"
    COLOR_RGB2GRAY     = 7,
    COLOR_GRAY2BGR     = 8,
    COLOR_GRAY2RGB     = COLOR_GRAY2BGR,
    COLOR_GRAY2BGRA    = 9,
    COLOR_GRAY2RGBA    = COLOR_GRAY2BGRA,
    COLOR_BGRA2GRAY    = 10,
    COLOR_RGBA2GRAY    = 11,

    COLOR_BGR2BGR565   = 12, //!< convert between RGB/BGR and BGR565 (16-bit images)
    COLOR_RGB2BGR565   = 13,
    COLOR_BGR5652BGR   = 14,
    COLOR_BGR5652RGB   = 15,
    COLOR_BGRA2BGR565  = 16,
    COLOR_RGBA2BGR565  = 17,
    COLOR_BGR5652BGRA  = 18,
    COLOR_BGR5652RGBA  = 19,

    COLOR_GRAY2BGR565  = 20, //!< convert between grayscale to BGR565 (16-bit images)
    COLOR_BGR5652GRAY  = 21,

    COLOR_BGR2BGR555   = 22,  //!< convert between RGB/BGR and BGR555 (16-bit images)
    COLOR_RGB2BGR555   = 23,
    COLOR_BGR5552BGR   = 24,
    COLOR_BGR5552RGB   = 25,
    COLOR_BGRA2BGR555  = 26,
    COLOR_RGBA2BGR555  = 27,
    COLOR_BGR5552BGRA  = 28,
    COLOR_BGR5552RGBA  = 29,

    COLOR_GRAY2BGR555  = 30, //!< convert between grayscale and BGR555 (16-bit images)
    COLOR_BGR5552GRAY  = 31,

    COLOR_BGR2XYZ      = 32, //!< convert RGB/BGR to CIE XYZ, @ref color_convert_rgb_xyz "color conversions"
    COLOR_RGB2XYZ      = 33,
    COLOR_XYZ2BGR      = 34,
    COLOR_XYZ2RGB      = 35,

    COLOR_BGR2YCrCb    = 36, //!< convert RGB/BGR to luma-chroma (aka YCC), @ref color_convert_rgb_ycrcb "color conversions"
    COLOR_RGB2YCrCb    = 37,
    COLOR_YCrCb2BGR    = 38,
    COLOR_YCrCb2RGB    = 39,

    COLOR_BGR2HSV      = 40, //!< convert RGB/BGR to HSV (hue saturation value) with H range 0..180 if 8 bit image, @ref color_convert_rgb_hsv "color conversions"
    COLOR_RGB2HSV      = 41,

    COLOR_BGR2Lab      = 44, //!< convert RGB/BGR to CIE Lab, @ref color_convert_rgb_lab "color conversions"
    COLOR_RGB2Lab      = 45,

    COLOR_BGR2Luv      = 50, //!< convert RGB/BGR to CIE Luv, @ref color_convert_rgb_luv "color conversions"
    COLOR_RGB2Luv      = 51,
    COLOR_BGR2HLS      = 52, //!< convert RGB/BGR to HLS (hue lightness saturation) with H range 0..180 if 8 bit image, @ref color_convert_rgb_hls "color conversions"
    COLOR_RGB2HLS      = 53,

    COLOR_HSV2BGR      = 54, //!< backward conversions HSV to RGB/BGR with H range 0..180 if 8 bit image
    COLOR_HSV2RGB      = 55,

    COLOR_Lab2BGR      = 56,
    COLOR_Lab2RGB      = 57,
    COLOR_Luv2BGR      = 58,
    COLOR_Luv2RGB      = 59,
    COLOR_HLS2BGR      = 60, //!< backward conversions HLS to RGB/BGR with H range 0..180 if 8 bit image
    COLOR_HLS2RGB      = 61,

    COLOR_BGR2HSV_FULL = 66, //!< convert RGB/BGR to HSV (hue saturation value) with H range 0..255 if 8 bit image, @ref color_convert_rgb_hsv "color conversions"
    COLOR_RGB2HSV_FULL = 67,
    COLOR_BGR2HLS_FULL = 68, //!< convert RGB/BGR to HLS (hue lightness saturation) with H range 0..255 if 8 bit image, @ref color_convert_rgb_hls "color conversions"
    COLOR_RGB2HLS_FULL = 69,

    COLOR_HSV2BGR_FULL = 70, //!< backward conversions HSV to RGB/BGR with H range 0..255 if 8 bit image
    COLOR_HSV2RGB_FULL = 71,
    COLOR_HLS2BGR_FULL = 72, //!< backward conversions HLS to RGB/BGR with H range 0..255 if 8 bit image
    COLOR_HLS2RGB_FULL = 73,

    COLOR_LBGR2Lab     = 74,
    COLOR_LRGB2Lab     = 75,
    COLOR_LBGR2Luv     = 76,
    COLOR_LRGB2Luv     = 77,

    COLOR_Lab2LBGR     = 78,
    COLOR_Lab2LRGB     = 79,
    COLOR_Luv2LBGR     = 80,
    COLOR_Luv2LRGB     = 81,

    COLOR_BGR2YUV      = 82, //!< convert between RGB/BGR and YUV
    COLOR_RGB2YUV      = 83,
    COLOR_YUV2BGR      = 84,
    COLOR_YUV2RGB      = 85,

    //! YUV 4:2:0 family to RGB
    COLOR_YUV2RGB_NV12  = 90,
    COLOR_YUV2BGR_NV12  = 91,
    COLOR_YUV2RGB_NV21  = 92,
    COLOR_YUV2BGR_NV21  = 93,
    COLOR_YUV420sp2RGB  = COLOR_YUV2RGB_NV21,
    COLOR_YUV420sp2BGR  = COLOR_YUV2BGR_NV21,

    COLOR_YUV2RGBA_NV12 = 94,
    COLOR_YUV2BGRA_NV12 = 95,
    COLOR_YUV2RGBA_NV21 = 96,
    COLOR_YUV2BGRA_NV21 = 97,
    COLOR_YUV420sp2RGBA = COLOR_YUV2RGBA_NV21,
    COLOR_YUV420sp2BGRA = COLOR_YUV2BGRA_NV21,

    COLOR_YUV2RGB_YV12  = 98,
    COLOR_YUV2BGR_YV12  = 99,
    COLOR_YUV2RGB_IYUV  = 100,
    COLOR_YUV2BGR_IYUV  = 101,
    COLOR_YUV2RGB_I420  = COLOR_YUV2RGB_IYUV,
    COLOR_YUV2BGR_I420  = COLOR_YUV2BGR_IYUV,
    COLOR_YUV420p2RGB   = COLOR_YUV2RGB_YV12,
    COLOR_YUV420p2BGR   = COLOR_YUV2BGR_YV12,

    COLOR_YUV2RGBA_YV12 = 102,
    COLOR_YUV2BGRA_YV12 = 103,
    COLOR_YUV2RGBA_IYUV = 104,
    COLOR_YUV2BGRA_IYUV = 105,
    COLOR_YUV2RGBA_I420 = COLOR_YUV2RGBA_IYUV,
    COLOR_YUV2BGRA_I420 = COLOR_YUV2BGRA_IYUV,
    COLOR_YUV420p2RGBA  = COLOR_YUV2RGBA_YV12,
    COLOR_YUV420p2BGRA  = COLOR_YUV2BGRA_YV12,

    COLOR_YUV2GRAY_420  = 106,
    COLOR_YUV2GRAY_NV21 = COLOR_YUV2GRAY_420,
    COLOR_YUV2GRAY_NV12 = COLOR_YUV2GRAY_420,
    COLOR_YUV2GRAY_YV12 = COLOR_YUV2GRAY_420,
    COLOR_YUV2GRAY_IYUV = COLOR_YUV2GRAY_420,
    COLOR_YUV2GRAY_I420 = COLOR_YUV2GRAY_420,
    COLOR_YUV420sp2GRAY = COLOR_YUV2GRAY_420,
    COLOR_YUV420p2GRAY  = COLOR_YUV2GRAY_420,

    //! YUV 4:2:2 family to RGB
    COLOR_YUV2RGB_UYVY = 107,
    COLOR_YUV2BGR_UYVY = 108,
    //COLOR_YUV2RGB_VYUY = 109,
    //COLOR_YUV2BGR_VYUY = 110,
    COLOR_YUV2RGB_Y422 = COLOR_YUV2RGB_UYVY,
    COLOR_YUV2BGR_Y422 = COLOR_YUV2BGR_UYVY,
    COLOR_YUV2RGB_UYNV = COLOR_YUV2RGB_UYVY,
    COLOR_YUV2BGR_UYNV = COLOR_YUV2BGR_UYVY,

    COLOR_YUV2RGBA_UYVY = 111,
    COLOR_YUV2BGRA_UYVY = 112,
    //COLOR_YUV2RGBA_VYUY = 113,
    //COLOR_YUV2BGRA_VYUY = 114,
    COLOR_YUV2RGBA_Y422 = COLOR_YUV2RGBA_UYVY,
    COLOR_YUV2BGRA_Y422 = COLOR_YUV2BGRA_UYVY,
    COLOR_YUV2RGBA_UYNV = COLOR_YUV2RGBA_UYVY,
    COLOR_YUV2BGRA_UYNV = COLOR_YUV2BGRA_UYVY,

    COLOR_YUV2RGB_YUY2 = 115,
    COLOR_YUV2BGR_YUY2 = 116,
    COLOR_YUV2RGB_YVYU = 117,
    COLOR_YUV2BGR_YVYU = 118,
    COLOR_YUV2RGB_YUYV = COLOR_YUV2RGB_YUY2,
    COLOR_YUV2BGR_YUYV = COLOR_YUV2BGR_YUY2,
    COLOR_YUV2RGB_YUNV = COLOR_YUV2RGB_YUY2,
    COLOR_YUV2BGR_YUNV = COLOR_YUV2BGR_YUY2,

    COLOR_YUV2RGBA_YUY2 = 119,
    COLOR_YUV2BGRA_YUY2 = 120,
    COLOR_YUV2RGBA_YVYU = 121,
    COLOR_YUV2BGRA_YVYU = 122,
    COLOR_YUV2RGBA_YUYV = COLOR_YUV2RGBA_YUY2,
    COLOR_YUV2BGRA_YUYV = COLOR_YUV2BGRA_YUY2,
    COLOR_YUV2RGBA_YUNV = COLOR_YUV2RGBA_YUY2,
    COLOR_YUV2BGRA_YUNV = COLOR_YUV2BGRA_YUY2,

    COLOR_YUV2GRAY_UYVY = 123,
    COLOR_YUV2GRAY_YUY2 = 124,
    //CV_YUV2GRAY_VYUY    = CV_YUV2GRAY_UYVY,
    COLOR_YUV2GRAY_Y422 = COLOR_YUV2GRAY_UYVY,
    COLOR_YUV2GRAY_UYNV = COLOR_YUV2GRAY_UYVY,
    COLOR_YUV2GRAY_YVYU = COLOR_YUV2GRAY_YUY2,
    COLOR_YUV2GRAY_YUYV = COLOR_YUV2GRAY_YUY2,
    COLOR_YUV2GRAY_YUNV = COLOR_YUV2GRAY_YUY2,

    //! alpha premultiplication
    COLOR_RGBA2mRGBA    = 125,
    COLOR_mRGBA2RGBA    = 126,

    //! RGB to YUV 4:2:0 family
    COLOR_RGB2YUV_I420  = 127,
    COLOR_BGR2YUV_I420  = 128,
    COLOR_RGB2YUV_IYUV  = COLOR_RGB2YUV_I420,
    COLOR_BGR2YUV_IYUV  = COLOR_BGR2YUV_I420,

    COLOR_RGBA2YUV_I420 = 129,
    COLOR_BGRA2YUV_I420 = 130,
    COLOR_RGBA2YUV_IYUV = COLOR_RGBA2YUV_I420,
    COLOR_BGRA2YUV_IYUV = COLOR_BGRA2YUV_I420,
    COLOR_RGB2YUV_YV12  = 131,
    COLOR_BGR2YUV_YV12  = 132,
    COLOR_RGBA2YUV_YV12 = 133,
    COLOR_BGRA2YUV_YV12 = 134,

    //! Demosaicing, see @ref color_convert_bayer "color conversions" for additional information
    COLOR_BayerBG2BGR = 46, //!< equivalent to RGGB Bayer pattern
    COLOR_BayerGB2BGR = 47, //!< equivalent to GRBG Bayer pattern
    COLOR_BayerRG2BGR = 48, //!< equivalent to BGGR Bayer pattern
    COLOR_BayerGR2BGR = 49, //!< equivalent to GBRG Bayer pattern

    COLOR_BayerRGGB2BGR = COLOR_BayerBG2BGR,
    COLOR_BayerGRBG2BGR = COLOR_BayerGB2BGR,
    COLOR_BayerBGGR2BGR = COLOR_BayerRG2BGR,
    COLOR_BayerGBRG2BGR = COLOR_BayerGR2BGR,

    COLOR_BayerRGGB2RGB = COLOR_BayerBGGR2BGR,
    COLOR_BayerGRBG2RGB = COLOR_BayerGBRG2BGR,
    COLOR_BayerBGGR2RGB = COLOR_BayerRGGB2BGR,
    COLOR_BayerGBRG2RGB = COLOR_BayerGRBG2BGR,

    COLOR_BayerBG2RGB = COLOR_BayerRG2BGR, //!< equivalent to RGGB Bayer pattern
    COLOR_BayerGB2RGB = COLOR_BayerGR2BGR, //!< equivalent to GRBG Bayer pattern
    COLOR_BayerRG2RGB = COLOR_BayerBG2BGR, //!< equivalent to BGGR Bayer pattern
    COLOR_BayerGR2RGB = COLOR_BayerGB2BGR, //!< equivalent to GBRG Bayer pattern

    COLOR_BayerBG2GRAY = 86, //!< equivalent to RGGB Bayer pattern
    COLOR_BayerGB2GRAY = 87, //!< equivalent to GRBG Bayer pattern
    COLOR_BayerRG2GRAY = 88, //!< equivalent to BGGR Bayer pattern
    COLOR_BayerGR2GRAY = 89, //!< equivalent to GBRG Bayer pattern

    COLOR_BayerRGGB2GRAY = COLOR_BayerBG2GRAY,
    COLOR_BayerGRBG2GRAY = COLOR_BayerGB2GRAY,
    COLOR_BayerBGGR2GRAY = COLOR_BayerRG2GRAY,
    COLOR_BayerGBRG2GRAY = COLOR_BayerGR2GRAY,

    //! Demosaicing using Variable Number of Gradients
    COLOR_BayerBG2BGR_VNG = 62, //!< equivalent to RGGB Bayer pattern
    COLOR_BayerGB2BGR_VNG = 63, //!< equivalent to GRBG Bayer pattern
    COLOR_BayerRG2BGR_VNG = 64, //!< equivalent to BGGR Bayer pattern
    COLOR_BayerGR2BGR_VNG = 65, //!< equivalent to GBRG Bayer pattern

    COLOR_BayerRGGB2BGR_VNG = COLOR_BayerBG2BGR_VNG,
    COLOR_BayerGRBG2BGR_VNG = COLOR_BayerGB2BGR_VNG,
    COLOR_BayerBGGR2BGR_VNG = COLOR_BayerRG2BGR_VNG,
    COLOR_BayerGBRG2BGR_VNG = COLOR_BayerGR2BGR_VNG,

    COLOR_BayerRGGB2RGB_VNG = COLOR_BayerBGGR2BGR_VNG,
    COLOR_BayerGRBG2RGB_VNG = COLOR_BayerGBRG2BGR_VNG,
    COLOR_BayerBGGR2RGB_VNG = COLOR_BayerRGGB2BGR_VNG,
    COLOR_BayerGBRG2RGB_VNG = COLOR_BayerGRBG2BGR_VNG,

    COLOR_BayerBG2RGB_VNG = COLOR_BayerRG2BGR_VNG, //!< equivalent to RGGB Bayer pattern
    COLOR_BayerGB2RGB_VNG = COLOR_BayerGR2BGR_VNG, //!< equivalent to GRBG Bayer pattern
    COLOR_BayerRG2RGB_VNG = COLOR_BayerBG2BGR_VNG, //!< equivalent to BGGR Bayer pattern
    COLOR_BayerGR2RGB_VNG = COLOR_BayerGB2BGR_VNG, //!< equivalent to GBRG Bayer pattern

    //! Edge-Aware Demosaicing
    COLOR_BayerBG2BGR_EA  = 135, //!< equivalent to RGGB Bayer pattern
    COLOR_BayerGB2BGR_EA  = 136, //!< equivalent to GRBG Bayer pattern
    COLOR_BayerRG2BGR_EA  = 137, //!< equivalent to BGGR Bayer pattern
    COLOR_BayerGR2BGR_EA  = 138, //!< equivalent to GBRG Bayer pattern

    COLOR_BayerRGGB2BGR_EA  = COLOR_BayerBG2BGR_EA,
    COLOR_BayerGRBG2BGR_EA  = COLOR_BayerGB2BGR_EA,
    COLOR_BayerBGGR2BGR_EA  = COLOR_BayerRG2BGR_EA,
    COLOR_BayerGBRG2BGR_EA  = COLOR_BayerGR2BGR_EA,

    COLOR_BayerRGGB2RGB_EA  = COLOR_BayerBGGR2BGR_EA,
    COLOR_BayerGRBG2RGB_EA  = COLOR_BayerGBRG2BGR_EA,
    COLOR_BayerBGGR2RGB_EA  = COLOR_BayerRGGB2BGR_EA,
    COLOR_BayerGBRG2RGB_EA  = COLOR_BayerGRBG2BGR_EA,

    COLOR_BayerBG2RGB_EA  = COLOR_BayerRG2BGR_EA, //!< equivalent to RGGB Bayer pattern
    COLOR_BayerGB2RGB_EA  = COLOR_BayerGR2BGR_EA, //!< equivalent to GRBG Bayer pattern
    COLOR_BayerRG2RGB_EA  = COLOR_BayerBG2BGR_EA, //!< equivalent to BGGR Bayer pattern
    COLOR_BayerGR2RGB_EA  = COLOR_BayerGB2BGR_EA, //!< equivalent to GBRG Bayer pattern

    //! Demosaicing with alpha channel
    COLOR_BayerBG2BGRA = 139, //!< equivalent to RGGB Bayer pattern
    COLOR_BayerGB2BGRA = 140, //!< equivalent to GRBG Bayer pattern
    COLOR_BayerRG2BGRA = 141, //!< equivalent to BGGR Bayer pattern
    COLOR_BayerGR2BGRA = 142, //!< equivalent to GBRG Bayer pattern

    COLOR_BayerRGGB2BGRA = COLOR_BayerBG2BGRA,
    COLOR_BayerGRBG2BGRA = COLOR_BayerGB2BGRA,
    COLOR_BayerBGGR2BGRA = COLOR_BayerRG2BGRA,
    COLOR_BayerGBRG2BGRA = COLOR_BayerGR2BGRA,

    COLOR_BayerRGGB2RGBA = COLOR_BayerBGGR2BGRA,
    COLOR_BayerGRBG2RGBA = COLOR_BayerGBRG2BGRA,
    COLOR_BayerBGGR2RGBA = COLOR_BayerRGGB2BGRA,
    COLOR_BayerGBRG2RGBA = COLOR_BayerGRBG2BGRA,

    COLOR_BayerBG2RGBA = COLOR_BayerRG2BGRA, //!< equivalent to RGGB Bayer pattern
    COLOR_BayerGB2RGBA = COLOR_BayerGR2BGRA, //!< equivalent to GRBG Bayer pattern
    COLOR_BayerRG2RGBA = COLOR_BayerBG2BGRA, //!< equivalent to BGGR Bayer pattern
    COLOR_BayerGR2RGBA = COLOR_BayerGB2BGRA, //!< equivalent to GBRG Bayer pattern

    COLOR_COLORCVT_MAX  = 143
}

//! interpolation algorithm
export enum InterpolationFlags{
    /** nearest neighbor interpolation */
    INTER_NEAREST        = 0,
    /** bilinear interpolation */
    INTER_LINEAR         = 1,
    /** bicubic interpolation */
    INTER_CUBIC          = 2,
    /** resampling using pixel area relation. It may be a preferred method for image decimation, as
    it gives moire'-free results. But when the image is zoomed, it is similar to the INTER_NEAREST
    method. */
    INTER_AREA           = 3,
    /** Lanczos interpolation over 8x8 neighborhood */
    INTER_LANCZOS4       = 4,
    /** Bit exact bilinear interpolation */
    INTER_LINEAR_EXACT = 5,
    /** Bit exact nearest neighbor interpolation. This will produce same results as
    the nearest neighbor method in PIL, scikit-image or Matlab. */
    INTER_NEAREST_EXACT  = 6,
    /** mask for interpolation codes */
    INTER_MAX            = 7,
    /** flag, fills all of the destination image pixels. If some of them correspond to outliers in the
    source image, they are set to zero */
    WARP_FILL_OUTLIERS   = 8,
    /** flag, inverse transformation

    For example, #linearPolar or #logPolar transforms:
    - flag is __not__ set: \f$dst( \rho , \phi ) = src(x,y)\f$
    - flag is set: \f$dst(x,y) = src( \rho , \phi )\f$
    */
    WARP_INVERSE_MAP     = 16
}

export enum ThresholdTypes {
    THRESH_BINARY     = 0, //!< \f[\texttt{dst} (x,y) =  \fork{\texttt{maxval}}{if \(\texttt{src}(x,y) > \texttt{thresh}\)}{0}{otherwise}\f]
    THRESH_BINARY_INV = 1, //!< \f[\texttt{dst} (x,y) =  \fork{0}{if \(\texttt{src}(x,y) > \texttt{thresh}\)}{\texttt{maxval}}{otherwise}\f]
    THRESH_TRUNC      = 2, //!< \f[\texttt{dst} (x,y) =  \fork{\texttt{threshold}}{if \(\texttt{src}(x,y) > \texttt{thresh}\)}{\texttt{src}(x,y)}{otherwise}\f]
    THRESH_TOZERO     = 3, //!< \f[\texttt{dst} (x,y) =  \fork{\texttt{src}(x,y)}{if \(\texttt{src}(x,y) > \texttt{thresh}\)}{0}{otherwise}\f]
    THRESH_TOZERO_INV = 4, //!< \f[\texttt{dst} (x,y) =  \fork{0}{if \(\texttt{src}(x,y) > \texttt{thresh}\)}{\texttt{src}(x,y)}{otherwise}\f]
    THRESH_MASK       = 7,
    THRESH_OTSU       = 8, //!< flag, use Otsu algorithm to choose the optimal threshold value
    THRESH_TRIANGLE   = 16 //!< flag, use Triangle algorithm to choose the optimal threshold value
}

export enum WarpPolarMode
{
    WARP_POLAR_LINEAR = 0, ///< Remaps an image to/from polar space.
    WARP_POLAR_LOG = 256   ///< Remaps an image to/from semilog-polar space.
}

//OpenCV Point
export interface Point {
    x: number;
    y: number;
}

export enum HersheyFonts {
    FONT_HERSHEY_SIMPLEX        = 0, //!< normal size sans-serif font
    FONT_HERSHEY_PLAIN          = 1, //!< small size sans-serif font
    FONT_HERSHEY_DUPLEX         = 2, //!< normal size sans-serif font (more complex than FONT_HERSHEY_SIMPLEX)
    FONT_HERSHEY_COMPLEX        = 3, //!< normal size serif font
    FONT_HERSHEY_TRIPLEX        = 4, //!< normal size serif font (more complex than FONT_HERSHEY_COMPLEX)
    FONT_HERSHEY_COMPLEX_SMALL  = 5, //!< smaller version of FONT_HERSHEY_COMPLEX
    FONT_HERSHEY_SCRIPT_SIMPLEX = 6, //!< hand-writing style font
    FONT_HERSHEY_SCRIPT_COMPLEX = 7, //!< more complex variant of FONT_HERSHEY_SCRIPT_SIMPLEX
    FONT_ITALIC                 = 16 //!< flag for italic font
}

export interface Hierarchy4i {
    v0: number;
    v1: number;
    v2: number;
    v3: number;
}

export interface ContoursInfo {
    contours: Array<Array<Point>>
    hierarchy: Array<Hierarchy4i>
}

export interface Line2fInfo {
    rho: number;
    theta: number;
}