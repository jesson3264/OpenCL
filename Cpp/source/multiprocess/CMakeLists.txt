cmake_minimum_required (VERSION 2.8)
project (multiprocess)

#set(SRCS "./source/*.cpp")
aux_source_directory(./source DIR_SRCS)
include_directories(./include)


#set (TestMain 0)
if (NOT SUB_IS_LIB)
	message("create exe")
	add_executable(multiprocess ${DIR_SRCS})
else ()
	message("create libs.")
	add_library(libmultiprocess ${DIR_SRCS})
endif ()
