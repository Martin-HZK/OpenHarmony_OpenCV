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

import router from '@ohos.router';
import image from '@ohos.multimedia.image';
import Logger from '../../../util/Logger';
import opencv from 'libohos_opencv_sample.so';
import { TitleBar } from '../../../common/TitleBar'

const TAG: string = 'opencv_edgeDetection';

@Entry
@Component
struct EdgeDetection {
  private btnFontColor: Resource = $r('app.color.white');
  private pixelMapFormat:image.PixelMapFormat = 3;
  @State isGray: Boolean = false;
  @State imagePixelMap: PixelMap | undefined = undefined;

  build() {
    Column() {
      // 标题
      TitleBar({ title: $r('app.string.image_edge_detection') })

      Column() {
        Column() {
          if (this.isGray == false) {
            Image($rawfile('lena.jpg'))
              .margin({ left: 24, right: 24 })
              .objectFit(ImageFit.Contain)
              .id('backBtn')
          } else {
            Image(this.imagePixelMap)
              .margin({ left: 24, right: 24 })
              .objectFit(ImageFit.Contain)
              .id('backBtn')
          }
        }
        .width('100%')
        .height(480)
        .alignItems(HorizontalAlign.Center)
        .justifyContent(FlexAlign.Start)

        Row() {
          if (this.isGray) {
            Button('EdgeDetect', { type: ButtonType.Capsule })
              .backgroundColor(Color.Gray)
              .width(120)
              .height(48)
              .fontSize(16)
              .fontWeight(500)
              .fontColor(this.btnFontColor)
              .margin({ left: 24 })
              .id('bFaceDetect')
              .enabled(false)

          } else {
            Button('EdgeDetect', { type: ButtonType.Capsule })
              .backgroundColor(Color.Blue)
              .width(120)
              .height(48)
              .fontSize(16)
              .fontWeight(500)
              .fontColor(this.btnFontColor)
              .margin({ left: 24 })
              .id('bFaceDetect')
              .onClick(() => {
                let pixelInfo: opencv.PixelInfo = opencv.edgeDetection(getContext().resourceManager, '', 'lena.jpg');
                Logger.info(TAG, `pixelInfo buffSize: ${pixelInfo.buffSize}`);

                let opts : image.InitializationOptions = { editable: true, pixelFormat: this.pixelMapFormat, size: { height: pixelInfo.rows, width: pixelInfo.cols } }
                image.createPixelMap(pixelInfo.byteBuffer, opts, (error, pixelmap) => {
                  if (error) {
                    Logger.error(TAG, `Failed to create pixelmap error_code ${error.code}`);
                  } else {
                    Logger.info(TAG, 'Succeeded in creating pixelmap.');
                    this.imagePixelMap = pixelmap;
                  }
                })
                this.isGray = true;
              })
          }
          Button('Recover', { type: ButtonType.Capsule })
            .backgroundColor(Color.Blue)
            .width(120)
            .height(48)
            .fontSize(16)
            .fontWeight(500)
            .fontColor(this.btnFontColor)
            .margin({ right: 24 })
            .id('bRecover')
            .onClick(() => {
              this.isGray = false;
            })
        }
        .width('100%')
        .height(48)
        .alignItems(VerticalAlign.Center)
        .justifyContent(FlexAlign.SpaceBetween)
      }
      .width('100%')
    }
    .height('100%')
    .width('100%')
    .backgroundColor($r('app.color.background_shallow_grey'))
  }
}
