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

static const URLProtocol * const url_protocols[] = {
    &ff_async_protocol,
    &ff_cache_protocol,
    &ff_concat_protocol,
    &ff_crypto_protocol,
    &ff_data_protocol,
    &ff_ffrtmphttp_protocol,
    &ff_file_protocol,
    &ff_ftp_protocol,
    &ff_gopher_protocol,
    &ff_hls_protocol,
    &ff_http_protocol,
    &ff_httpproxy_protocol,
    &ff_icecast_protocol,
    &ff_mmsh_protocol,
    &ff_mmst_protocol,
    &ff_md5_protocol,
    &ff_pipe_protocol,
    &ff_prompeg_protocol,
    &ff_rtmp_protocol,
    &ff_rtmpt_protocol,
    &ff_rtp_protocol,
    &ff_srtp_protocol,
    &ff_subfile_protocol,
    &ff_tee_protocol,
    &ff_tcp_protocol,
    &ff_udp_protocol,
    &ff_udplite_protocol,
    &ff_unix_protocol,
    NULL };
