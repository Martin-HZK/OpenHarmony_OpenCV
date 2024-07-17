import { AsyncCallback } from './basic';
import { Mat } from './opencv/opencvMat';
import { ImreadModes,ImwriteFlags  } from './opencv/imgcodecs';

declare namespace opencv {
  
    function imRead(fileName: string,mode:ImreadModes,callback?: AsyncCallback<Mat>):void;
    function imRead(fileName: string,mode:ImreadModes):Promise<Mat>;

    function imWrite(fileName: string,inMat: Mat,writeFlag:ImwriteFlags, callback?: AsyncCallback<void>):void;
    function imWrite(fileName: string,inMat: Mat,writeFlag:ImwriteFlags):Promise<void>;
 
  }
  
  export default opencv;