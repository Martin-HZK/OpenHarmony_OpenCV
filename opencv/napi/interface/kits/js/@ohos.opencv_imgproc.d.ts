import { AsyncCallback } from './basic';
import { Mat } from './opencv/opencvMat';
import { ColorConversionCodes } from './opencv/imgproc';
import { InterpolationFlags } from './opencv/imgproc';
import { ThresholdTypes } from './opencv/imgproc';
import { ContoursInfo } from './opencv/imgproc';
import { Line2fInfo } from './opencv/imgproc';
import { BorderTypes } from './opencv/core';
import { Point } from './opencv/core';
import { Scalar } from './opencv/core';
import { Size } from './opencv/core';

declare namespace opencv {

    function cvtColor(inMat: Mat, cvtCode:ColorConversionCodes, callback?: AsyncCallback<Mat>):void;
    function cvtColor(inMat: Mat, cvtCode:ColorConversionCodes):Promise<Mat>;
    
    function edgeDetect(inMat: Mat, callback?: AsyncCallback<Mat>):void;
    function edgeDetect(inMat: Mat):Promise<Mat>;

    function resize(inMat: Mat, width: Number, height: Number, scale_x?: Number, scale_y?: Number, interpolation?: InterpolationFlags, callback?: AsyncCallback<Mat>):void;
    function resize(inMat: Mat, width: Number, height: Number, scale_x?: Number, scale_y?: Number, interpolation?: InterpolationFlags):Promise<Mat>;

    function warpAffine(inMat: Mat, angle: Number, scale: Number, interpolation?: InterpolationFlags, borderMode?: BorderTypes, callback?: AsyncCallback<Mat>):void;
    function warpAffine(inMat: Mat, angle: Number, scale: Number, interpolation?: InterpolationFlags, borderMode?: BorderTypes):Promise<Mat>; 
   
    function threshold(inMat: Mat, thresh: Number, maxval: Number, type: ThresholdTypes, callback?: AsyncCallback<Mat>):void;
    function threshold(inMat: Mat, thresh: Number, maxval: Number, type: ThresholdTypes):Promise<Mat>;   

    function adaptiveThreshold(inMat: Mat, maxValue: Number, adaptiveMethod: Number, thresholdType: Number, blockSize: Number, C: Number, callback?: AsyncCallback<Mat>):void;
    function adaptiveThreshold(inMat: Mat, maxValue: Number, adaptiveMethod: Number, thresholdType: Number, blockSize: Number, C: Number):Promise<Mat>; 
    
    function warpPolar(inMat: Mat, destWidth: Number, destHeight: Number, center_x: Number, center_y: Number, maxRadius: Number, flags: Number, callback?: AsyncCallback<Mat>):void;
    function warpPolar(inMat: Mat, destWidth: Number, destHeight: Number, center_x: Number, center_y: Number, maxRadius: Number, flags: Number):Promise<Mat>; 

    function circle(inMat: Mat, center_x: Number, center_y: Number, radius: Number, scalar_1: Number, scalar_2: Number, scalar_3: Number, thickness: Number, callback?: AsyncCallback<Mat>):void;
    function circle(inMat: Mat, center_x: Number, center_y: Number, radius: Number, scalar_1: Number, scalar_2: Number, scalar_3: Number, thickness: Number):Promise<Mat>;
    
    function line(inMat: Mat, pt1: Point, pt2: Point, color: Scalar, thickness: Number, callback?: AsyncCallback<Mat>):void;
    function line(inMat: Mat, pt1: Point, pt2: Point, color: Scalar, thickness: Number):Promise<Mat>; 

    function ellipse(inMat:Mat, center:Point, axes:Size, angle:Number, startAngle:Number, endAngle:Number, color:Scalar, thickness:Number, callback?: AsyncCallback<Mat>):void;
    function ellipse(inMat:Mat, center:Point, axes:Size, angle:Number, startAngle:Number, endAngle:Number, color:Scalar, thickness:Number):Promise<Mat>;

    function putText(inMat:Mat, text:string, org:Point, fontFace:Number, fontScale:Number, color:Scalar, thickness:Number, lineType:Number, bottomLeftOrigin:boolean, callback?: AsyncCallback<Mat>):void;
    function putText(inMat:Mat, text:string, org:Point, fontFace:Number, fontScale:Number, color:Scalar, thickness:Number, lineType:Number, bottomLeftOrigin:boolean):Promise<Mat>; 

    function pyrDown(inMat:Mat, dsize:Size, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function pyrDown(inMat:Mat, dsize:Size, borderType:Number):Promise<Mat>;

    function pyrUp(inMat:Mat, dsize:Size, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function pyrUp(inMat:Mat, dsize:Size, borderType:Number):Promise<Mat>; 

    function fillPoly(inMat:Mat, pts:Array<Point>, color:Scalar, lineType:Number, shift:Number, offset:Point, callback?: AsyncCallback<Mat>):void;
    function fillPoly(inMat:Mat, pts:Array<Point>, color:Scalar, lineType:Number, shift:Number, offset:Point):Promise<Mat>; 

    function filter2D(inMat:Mat, ddepth:Number, kernel:Array<Number>, anchor:Point, delta:Number, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function filter2D(inMat:Mat, ddepth:Number, kernel:Array<Number>, anchor:Point, delta:Number, borderType:Number):Promise<Mat>;

    function blur(inMat:Mat, ksize:Size, anchor:Point, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function blur(inMat:Mat, ksize:Size, anchor:Point, borderType:Number):Promise<Mat>;

    function boxFilter(inMat:Mat, ddepth:Number, ksize:Size, anchor:Point, normalize:boolean, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function boxFilter(inMat:Mat, ddepth:Number, ksize:Size, anchor:Point, normalize:boolean, borderType:Number):Promise<Mat>;

    function sqrBoxFilter(inMat:Mat, ddepth:Number, ksize:Size, anchor:Point, normalize:boolean, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function sqrBoxFilter(inMat:Mat, ddepth:Number, ksize:Size, anchor:Point, normalize:boolean, borderType:Number):Promise<Mat>;

    function GaussianBlur(inMat:Mat, ksize:Size, sigmaX:Number, sigmaY:Number, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function GaussianBlur(inMat:Mat, ksize:Size, sigmaX:Number, sigmaY:Number, borderType:Number):Promise<Mat>;

    function bilateralFilter(inMat:Mat, d:Number, sigmaColor:Number, sigmaSpace:Number, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function bilateralFilter(inMat:Mat, d:Number, sigmaColor:Number, sigmaSpace:Number, borderType:Number):Promise<Mat>;

    function medianBlur(inMat:Mat, ksize:Number, callback?: AsyncCallback<Mat>):void;
    function medianBlur(inMat:Mat, ksize:Number):Promise<Mat>;

    function Sobel(inMat:Mat, ddepth:Number, dx:Number, dy:Number, ksize:Number, scale:Number, delta:Number, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function Sobel(inMat:Mat, ddepth:Number, dx:Number, dy:Number, ksize:Number, scale:Number, delta:Number, borderType:Number):Promise<Mat>;

    function Canny(inMat:Mat, threshold1:Number, threshold2:Number, apertureSize:Number, L2gradient:boolean, callback?: AsyncCallback<Mat>):void;
    function Canny(inMat:Mat, threshold1:Number, threshold2:Number, apertureSize:Number, L2gradient:boolean):Promise<Mat>;

    function Laplacian(inMat:Mat, ddepth:Number, ksize:Number, scale:Number, delta:Number, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function Laplacian(inMat:Mat, ddepth:Number, ksize:Number, scale:Number, delta:Number, borderType:Number):Promise<Mat>;

    function Scharr(inMat:Mat, ddepth:Number, dx:Number, dy:Number, scale:Number, delta:Number, borderType:Number, callback?: AsyncCallback<Mat>):void;
    function Scharr(inMat:Mat, ddepth:Number, dx:Number, dy:Number, scale:Number, delta:Number, borderType:Number):Promise<Mat>;

    function findContours(inMat:Mat, mode:Number, method:Number, offset:Point, callback?: AsyncCallback<ContoursInfo>):void;
    function findContours(inMat:Mat, mode:Number, method:Number, offset:Point):Promise<ContoursInfo>;

    function HoughLines(inMat:Mat, rho:Number, theta:Number, threshold:Number, srn:Number, stn:Number, min_theta:Number, max_theta:Number, callback?: AsyncCallback<Array<Line2fInfo>>):void;
    function HoughLines(inMat:Mat, rho:Number, theta:Number, threshold:Number, srn:Number, stn:Number, min_theta:Number, max_theta:Number):Promise<Array<Line2fInfo>>;
}
  
  export default opencv;