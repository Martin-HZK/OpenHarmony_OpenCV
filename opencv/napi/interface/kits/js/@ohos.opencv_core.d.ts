import { AsyncCallback } from './basic';
import { Mat } from './opencv/opencvMat';
import { ColorConversionCodes } from './opencv/imgproc';
import { InterpolationFlags } from './opencv/imgproc';
import { BorderTypes } from './opencv/core';

declare namespace opencv {
    function copyMakeBorder(inMat: Mat, top: Number, bottom: Number, left: Number, right: Number, borderMode?: BorderTypes, callback?: AsyncCallback<Mat>):void;
    function copyMakeBorder(inMat: Mat, top: Number, bottom: Number, left: Number, right: Number, borderMode?: BorderTypes):Promise<Mat>;

    function vconcat(firstMat: Mat, secondMat: Mat, callback?: AsyncCallback<Mat>):void;
    function vconcat(firstMat: Mat, secondMat: Mat):Promise<Mat>;    

    function hconcat(firstMat: Mat, secondMat: Mat, callback?: AsyncCallback<Mat>):void;
    function hconcat(firstMat: Mat, secondMat: Mat):Promise<Mat>; 
    
    function flip(inMat: Mat, flipCode: Number, callback?: AsyncCallback<Mat>):void;
    function flip(inMat: Mat, flipCode: Number):Promise<Mat>; 

    function bitwise_and(inMat:Mat, secondMat:Mat, callback?: AsyncCallback<Mat>):void;
    function bitwise_and(inMat:Mat, secondMat:Mat):Promise<Mat>;  

    function bitwise_or(inMat:Mat, secondMat:Mat, callback?: AsyncCallback<Mat>):void;
    function bitwise_or(inMat:Mat, secondMat:Mat):Promise<Mat>;

    function bitwise_xor(inMat:Mat, secondMat:Mat, callback?: AsyncCallback<Mat>):void;
    function bitwise_xor(inMat:Mat, secondMat:Mat):Promise<Mat>;

    function bitwise_not(inMat:Mat, callback?: AsyncCallback<Mat>):void;
    function bitwise_not(inMat:Mat):Promise<Mat>;

    function convertScaleAbs(inMat:Mat, alpha:Number, beta:Number, callback?: AsyncCallback<Mat>):void;
    function convertScaleAbs(inMat:Mat, alpha:Number, beta:Number):Promise<Mat>;
  }
  
  export default opencv;