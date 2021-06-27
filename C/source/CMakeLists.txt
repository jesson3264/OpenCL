cmake_minimum_required (VERSION 2.8)
project(libscl)
# 设置安装的目录

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

set (CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../scl)

set(MODULE_NAMES     datastruct)
list(APPEND MODULE_NAMES memory common strings )
### 在这里加上新的模块名即可


FOREACH (module_name ${MODULE_NAMES})
	include_directories(./${module_name}/include)
ENDFOREACH(module_name)

FILE(GLOB_RECURSE ALL_SRC_FILES  "./*.c")

message(STATUS "xxx")
message(STATUS ${ALL_SRC_FILES})
message (STATUS ${PROJECT_SOURCE_DIR})
message(STATUS "yyy")

# 生成静态库 libscl.a
add_library (scl ${ALL_SRC_FILES})

install (TARGETS scl 
	#LIBRARY DESTINATION lib)
	ARCHIVE DESTINATION lib
	#PUBLIC_HEADERS DESTINATION include
)

FOREACH (module_name ${MODULE_NAMES})
	FILE (GLOB module_name_headers ${PROJECT_SOURCE_DIR}/${module_name}/include/*.h)
	message(status ${module_name_headers})
	install (FILES ${module_name_headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${module_name})
ENDFOREACH(module_name)


