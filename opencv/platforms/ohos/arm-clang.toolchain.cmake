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

set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CXX_COMPILER_ID Clang)
SET ( CMAKE_SYSTEM_PROCESSOR arm )

SET ( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
SET ( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
SET ( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )

SET ( CROSS_COMPILATION_ARCHITECTURE armv7-a )

set(CMAKE_TOOLCHAIN_PREFIX llvm-)

set(MUSL_SYSROOT $ENV{OHOS_SYSROOT_DIR})
set(COMPILER_DIR $ENV{OHOS_COMPILER_DIR})

#指定c编译工具（确保工具链所在路径已经添加到了PATH环境变量中）和编译标志，使用clang编译时标志中必须指定--target，否则无法交叉编译。
set(CMAKE_C_COMPILER ${COMPILER_DIR}/clang)
set(CMAKE_C_FLAGS "--target=arm-linux-ohosmusl -D__clang__ -march=armv7-a  -mfloat-abi=softfp -mfpu=neon-vfpv4 -w")
#指定c++编译工具（确保工具链所在路径已经添加到了PATH环境变量中）和编译标志，必须指定--target，否则无法交叉编译。
set(CMAKE_CXX_COMPILER ${COMPILER_DIR}/clang++) 
set(CMAKE_CXX_FLAGS "--target=arm-linux-ohosmusl -D__clang__ -march=armv7-a  -mfloat-abi=softfp -mfpu=neon-vfpv4 -w")
#指定链接工具和链接标志，必须指定--target和--sysroot，其中OHOS_ROOT_PATH可通过cmake命令后缀参数来指定。
#set(MY_LINK_FLAGS "--target=arm-linux-ohosmusl --sysroot=/home/zhangsz/OpenHarmony_3.2_beta1/out/rk3568/obj/third_party/musl/")
set(CMAKE_LINKER ${COMPILER_DIR}/clang)
set(CMAKE_CXX_LINKER ${COMPILER_DIR}/clang++)
set(CMAKE_C_LINKER ${COMPILER_DIR}/clang)

#指定链接库的查找路径。
#set(CMAKE_SYSROOT /home/zhangsz/OpenHarmony_3.2_beta1/out/rk3568/obj/third_party/musl/)
set(CMAKE_SYSROOT ${MUSL_SYSROOT})
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--dynamic-linker,/system/bin/ld-musl-arm.so.1")
