#!/bin/bash
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

set -e

# sync code directory
script_path=$(cd $(dirname $0);pwd)
#echo "script_path:${script_path}"
code_dir=$(dirname $(dirname ${script_path}))
#echo "code_dir:${code_dir}"

musl_sysroot=""
complier_dir=""
for para in $*
do
    if [[ $para == --musl_sysroot* ]];then
        musl_sysroot=`echo ${para#*=}`
    elif [[ $para == --complier_dir* ]];then
        complier_dir=`echo ${para#*=}`
	fi
done

if [ "X${musl_sysroot}" != "X" ];then
    export OHOS_SYSROOT_DIR=${musl_sysroot}
fi
if [ "X${complier_dir}" != "X" ];then
    export OHOS_COMPILER_DIR=${complier_dir}
fi

if [ "X${OHOS_SYSROOT_DIR}" == "X" ];then
    echo "Please config environment variable \"OHOS_SYSROOT_DIR\"!"
    exit 1
fi

if [ "X${OHOS_COMPILER_DIR}" == "X" ];then
    echo "Please config environment variable \"OHOS_COMPILER_DIR\"!"
    exit 1
fi
echo "OHOS_SYSROOT_DIR: ${OHOS_SYSROOT_DIR}"
echo "OHOS_COMPILER_DIR: ${OHOS_COMPILER_DIR}"

if [ ! -d ${code_dir}/build ];then
    mkdir -p ${code_dir}/build
fi
#cmake build and make
cd ${code_dir}/build
echo "Start build opencv libs for OpenHarmony, please wait some seconds!"

build_cmd=`cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release -DWITH_CUDA=OFF -DWITH_FFMPEG=ON -DCMAKE_TOOLCHAIN_FILE=../platforms/ohos/arm-clang.toolchain.cmake .. && make -j 16`
echo "${build_cmd}"


