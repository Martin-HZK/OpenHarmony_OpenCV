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

import resourceManager from '@ohos.resourceManager';
export interface PixelInfo {
  rows: number;
  cols: number;
  buffSize: number;
  byteBuffer: ArrayBuffer;
}
export interface Qrcode {
  detected: boolean;
  decodes?: string[];
}
export const SayHello: (msg: string) => string;
export const img2Gray: (resmgr: resourceManager.ResourceManager, path: string, file: string) => PixelInfo;
export const qrcode: (resmgr: resourceManager.ResourceManager, path: string, file: string) => Promise<Qrcode>;
export const edgeDetection: (resmgr: resourceManager.ResourceManager, path: string, file: string) => PixelInfo;
export const faceDetect: (resmgr: resourceManager.ResourceManager, path: string, file: string) => PixelInfo;
