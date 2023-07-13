cmake_minimum_required(VERSION 3.25)
project(GoProSurfSplitter)

set(CMAKE_CXX_STANDARD 17)

find_package(cxxopts REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
        libavformat
        libavcodec
        libswresample
        libswscale
        libavutil
        )

include_directories(third_party/gpmf-parser)

add_executable(GoProSurfSplitter
        third_party/gpmf-parser/demo/GPMF_mp4reader.c
        third_party/gpmf-parser/GPMF_parser.c
        main.cpp
        GpsInfo.h Detector.h Video.h)

target_link_libraries(GoProSurfSplitter
        PkgConfig::LIBAV
        )