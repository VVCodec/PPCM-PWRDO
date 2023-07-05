HDRTools package
================
This software package is under development to support 
video standardization work and may eventually be formally 
published as reference software, e.g., by ITU-T and ISO/IEC. 

Please do not use this package for any other applications without 
the knowledge or permission from the software owners/creators.

Disclaimer:
===========
Please note that this software was developed mainly for research purposes, with 
the desire for portability, extensibility, and testing in the context of video
coding standards development. It was highly desirable to make this software 
independent from other "non-standard" libraries, such as libraries for I/O and
processing, to ease compilation and testing across multiple platforms. Instead, 
such functionality may be replicated in this software, while at the same time, 
it may be highly unoptimized or limited in scope. Optimized code, however was
never the intent of this tool. Optimization as well as extensions in scope for
some of this functionality may be added in the future as deemed necessary.

Build instructions:
===================
The Software has CMake support to create platform-specific build files. A working CMake installation is recommended for building the software.

CMake generates configuration files for the compiler environment/development environment on each platform. A list of examples is given in the following.

Create a build directory in the root directory:
**
mkdir build
cd build
**

Windows Visual Studio 2015 64 Bit:
**
cmake ..  -G "Visual Studio 14 2015 Win64"
**

macOS Xcode:
**
cmake .. -G "Xcode"
**

Linux:
**
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j
**

The binaries are being build in the binary tree. In case you want them to appear directly in the source tree set -DBINARIES_IN_SOURCETREE=1

For more details, refer to the CMake documentation: https://cmake.org/cmake/help/latest/
