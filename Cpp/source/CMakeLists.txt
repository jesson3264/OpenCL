
## Function: 生成直接调试的代码或者直接生成库文件，默认直接生成库文件
## Useage: 
## 
###############

cmake_minimum_required (VERSION 2.8)

add_definitions(-std=c++11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11")
option(ENABLE_CREATE_TEST "enable to compile test exe." ON)

## 判断是否生成 scppl 库
if (ENABLE_CREATE_TEST)
    message (STATUS "ENABLE_CREATE_TEST IS ON")
    add_definitions(-DENABLE_CREATE_TEST)
    set(execute_name test-scppl)
    project(${execute_name})

else()
    set(lib_name scppl)
    project(${lib_name})
endif(ENABLE_CREATE_TEST)

configure_file(
    "${PROJECT_SOURCE_DIR}/config/include/config.h.in"
    "${PROJECT_SOURCE_DIR}/config/include/config.h")


## 配置编译模块

set (MODULE_NAMES common)
list (APPEND MODULE_NAMES algorithm test config file xString) 

set (ALL_SRC_FILES "")
FOREACH (module_name ${MODULE_NAMES})
	include_directories(./${module_name}/include)
	FILE (GLOB_RECURSE files ./${module_name}/source/*.cpp ./${module_name}/source/*.c)
	list(APPEND ALL_SRC_FILES ${files})	
	message(STATUS ${files})
ENDFOREACH(module_name)

### 增加 C 库的代码编译

set(C_MODULE_NAMES common)
list(APPEND C_MODULE_NAMES datastruct memory strings)
FOREACH(module_name ${C_MODULE_NAMES})
    include_directories(./../../C/source/${module_name}/include)
    message(STATUS "iterator C module")
    FILE( GLOB_RECURSE files ./../../C/source/${module_name}/source/*.c)
    list(APPEND ALL_SRC_FILES ${files})
    message(STATUS ${files})
ENDFOREACH(module_name)

message(STATUS "all files.")
message(STATUS ${ALL_SRC_FILES})


if (ENABLE_CREATE_TEST)
    add_executable(${execute_name} ${ALL_SRC_FILES})
    install (TARGETS ${execute_name}
		 RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/
		)
else()
    message(STATUS  "start install ")
    set (CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../scppl)
    
    add_library(scppl ${ALL_SRC_FILES})

    install (TARGETS scppl 
	    ARCHIVE DESTINATION lib)

    FOREACH (module_name ${MODULE_NAMES})
	    FILE (GLOB module_name_headers ${PROJECT_SOURCE_DIR}/${module_name}/include/*.h)
	    message(status ${module_name_headers})
	    install (FILES ${module_name_headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${module_name})
    ENDFOREACH(module_name)

endif(ENABLE_CREATE_TEST)





