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
import ShowToast from '../../../util/ShowToast';
import opencv from 'libohos_opencv_sample.so';
import { TitleBar } from '../../../common/TitleBar'

const TAG: string = 'opencv_qrcode';

@Entry
@Component
struct Image2Gray {
  private btnFontColor: Resource = $r('app.color.white');
  @State isDetected: Boolean = false;
  @State qrcodeStr: string | undefined = "";
  @State isInIdentifing: Boolean = false;
  private fileName: string = 'qrcode.jpg';
  private webUrl: string = 'pages/image/basicSample/qrcode_result_web';

  build() {
    Column() {
      // 标题
      TitleBar({ title: $r('app.string.opencv_qrcode') })
      // 二维码图片
      Column() {
        Column() {
          Image($rawfile(this.fileName))
            .margin({ left: 24, right: 24 })
            .objectFit(ImageFit.Contain)
            .id('backBtn')
        }
        .width('100%')
        .height(432)
        .alignItems(HorizontalAlign.Center)
        .justifyContent(FlexAlign.Start)

        // 识别按钮
        Column() {
          if (this.isInIdentifing) {
            Button($r('app.string.qrcode_identification'), { type: ButtonType.Capsule })
              .backgroundColor(Color.Gray)
              .width('100%')
              .fontSize(16)
              .fontWeight(500)
              .fontColor(this.btnFontColor)
              .enabled(false)
          } else {
            Button($r('app.string.qrcode_identification'), { type: ButtonType.Capsule })
              .backgroundColor(Color.Blue)
              .width('100%')
              .fontSize(16)
              .fontWeight(500)
              .fontColor(this.btnFontColor)
              .id('imageGray')
              .onClick(() => {
                this.isInIdentifing = true;
                opencv.qrcode(getContext().resourceManager, '', this.fileName)
                  .then((qrcodeInfo: opencv.Qrcode) => {
                    Logger.info(TAG, `qrcodeInfo detected: ${qrcodeInfo.detected}`);
                    if (qrcodeInfo.detected) {
                      this.qrcodeStr = qrcodeInfo.decodes[0];
                      Logger.info(TAG, `qrcodeInfo url: ${this.qrcodeStr}`);
                      this.isDetected = true;
                    } else {
                      this.isDetected = false;
                      ShowToast.longToast($r('app.string.qrcode_error_prompt'))
                    }
                  })
              })
          }
        }
        .margin({ left: 24, right: 24, top: 16 })
        .height(48)
        .alignItems(HorizontalAlign.Center)
        .justifyContent(FlexAlign.Center)

        if (this.isDetected) {
          Column() {
            Text($r('app.string.qrcode_prompt'))
              .fontSize(14)
              .layoutWeight(1)
              .fontWeight(400)
              .width('100%')
              .align(Alignment.Start)
          }
          .margin({ left: 24, right: 24, top: 12 })
          .height(40)
          .alignItems(HorizontalAlign.Center)
          .justifyContent(FlexAlign.Center)

          Column() {
            Text(this.qrcodeStr)
              .fontSize(16)
              .layoutWeight(1)
              .fontWeight(500)
              .padding({ left: 12, right: 12 })
              .width('100%')
              .align(Alignment.Start)
          }
          .margin({ left: 12, right: 12 })
          .height(56)
          .alignItems(HorizontalAlign.Center)
          .justifyContent(FlexAlign.Center)
          .backgroundColor(this.btnFontColor)
          .borderRadius(20)
          .onClick(() => {
            router.pushUrl({
              url: this.webUrl,
              params: {
                webAddr: this.qrcodeStr
              }
            })
          })
        }
      }
      .width('100%')
      .alignItems(HorizontalAlign.Center)
      .justifyContent(FlexAlign.Start)
    }
    .height('100%')
    .width('100%')
    .backgroundColor($r('app.color.background_shallow_grey'))
  }
}
