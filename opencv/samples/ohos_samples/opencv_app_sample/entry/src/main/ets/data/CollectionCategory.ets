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

import { MACHINE_LEARNING_CATEGORIES } from '../pages/machineLearning/MachineLearningData'
import { IMAGE_CATEGORIES } from '../pages/image/ImageData'
import { FirstLevelCategory } from '../model/CategoricalDataType'
import { VIDEO_CATEGORIES } from '../pages/video/VideoData'

// The component collection classifies the data as a whole
export const OPENCV_SAMPLE_CATEGORIES: FirstLevelCategory[] =
  [
    {
      childNodes: IMAGE_CATEGORIES,
      selectedImage: $r('app.media.ic_select_component'),
      unselectedImage: $r('app.media.ic_unselect_component'),
      tabBarName: $r('app.string.opencv_image')
    },
    {
      childNodes: VIDEO_CATEGORIES,
      selectedImage: $r('app.media.ic_select_universal'),
      unselectedImage: $r('app.media.ic_unselect_universal'),
      tabBarName: $r('app.string.opencv_video')
    },
    {
      childNodes: MACHINE_LEARNING_CATEGORIES,
      selectedImage: $r('app.media.ic_select_animation'),
      unselectedImage: $r('app.media.ic_unselect_animation'),
      tabBarName: $r('app.string.opencv_machine')
    }
  ]

export const fruitArray: Resource[] = []