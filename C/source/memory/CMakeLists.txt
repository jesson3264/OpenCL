cmake_minimum_required (VERSION 2.8)

project(libmemory)
aux_source_directory(./source DIR_LIB_SRCS)
include_directories(./include)
# 生成链接库
add_library (memory ${DIR_LIB_SRCS})
