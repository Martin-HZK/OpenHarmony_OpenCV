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

import { ThirdLevelCategory } from '../../model/CategoricalDataType'

const IMAGE_BASIC: ThirdLevelCategory =
  {
    image: $r('app.media.ic_button_and_selection'),
    title: $r('app.string.image_basic_title'),
    childNodes: [
      {
        title: $r('app.string.image_color_title'),
        url: 'pages/image/basicSample/image2Gray'
      },
      {
        title: $r('app.string.image_edge_detection'),
        url: 'pages/image/basicSample/edgeDetection'
      }

    ]
  }

const IMAGE_HISTOGRAM: ThirdLevelCategory =
  {
    image: $r('app.media.ic_information_presentation_and_dynamic_effects'),
    title: $r('app.string.image_histogram_title'),
    childNodes: [
      {
        title: $r('app.string.image_histogram_title'),
        url: 'pages/image/basicSample/image2Gray'
      }

    ]
  }
const IMAGE_FILTER: ThirdLevelCategory =
  {
    image: $r('app.media.ic_text_and_input'),
    title: $r('app.string.image_filter_title'),
    childNodes: [
      {
        title: $r('app.string.image_filter_title'),
        url: 'pages/image/basicSample/image2Gray'
      }

    ]
  }
const IMAGE_MORPHOLOGY: ThirdLevelCategory =
  {
    image: $r('app.media.ic_rows_and_columns'),
    title: $r('app.string.image_morphology_title'),
    childNodes: [
      {
        title: $r('app.string.image_morphology_title'),
        url: 'pages/image/basicSample/image2Gray'
      }

    ]
  }
const IMAGE_OBJDETECT: ThirdLevelCategory =
  {
    image: $r('app.media.ic_stack_flex_and_grid'),
    title: $r('app.string.image_objdetect_title'),
    childNodes: [
      {
        title: $r('app.string.opencv_qrcode'),
        url: 'pages/image/basicSample/qrcode'
      },
      {
        title: $r('app.string.image_objdetect_title'),
        url: 'pages/image/basicSample/faceDetect'
      }
    ]
  }

export const IMAGE_CATEGORIES: ThirdLevelCategory[] =
  [
    IMAGE_BASIC,
    IMAGE_HISTOGRAM,
    IMAGE_FILTER,
    IMAGE_MORPHOLOGY,
    IMAGE_OBJDETECT
  ]
// 辅助/AlphabetIndexerSampleData
export let alphabetIndexerDataB: Resource[] = [$r('app.string.alphabetindexer_bu'), $r('app.string.alphabetindexer_bai'), $r('app.string.alphabetindexer_bao'), $r('app.string.alphabetindexer_bi'), $r('app.string.alphabetindexer_bing')]

export let alphabetIndexerDataC: Resource[] = [$r('app.string.alphabetindexer_cao'), $r('app.string.alphabetindexer_cheng'), $r('app.string.alphabetindexer_chen'), $r('app.string.alphabetindexer_cui')]

export let alphabetIndexerDataL: Resource[] = [$r('app.string.alphabetindexer_liu'), $r('app.string.alphabetindexer_li'), $r('app.string.alphabetindexer_lou'), $r('app.string.alphabetindexer_liang'), $r('app.string.alphabetindexer_lei'), $r('app.string.alphabetindexer_lv'), $r('app.string.alphabetindexer_liu3'), $r('app.string.alphabetindexer_lu')]

export let letterValue: string[] = ['#', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
  'H', 'I', 'J', 'K', 'L', 'M', 'N',
  'O', 'P', 'Q', 'R', 'S', 'T', 'U',
  'V', 'W', 'X', 'Y', 'Z']
// 辅助&按钮与选择/panel&textPickerArrayData
export let fruitArray: Resource[] = [$r('app.string.checkbox_apple'), $r('app.string.checkbox_banbana'), $r('app.string.checkbox_cherries'), $r('app.string.checkbox_durian')]

// 信息展示与动效/dataPanelSample
export let panelData: number[] = [10, 10, 30, 10, 10, 20]
// 文本与输入/spanSelectData
export let selectData: Resource[] = [$r('app.string.span_decoration_none'), $r('app.string.span_decoration_linethrough'), $r('app.string.span_decoration_underline'), $r('app.string.span_decoration_overline')]
// 三级类别 Scroll 通用 left padding
export const LEFT_PADDING: number = 12
// 三级类别 Scroll 通用 right padding
export const RIGHT_PADDING: number = 12
// 行列与分栏/rowAlignItemsData
export const ROW_ALIGNITEMS_DATA: Resource[] = [$r('app.string.row_alignitems_top'), $r('app.string.row_alignitems_center'), $r('app.string.row_alignitems_bottom')]
// 行列与分栏/rowJustifyContentData
export const ROW_JUSTIFYCONTENT_DATA: Resource[] = [$r('app.string.row_justifycontent_start'), $r('app.string.row_justifycontent_center'), $r('app.string.row_justifycontent_end'),
$r('app.string.row_justifycontent_spacebetween'), $r('app.string.row_justifycontent_spacearound'), $r('app.string.row_justifycontent_spaceevenly')]
// 行列与分栏/columnAlignItemsData
export const COLUMN_ALIGNITEMS_DATA: Resource[] = [$r('app.string.column_alignitems_start'), $r('app.string.column_alignitems_center'), $r('app.string.column_alignitems_end')]
// 行列与分栏/columnJustifyContentData
export const COLUMN_JUSTIFYCONTENT_DATA: Resource[] = [$r('app.string.column_justifycontent_start'), $r('app.string.column_justifycontent_center'), $r('app.string.column_justifycontent_end'),
$r('app.string.column_justifycontent_spacebetween'), $r('app.string.column_justifycontent_spacearound'), $r('app.string.column_justifycontent_spaceevenly')]
// 行列与分栏/sidebarContainerTypeData
export const SBC_TYPE_DATA: Resource[] = [$r('app.string.sidebarcontainer_type_embed'), $r('app.string.sidebarcontainer_type_overlay')]
// 行列与分栏/sidebarContainerPositionData
export const SBC_POSITION_DATA: Resource[] = [$r('app.string.sidebarcontainer_sidebar_show_position_start'), $r('app.string.sidebarcontainer_sidebar_show_position_end')]
// 堆叠、Flex与栅格/stackArgsSelectData
export const STACK_ARGUMENTS_SELECT_DATA: Resource[] = [$r('app.string.stack_alignments_topstart'),$r('app.string.stack_alignments_top'),
$r('app.string.stack_alignments_topend'),$r('app.string.stack_alignments_start'),$r('app.string.stack_alignment_center'),$r('app.string.stack_alignment_end'),
$r('app.string.stack_alignments_bottomstart'),$r('app.string.stack_alignments_bottom'),$r('app.string.stack_alignments_bottomend')]
// 堆叠、Flex与栅格/stackZIndexSelectData
export const STACK_ZIND_SELECT_DATA: Resource[] = [$r('app.string.stack_zindex_four'),$r('app.string.stack_zindex_one')]
// 堆叠、Flex与栅格/flexDirectionSelectData
export const FLEX_DIRECTION_SELECT_DATA: Resource[] = [$r('app.string.flex_argument_direction_row'),$r('app.string.flex_argument_direction_row_reverse'),
$r('app.string.flex_argument_direction_column'),$r('app.string.flex_argument_direction_column_reverse')]
// 堆叠、Flex与栅格/flexWrapSelectData
export const FLEX_WRAP_SELECT_DATA : Resource[] = [$r('app.string.flex_argument_wrap_no_wrap'),$r('app.string.flex_argument_wrap_wrap'),$r('app.string.flex_argument_wrap_wrap_reverse')]
// 堆叠、Flex与栅格/flexJustifyContentSelectData
export const FLEX_JUSTIFY_CONTENT_SELECT_DATA : Resource[] = [$r('app.string.flex_argument_justifycontent_start'),$r('app.string.flex_argument_justifycontent_center'),$r('app.string.flex_argument_justifycontent_End'),
$r('app.string.flex_argument_justifycontent_space_between'),$r('app.string.flex_argument_justifycontent_space_around'),$r('app.string.flex_argument_justifycontent_speac_evenly')]
// 堆叠、Flex与栅格/flexAlignItemsSelectData
export const FLEX_ALIGN_ITEMS_SELECT_DATA : Resource[] = [$r('app.string.flex_argumen_alignItems_auto'),$r('app.string.flex_argumen_alignItems_start'),$r('app.string.flex_argumen_alignItems_center'),
$r('app.string.flex_argumen_alignItems_end'),$r('app.string.flex_argumen_alignItems_stretch'),$r('app.string.flex_argumen_alignItems_baseline')]
// 堆叠、Flex与栅格/gridContainerSizeTypeSelectData
export const GRIDCONTAIER_SIZE_TYPE_SELECT_DATA : Resource[] = [$r('app.string.gridcontainer_sizetype_xs'),$r('app.string.gridcontainer_sizetype_sm'),$r('app.string.gridcontainer_sizetype_md'),$r('app.string.gridcontainer_sizetype_lg'),$r('app.string.gridcontainer_sizetype_auto'),]
//图片、视频与媒体/imageSelectObjectFit
export const OBJECT_FIT_SELECT: Resource[] = [$r('app.string.object_fit_none'),$r('app.string.object_fit_cover'),$r('app.string.object_fit_contain'),$r('app.string.object_fit_fill'),$r('app.string.object_fit_scale_down'),$r('app.string.object_fit_auto')]
//图片、视频与媒体/imageSelectRenderMode
export const RENDER_MODE_SELECT: Resource[] = [$r('app.string.image_rendermode_original'),$r('app.string.image_rendermode_template')]
//图片、视频与媒体/imageSelectRepeat
export const REPEAT_SELECT: Resource[] = [$r('app.string.image_repeat_no'),$r('app.string.image_repeat_x'),$r('app.string.image_repeat_y'),$r('app.string.image_repeat_xy')]
// 图片、视频与媒体/videoSelectCurrentRate
export const CURRENT_RATE: string[] = ['0.75X','1X','1.25X','1.75X','2X']
// 列表与宫格/listDirectionSelectData
export const LIST_DIRECTION_SELECT_DATA : Resource[] = [$r('app.string.list_properties_direction_vertical'),$r('app.string.list_properties_direction_Horizontal')]
// 列表与宫格/listScrollBarSelectData
export const LIST_SCROLLBAR_SELECT_DATA : Resource[] = [$r('app.string.list_properties_scroll_bar_off'),$r('app.string.list_properties_scroll_bar_on'),$r('app.string.list_properties_scroll_bar_auto')]
// 列表与宫格/listEdgeEffectSelectData
export const LIST_EDGE_EFFECT_SELECT_DATA : Resource[] = [$r('app.string.list_properties_edge_effect_spring'),$r('app.string.list_properties_edge_effect_fade'),$r('app.string.list_properties_edge_effect_none')]
// 列表与宫格/listAlignListItemSelectData
export const LIST_ALIGN_LIST_ITEM_SELECT_DATA : Resource[] =[$r('app.string.list_properties_align_list_item_start'),$r('app.string.list_properties_align_list_item_Center'),$r('app.string.list_properties_align_list_item_End')]
// 列表与宫格/gridLayoutDirectionSelectData
export const GRID_LAYOUT_DIRECTION_SELECT : Resource[] =[$r('app.string.grid_properties_layout_direction_row'),$r('app.string.grid_properties_layout_direction_column')
  ,$r('app.string.grid_properties_layout_direction_row_reverse'),$r('app.string.grid_properties_layout_direction_column_reverse')]
// 导航/tabsBarPositionSelectData
export const TABS_BAR_POSITION_SELECT : Resource[] = [$r('app.string.tabs_arguments_bar_position_start'),$r('app.string.tabs_arguments_bar_position_end')]
// 导航/tabsBarModeSelectData
export const TABS_BAR_MODE_SELECT : Resource[] = [$r('app.string.tabs_properties_bar_mode_fixed'),$r('app.string.tabs_properties_bar_scrollable')]
//画布与图形绘制/PathSelectCommands
export const PATH_SELECT_COMMANDS: Resource[] = [$r('app.string.path_line'),$r('app.string.path_triangle'),$r('app.string.path_square'),$r('app.string.path_polygon')
  ,$r('app.string.path_camber_first'),$r('app.string.path_camber_second'),$r('app.string.path_camber_third')]
