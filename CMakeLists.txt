cmake_minimum_required(VERSION 3.25)
project(GoProSurf)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)

if(WIN32)
    set(ICON_FILE ${CMAKE_SOURCE_DIR}/.win/icon.ico)
    set(WIN_RC_FILE ${CMAKE_SOURCE_DIR}/.win/resource.rc)
    set(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif()

find_package(Qt6 COMPONENTS Core Charts Gui Widgets Multimedia MultimediaWidgets REQUIRED)

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
#        libavformat
#        libavcodec
#        libswresample
#        libswscale
#        libavutil
#        )

include_directories(third_party/gpmf-parser)

set(GOPROSURF_SOURCES
        third_party/gpmf-parser/demo/GPMF_mp4reader.c
        third_party/gpmf-parser/GPMF_parser.c
        GpsInfo.h Detector.h Video.h QGoProFiles.h
        QTWindow.h QWavesModel.h QVersion.h
        )

if (WIN32)
    add_executable(GoProSurf WIN32 main.cpp ${WIN_RC_FILE})
else ()
    add_executable(GoProSurf main.cpp)
endif ()

target_sources(GoProSurf PRIVATE ${GOPROSURF_SOURCES})

target_link_libraries(GoProSurf
        PRIVATE Qt6::Core Qt6::Charts Qt6::Gui Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets
        # PkgConfig::LIBAV
        )