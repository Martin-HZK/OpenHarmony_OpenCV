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
import shutil

def main():
    print('[opencv] start run srcript {}.'.format(sys.argv[0]))
    parser = argparse.ArgumentParser()
    parser.add_argument('--src_file', default="./.pre_build", help='prebuild header file.')
    parser.add_argument('--dest_file', default="./build", help='dest file.')
    args = parser.parse_args()
        
    if not os.path.exists(args.src_file):
        print('prebuild header folder[{}] not exist'.format(args.src_file))

    if os.path.exists(args.dest_file):
        shutil.rmtree(args.dest_file, True)
        
    shutil.copytree(args.src_file, args.dest_file)
    
 
if __name__ == '__main__':
    sys.exit(main())

