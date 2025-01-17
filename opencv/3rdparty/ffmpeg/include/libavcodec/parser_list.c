/*
# Copyright (c) 2022 Shenzhen Kaihong Digital Industry Development Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/

static const AVCodecParser * const parser_list[] = {
    &ff_aac_parser,
    &ff_aac_latm_parser,
    &ff_ac3_parser,
    &ff_adx_parser,
    &ff_av1_parser,
    &ff_avs2_parser,
    &ff_bmp_parser,
    &ff_cavsvideo_parser,
    &ff_cook_parser,
    &ff_dca_parser,
    &ff_dirac_parser,
    &ff_dnxhd_parser,
    &ff_dpx_parser,
    &ff_dvaudio_parser,
    &ff_dvbsub_parser,
    &ff_dvdsub_parser,
    &ff_dvd_nav_parser,
    &ff_flac_parser,
    &ff_g723_1_parser,
    &ff_g729_parser,
    &ff_gif_parser,
    &ff_gsm_parser,
    &ff_h261_parser,
    &ff_h263_parser,
    &ff_h264_parser,
    &ff_hevc_parser,
    &ff_jpeg2000_parser,
    &ff_mjpeg_parser,
    &ff_mlp_parser,
    &ff_mpeg4video_parser,
    &ff_mpegaudio_parser,
    &ff_mpegvideo_parser,
    &ff_opus_parser,
    &ff_png_parser,
    &ff_pnm_parser,
    &ff_rv30_parser,
    &ff_rv40_parser,
    &ff_sbc_parser,
    &ff_sipr_parser,
    &ff_tak_parser,
    &ff_vc1_parser,
    &ff_vorbis_parser,
    &ff_vp3_parser,
    &ff_vp8_parser,
    &ff_vp9_parser,
    &ff_webp_parser,
    &ff_xma_parser,
    NULL };
