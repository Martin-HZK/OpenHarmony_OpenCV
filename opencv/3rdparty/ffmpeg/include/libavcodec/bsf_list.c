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

static const AVBitStreamFilter * const bitstream_filters[] = {
    &ff_aac_adtstoasc_bsf,
    &ff_av1_frame_merge_bsf,
    &ff_av1_frame_split_bsf,
    &ff_av1_metadata_bsf,
    &ff_chomp_bsf,
    &ff_dump_extradata_bsf,
    &ff_dca_core_bsf,
    &ff_eac3_core_bsf,
    &ff_extract_extradata_bsf,
    &ff_filter_units_bsf,
    &ff_h264_metadata_bsf,
    &ff_h264_mp4toannexb_bsf,
    &ff_h264_redundant_pps_bsf,
    &ff_hapqa_extract_bsf,
    &ff_hevc_metadata_bsf,
    &ff_hevc_mp4toannexb_bsf,
    &ff_imx_dump_header_bsf,
    &ff_mjpeg2jpeg_bsf,
    &ff_mjpega_dump_header_bsf,
    &ff_mp3_header_decompress_bsf,
    &ff_mpeg2_metadata_bsf,
    &ff_mpeg4_unpack_bframes_bsf,
    &ff_mov2textsub_bsf,
    &ff_noise_bsf,
    &ff_null_bsf,
    &ff_opus_metadata_bsf,
    &ff_pcm_rechunk_bsf,
    &ff_prores_metadata_bsf,
    &ff_remove_extradata_bsf,
    &ff_text2movsub_bsf,
    &ff_trace_headers_bsf,
    &ff_truehd_core_bsf,
    &ff_vp9_metadata_bsf,
    &ff_vp9_raw_reorder_bsf,
    &ff_vp9_superframe_bsf,
    &ff_vp9_superframe_split_bsf,
    NULL };
