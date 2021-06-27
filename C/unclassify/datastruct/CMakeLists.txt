#################
#
#
##################
cmake_minimum_required (VERSION 2.8)

project(datastruct)
include_directories(./include)
aux_source_directory(./source SRCS)

# 生成链接库
# 默认生成 lib
# windows 下编译
# cmake .. -G "Visual Studio 16 2019" -A "x64"  -DTARGET_TYPE:string=exe 
#cmake -DTARGET_TYPE:STRING=exe
if(TARGET_TYPE STREQUAL "exe")
    message(STATUS "create exe.")
    message(STATUS ${SRCS})
    add_executable(datastruct ${SRCS})
else()
    message(STATUS "create lib")
    message(STATUS ${SRCS})
    list(APPEND SRCS ./source/test_main.c)
    add_library (datastruct ${SRCS})
endif()


