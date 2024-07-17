#!/bin/bash
# Copyright (c) 2023 Huawei Device Co., Ltd.
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

#set -e

# script code directory
script_path=$(cd $(dirname $0);pwd)
#echo "script_path:${script_path}"
# source code root directory
code_dir=$(dirname $(dirname ${script_path}))
echo "code_dir:${code_dir}"

sha256_result=0
check_sha256=''
local_sha256=''
build_folder="build_ohos"
sdk_folder="ohos_sdk"
nativeFile_key="native-linux"

function check_sha256() {
    success_color='\033[1;42mSuccess\033[0m'
    failed_color='\033[1;41mFailed\033[0m'
    check_url=$1 # source URL
    local_file=$2  # local absolute path
    check_sha256=$(curl -s -k ${check_url}.sha256)
    local_sha256=$(sha256sum ${local_file} |awk '{print $1}')
    if [ "X${check_sha256}" == "X${local_sha256}" ];then
        echo -e "${success_color},${check_url} Sha256 check OK."
        sha256_result=0
    else
        echo -e "${failed_color},${check_url} Sha256 check Failed.Retry!"
        sha256_result=1
    fi
}

function check_sha256_by_mark() {
    success_color='\033[1;42mSuccess\033[0m'
    check_url=$1 # source URL
    check_sha256=$(curl -s -k ${check_url}.sha256)
    #echo $1
    if [ -f "${ohos_sdk_path}/${check_sha256}.mark" ];then
        echo -e "${success_color},${check_url} Sha256 markword check OK."
        sha256_result=0
    else
        echo -e "${check_url} Sha256 mismatch or files not downloaded yet."
        sha256_result=1
    fi
}

function hwcloud_download() {
    # when proxy certfication not required : wget -t3 -T10 -O ${bin_dir} -e "https_proxy=http://domain.com:port" ${huaweicloud_url}
    # when proxy certfication required (special char need URL translate, e.g '!' -> '%21'git
    # wget -t3 -T10 -O ${bin_dir} -e "https_proxy=http://username:password@domain.com:port" ${huaweicloud_url}

    download_local_file=$1
    download_source_url=$2
    for((i=1;i<=3;i++));
    do
        if [ -f "${download_local_file}" ];then
            check_sha256 "${download_source_url}" "${download_local_file}"
            if [ ${sha256_result} -gt 0 ];then
                rm -rf "${download_local_file:-/tmp/20210721_not_exit_file}"
                #echo "i=${i} sha256_result gt 0"
            else
                #echo "i=${i}"
                return 0
            fi
        fi
        if [ ! -f "${download_local_file}" ];then
            wget -t30 -T120 -O "${download_local_file}" "${download_source_url}"
        fi
    done
    # three times error, exit
    echo -e """Sha256 check failed!
Download URL: ${download_source_url}
Local file: ${download_local_file}
Remote sha256: ${check_sha256}
Local sha256: ${local_sha256}"""
    exit 1
}

#Get download url from input parameter
sdk_download_url=""
build_clean="false"
for para in $*
do
    if [[ $para == --download_url* ]];then
        sdk_download_url=`echo ${para#*=}`
	    echo "sdk_download_url: ${sdk_download_url}"
    elif [[ $para == --build_clean* ]];then
        build_clean=`echo ${para#*=}`
        echo "build_clean: ${build_clean}"
    fi
done

if [[ "${sdk_download_url}X" == "X" ]];then
    echo "Download url empty, please check!."
    exit 1
fi

#check download url wherether exist or not
result_str=`wget --spider ${sdk_download_url} 2>&1`
echo "result_str: $result_str"
test_result=$(echo $result_str | grep "Remote file exists")
if [[ "$test_result" == "" ]];then
    echo "Remote file not exists, please check for correctness of the download url."
    exit 1
fi

set -e

#Get download file name and suffix
bin_file=$(basename ${sdk_download_url})
echo "bin_file: ${bin_file}"
bin_file_suffix=${bin_file#*.}
echo "bin_file_suffix: ${bin_file_suffix}"

#create ohos sdk folder
ohos_sdk_path="${code_dir}/${build_folder}/${sdk_folder}"
if [ ! -d "${ohos_sdk_path}" ];then
    mkdir -p ${ohos_sdk_path}
else
    #rm ${ohos_sdk_path}/* -rf
    softfiles=$(ls $ohos_sdk_path)
    for sfile in ${softfiles}
    do
        if [ "$sfile" != "$bin_file" ];then 
            echo "Delete old sdk file: ${sfile}"
            rm $ohos_sdk_path/$sfile -rf
        fi
    done
fi

#download and decompress sdk file
check_sha256_by_mark ${sdk_download_url}
if [ ${sha256_result} -gt 0 ]; then
    hwcloud_download "${ohos_sdk_path}/${bin_file}"  "${sdk_download_url}"
    if [ "X${bin_file:0-3}" = "Xzip" ];then
        unzip -o "${ohos_sdk_path}/${bin_file}" -d "${ohos_sdk_path}"
    elif [ "X${bin_file:0-6}" = "Xtar.gz" ];then
        tar -xvzf "${ohos_sdk_path}/${bin_file}"  -C  "${ohos_sdk_path}"
    else
        tar -xvf "${ohos_sdk_path}/${bin_file}"  -C  "${ohos_sdk_path}"
    fi
fi

#unzip native-linux zipfile
zip_file=`find ${ohos_sdk_path} -name "${nativeFile_key}*" | sed -n 1p`
echo "zip_file:${zip_file}"
if [ -f "${zip_file}" ];then
    unzip ${zip_file} -d ${ohos_sdk_path}
else
    echo "nativ-linux zip file not found."
fi

#Find the paht of compiler clang and musl
cd ${ohos_sdk_path}
sysroot=`find ${ohos_sdk_path} -name "sysroot" | sed -n 1p`
musl_sysroot_dir="${sysroot}"
echo "sysroot dir: ${musl_sysroot_dir}"

compiler=`find ${ohos_sdk_path} -name "llvm" | sed -n 1p`
compiler_dir="${compiler}/bin"
echo "compiler dir: ${compiler_dir}"

export OHOS_SYSROOT_DIR=${musl_sysroot_dir}
export OHOS_COMPILER_DIR=${compiler_dir}

cd ${code_dir}/${build_folder}
#cmake clean
if [ "X$build_clean" == "Xtrue" ];then
    echo "Clean opencv build"
    build_cmd=`cmake clean ..`
    echo -e "${build_cmd}"
    build_cmd=`make clean`
    echo -e "${build_cmd}"
fi

#cmake cross compile opencv
echo "Start build opencv for OpenHarmony, please wait few moments!"
build_cmd=`cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release -DWITH_CUDA=OFF -DWITH_FFMPEG=ON -DCMAKE_TOOLCHAIN_FILE=../platforms/ohos/arm-clang.toolchain.cmake .. && make -j 16`
echo -e "${build_cmd}"
