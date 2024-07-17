import { AsyncCallback } from './basic';
import { Mat } from './opencv/opencvMat';
import { Qrcode } from './opencv/objdetect';

declare namespace opencv {

    function qrcode(inMat: Mat, detectOnly?: boolean, multiQR?: boolean, callback?: AsyncCallback<Qrcode>):void;
    function qrcode(inMat: Mat, detectOnly?: boolean, multiQR?: boolean):Promise<Qrcode>;
 
  }
  
  export default opencv;