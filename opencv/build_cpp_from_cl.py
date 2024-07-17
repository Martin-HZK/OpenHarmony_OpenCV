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
from hashlib import md5
import re

def ocv_cl_to_cpp(cl_path, build_path, module_name, target_cpu):
    if not os.path.exists(cl_path):
        print('cl_path:{%s} is not exist'.format(cl_path))
        return 1
    file_list = os.listdir(cl_path)
    #print('file_list=', file_list)
    cl_list = []
    for file in file_list:
        if os.path.splitext(file)[1] == ".cl":
            full_file = os.path.join(cl_path, file)
            cl_list.append(full_file)
    #print('cl_list=', cl_list)
    cl_list.sort()
    #print('After sort cl_list=', cl_list)
    output_cpp = os.path.join(build_path, "opencl_kernels_" + module_name + ".cpp")
    output_hpp = os.path.join(build_path, "opencl_kernels_" + module_name + ".hpp")
    OUTPUT_HPP_NAME = output_hpp.split("/")[-1]
    #print('output_cpp={}, output_hpp={}'.format(output_cpp, output_hpp))

    nested_namespace_start = "namespace " + module_name + "\n{"
    nested_namespace_end = "}"
    str_cpp = "// This file is auto-generated. Do not edit!" + "\n\n" + \
              "#include \"opencv2/core.hpp\"\n" + \
              "#include \"cvconfig.h\"\n" + \
              "#include \"" + OUTPUT_HPP_NAME + "\"\n\n" + \
              "#ifdef HAVE_OPENCL\n\n" + \
              "namespace cv\n" + "{\n" + \
              "namespace ocl\n" + "{\n" + \
              nested_namespace_start + \
              "\n\nstatic const char* const moduleName = \"" + module_name + "\";\n\n"

    str_hpp = "// This file is auto-generated. Do not edit!\n\n" + \
              "#include \"opencv2/core/ocl.hpp\"\n" + \
              "#include \"opencv2/core/ocl_genbase.hpp\"\n" + \
              "#include \"opencv2/core/opencl/ocl_defs.hpp\"\n\n" + \
              "#ifdef HAVE_OPENCL\n\n" + \
              "namespace cv\n" + "{\n" + \
              "namespace ocl\n" + "{\n" + \
              nested_namespace_start + "\n\n"

    for cl in cl_list:
        cl_filename = cl.split("/")[-1].split(".")[-2]

        file_object = open(cl, 'r')
        lines = file_object.read()
        lines = lines.replace("\r", "")
        lines = lines.replace("\t", "  ")
        if not lines.endswith("\n"):
            lines = lines + "\n"
        
        #print('&&&& 1 cl_filename = {} lines = {}'.format(cl_filename, lines))
        lines = re.sub("/\\*([^*]/|\\*[^/]|[^*/])*\\*/", "", lines) # multiline comments
        lines = re.sub( "/\\*([^\n])*\\*/", "", lines) # single-line comments
        lines = re.sub( "[ ]*//[^\n]*\n", "\n", lines) # single-line comments
        lines = re.sub( "\n[ ]*(\n[ ]*)*", "\n", lines) # empty lines & leading whitespace
        lines = re.sub( "^\n", "", lines) # leading new line

        #print('&&&& 2 cl_filename = {} lines = {}'.format(cl_filename, lines))

        lines = lines.replace("\\", "\\\\")
        #print('&&&& 3.1 cl_filename = {} lines = {}'.format(cl_filename, lines))
        lines = lines.replace("\"", "\\\"")
        #print('&&&& 3.2 cl_filename = {} lines = {}'.format(cl_filename, lines))
        lines = lines.replace("\n", "\\n\"\n\"")
        #print('&&&& 3.3 cl_filename = {} lines = {}'.format(cl_filename, lines))
        
        lines = re.sub( "\"$", "", lines) # unneeded " at the eof
        #print('&&&& 4 cl_filename = {} lines = {}'.format(cl_filename, lines))
        
        m = md5()
        m.update(lines.encode('utf-8'))
        hash = m.hexdigest()
        #print('&&&& 5 cl_filename = {} hash = {}'.format(cl_filename, hash))

        str_cpp_decl = "struct cv::ocl::internal::ProgramEntry " + cl_filename + "_oclsrc={moduleName, \"" + cl_filename + "\",\n\"" + lines + ", \"" + hash + "\", NULL};\n"
        str_hpp_decl = "extern struct cv::ocl::internal::ProgramEntry " + cl_filename + "_oclsrc;\n"

        str_cpp = str_cpp + str_cpp_decl
        str_hpp = str_hpp + str_hpp_decl

        file_object.close()
    
    str_cpp = str_cpp + "\n" + nested_namespace_end + "}}\n#endif\n"
    str_hpp = str_hpp + "\n" + nested_namespace_end + "}}\n#endif\n"
    
    fp = open(output_cpp, 'w')
    fp.write(str_cpp)
    fp.close()

    fp = open(output_hpp, 'w')
    fp.write(str_hpp)
    fp.close()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--cl_path', help='path of cl files.')
    parser.add_argument('--save_path', help='path of build dispatch file.')
    parser.add_argument('--module_name', help='module name.')
    parser.add_argument('--target_cpu', help='target cpu type.')
    args = parser.parse_args()

    build_path = args.save_path
    if not os.path.isabs(args.save_path):
        build_path = os.path.abspath(args.save_path)
    
    if not os.path.exists(build_path):
        os.makedirs(build_path)

    cl_path = args.cl_path
    if not os.path.isabs(args.cl_path):
        cl_path = os.path.abspath(args.cl_path)

    ocv_cl_to_cpp(cl_path, build_path, args.module_name, args.target_cpu)
 
if __name__ == '__main__':
    sys.exit(main())

