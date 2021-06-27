
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -pthread" )
project(log4cpp-test)

aux_source_directory(./source DIR_LIB_SRCS)

include_directories(./source)
include_directories(/home/jesson/compiler-out/log4cpp-1.1.3/include)

add_executable(log4cpp-test ${DIR_LIB_SRCS})
target_link_libraries(log4cpp-test /home/jesson/compiler-out/log4cpp-1.1.3/lib/liblog4cpp.a)

