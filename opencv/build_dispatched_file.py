#!/usr/bin/env python

# Copyright (c) 2022 Kaihongdigi Limited,
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

import os
import sys
import argparse
    
def ocv_add_dispatched_file(src_path, build_path, file_name, optimizations, target_cpu, precomp):
    if target_cpu == "arm" or target_cpu == "arm64":
        cpu_dispatch_list = []
    else:
        cpu_dispatch_list = ['SSE4_1', 'SSE4_2', 'FP16', 'AVX', 'AVX2', 'AVX512_SKX']

    #precomp = precomp
    codestr = "\n" + "#include \"" + src_path + "/" + precomp + "\"\n" + "#include \"" + src_path + "/" + file_name + ".simd.hpp\""
    #print('codestr=', codestr)
    declarations_str = "#define CV_CPU_SIMD_FILENAME \"" + src_path + "/" + file_name + ".simd.hpp\""
    #print('declarations_str=', declarations_str)
    dispatch_modes = "BASELINE"
    opt_list = optimizations.split(" ")
    for opt in opt_list:
        opt_lower = opt.lower()
        cpp_file = os.path.join(build_path, file_name+"."+opt_lower+".cpp")
        #print('cpp_file=', cpp_file)
        fp = open(cpp_file, 'w')
        fp.write(codestr)
        fp.close()

        #if opt in cpu_dispatch_list:
        if precomp == 'test_precomp.hpp':
            declarations_str = declarations_str + "\n" + "#define CV_CPU_DISPATCH_MODE " + opt + "\n" + "#include \"opencv2/core/private/cv_cpu_include_simd_declarations.hpp\""
            dispatch_modes = opt + ", " + dispatch_modes

    
    #print('dispatch_modes=', dispatch_modes)
    
    declarations_str = declarations_str + "\n" + "#define CV_CPU_DISPATCH_MODES_ALL " + dispatch_modes + "\n\n" + "#undef CV_CPU_SIMD_FILENAME"
    #print('Final declarations_str=', declarations_str)

    simd_declarations_hpp_file = os.path.join(build_path, file_name+".simd_declarations.hpp")
    #print('simd_declarations_hpp_file=', simd_declarations_hpp_file)
    fp_declr = open(simd_declarations_hpp_file, 'w')
    fp_declr.write(declarations_str)
    fp_declr.close()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--src_path', help='path of src file.')
    parser.add_argument('--save_path', help='path of build dispatch file.')
    parser.add_argument('--file_name', help='file name of dispatched.')
    parser.add_argument('--optimizations', help='Compiler optimization list.')
    parser.add_argument('--target_cpu', help='target cpu type.', default='arm')
    parser.add_argument('--precomp', help='precomp name.', default='precomp.hpp')
    args = parser.parse_args()
    build_path = args.save_path
    if not os.path.isabs(args.save_path):
        build_path = os.path.abspath(args.save_path)
    
    if not os.path.exists(build_path):
        os.makedirs(build_path)

    src_path = args.src_path
    if not os.path.isabs(args.src_path):
        src_path = os.path.abspath(args.src_path)

    ocv_add_dispatched_file(src_path, build_path, args.file_name, args.optimizations, args.target_cpu, args.precomp)
 

if __name__ == '__main__':
    sys.exit(main())

