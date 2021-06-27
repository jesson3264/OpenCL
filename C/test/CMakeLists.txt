
cmake_minimum_required(VERSION 2.8)

project(scl_test)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
include_directories(./include)
include_directories(../scl/include/datastruct/)
include_directories(../scl/include/memory/)
include_directories(../scl/include/common/)

aux_source_directory(./source DIR_EXE_SRCS)

# 生成可执行文件
add_executable(scl_test ${DIR_EXE_SRCS})

#find_library(liblibscl.a ../source/build/)
target_link_libraries(scl_test ${PROJECT_SOURCE_DIR}/../scl/lib/libscl.a)

install (TARGETS scl_test
	RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/
	
	)
