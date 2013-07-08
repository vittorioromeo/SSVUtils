cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type.")
project(SSVUtils)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/VRCMakeCommon/cmake/modules/;${CMAKE_MODULE_PATH}")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -W -pedantic" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer -g3 -gdwarf-2 -O0" CACHE STRING "" FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/;${CMAKE_MODULE_PATH}")

set(BUILD_SHARED_LIB TRUE CACHE BOOL "Build shared libraries.")
if(BUILD_SHARED_LIB)
	set(LIB_TYPE SHARED)
else()
	set(LIB_TYPE STATIC)
endif()

set(SRC_DIR "src")
set(INC_DIR "include")
file(GLOB_RECURSE SRC_LIST "${INC_DIR}/*" "${SRC_DIR}/*")

include_directories(${INC_DIR})
add_library(${PROJECT_NAME} ${LIB_TYPE} ${SRC_LIST})

if(BUILD_SHARED_LIB)
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)
else()
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -s-d)
	set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX -s)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION lib LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY ${INC_DIR} DESTINATION .)
