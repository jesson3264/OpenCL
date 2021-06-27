cmake_minimum_required (VERSION 2.8)

project(libdatastruct)
include_directories(./include)
include_directories(../common/include)

aux_source_directory(./source DIR_LIB_SRCS)
aux_source_directory(../source )
# 生成链接库

# 默认生成 lib
#cmake -DTARGET_LIB:STRING=exe
if(TARGET_LIB STREQUAL "exe")
    add_executable(datastruct ${DIR_LIB_SRCS})
else()
    list(REMOVE_ITEM DIR_LIB_SRCS ./source/test_main.c)
    add_library (datastruct ${DIR_LIB_SRCS})
endif()


