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
import web_webview from '@ohos.web.webview';
import Logger from '../../../util/Logger';
import ShowToast from '../../../util/ShowToast';
import opencv from 'libohos_opencv_sample.so';
import { TitleBar } from '../../../common/TitleBar'
import { IntroductionTitle } from '../../../common/IntroductionTitle';

const TAG: string = 'opencv_qrcode';
interface paramsTape {
  webAddr: string
}

@Extend(Column) function cardStyle(){
  .backgroundColor($r('app.color.white'))
  .borderRadius(16)
  .width('100%')
  .padding(12)
}

@Entry
@Component
struct Image2Gray {
  controller: web_webview.WebviewController = new web_webview.WebviewController();
  @State webAddr: string = (router.getParams() as paramsTape).webAddr;

  build() {
    Column() {
      // 标题
      TitleBar({ title: $r('app.string.web_title') })
      // 网页内容
      Scroll() {
        Column() {
          //IntroductionTitle({ introduction: $r('app.string.web_show') })
          Text(this.webAddr)
            .fontSize(18)
            .width('100%')
            .direction(Direction.Ltr)
            .margin({ left: 26, top: 8, bottom: 8 })
            .fontColor($r('app.color.font_color_shallow'))
          Column() {
            Web({ src: this.webAddr, controller: this.controller })
              .size({ width: '100%', height: '100%' })
              .domStorageAccess(true)
              .onPageBegin(() => {
                ShowToast.shortToast('onPageBegin')
              })
              .onPageEnd(() => {
                ShowToast.shortToast('onPageEnd')
              })
              .onRefreshAccessedHistory(() => {
                ShowToast.shortToast('The web is opening!')
              })
              .onScaleChange(() => {
                ShowToast.shortToast('scaleChange')
              })
          }.cardStyle()
        }
      }
      .padding({ left: 12, right: 12 })
      .margin({ bottom: 100 })
    }
    .height('100%')
    .width('100%')
    .backgroundColor($r('app.color.background_shallow_grey'))
  }
}
