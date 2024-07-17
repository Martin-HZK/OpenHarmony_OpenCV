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

# script code directory
script_path=$(cd $(dirname $0);pwd)
#echo "script_path:${script_path}"
# source code root directory
code_dir=$(dirname $(dirname ${script_path}))
echo "code_dir:${code_dir}"

#sdk安装可执行文件名
exec_name="ohsdkmgr"

ohos_sdk_path="${code_dir}/build/ohos_sdk"
if [ ! -d "${ohos_sdk_path}" ];then
    mkdir -p ${ohos_sdk_path}
fi

#删除已存在的解压后的文件目录
unzip_path="${code_dir}/build"
install_file=`find ${unzip_path} -name "${exec_name}" | sed -n 1p`
#echo "install_file:${install_file}"
if [ -f "${install_file}" ];then
    install_path=$(dirname $(dirname ${install_file}))
    #echo "install_path:${install_path}"
    rm -rf ${install_path}
    echo "unzip file:${install_path} already existed, remove it"
fi

sdk_zip_file="${code_dir}/build/ohcommandlinetools-linux-tool-1.0.0.400.zip"
for para in $*
do
    if [[ $para == --SDK_ZIP* ]];then
        sdk_zip_file=`echo ${para#*=}`
	#echo "sdk_zip_file: ${sdk_zip_file}"
    fi
done

if [ ! -f "${sdk_zip_file}" ];then
    echo "ERROR: OHOS SDK zip file[${sdk_zip_file}] not find, please check!"
    exit 1
fi

#解压sdk zip文件
unzip_cmd=`unzip ${sdk_zip_file} -d ${unzip_path}`
echo "${unzip_cmd}"

#获取安装文件路径
install_file=`find ${unzip_path} -name "${exec_name}" | sed -n 1p`
#echo "install_file:${install_file}"
install_cmd_path=$(cd $(dirname $install_file);pwd)
echo "cd sdk install directory: ${install_cmd_path}"
cd ${install_cmd_path}

echo "Start install ohos sdk"
#管道方式自动交互安装sdk
#echo "y" | ./ohsdkmgr install native --sdk-directory=${ohos_sdk_path}

# expect方式自动交互，expect嵌入安装sdk
/usr/bin/expect << EOF
set timeout -1
spawn ./${exec_name} install native --sdk-directory=${ohos_sdk_path}
expect {
    "*(y/n)*" { send "y\n";  exp_continue}
    eof
}
EOF

#查询交叉编译器clang和链接器musl路径
cd ${ohos_sdk_path}
sysroot=`find ${ohos_sdk_path} -name "sysroot" | sed -n 1p`
#musl_sysroot_dir="${ohos_sdk_path}/${sysroot#*/}"
musl_sysroot_dir="${sysroot}"
echo "sysroot dir: ${musl_sysroot_dir}"

compiler=`find ${ohos_sdk_path} -name "llvm" | sed -n 1p`
#compiler_dir="${ohos_sdk_path}/${compiler#*/}/bin"
compiler_dir="${compiler}/bin"
echo "compiler dir: ${compiler_dir}"

export OHOS_SYSROOT_DIR=${musl_sysroot_dir}
export OHOS_COMPILER_DIR=${compiler_dir}

cd ${code_dir}/build
echo "Start build opencv libs for OpenHarmony, please wait few moments!"
#cmake交叉编译opencv
build_cmd=`cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release -DWITH_CUDA=OFF -DWITH_FFMPEG=ON -DCMAKE_TOOLCHAIN_FILE=../platforms/ohos/arm-clang.toolchain.cmake .. && make -j 16`
echo "${build_cmd}"


