cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type.")
project(SSVUtils)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/VRCMakeCommon/cmake/modules/;${CMAKE_MODULE_PATH}")
include(VRCC_flags)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/;${CMAKE_MODULE_PATH}")
set(SSVUTILS_BUILD_SHARED_LIB TRUE CACHE BOOL "Build shared libraries.")

if(SSVUTILS_BUILD_SHARED_LIB)
	set(LIB_TYPE SHARED)
else()
	set(LIB_TYPE STATIC)
endif()

set(SRC_DIR "src")
set(INC_DIR "include")

file(GLOB SRC_LIST
	"${INC_DIR}/${PROJECT_NAME}/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Delegate/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Delegate/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Encryption/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Encryption/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/FileSystem/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/FileSystem/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Log/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Log/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/MemoryManager/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/MemoryManager/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/MemoryManager/Traits/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/MemoryManager/Traits/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Timeline/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Timeline/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Utils/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Utils/*.cpp"
)

include_directories(${INC_DIR})
add_library(${PROJECT_NAME} ${LIB_TYPE} ${SRC_LIST})

if(SSVUTILS_BUILD_SHARED_LIB)
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)
else()
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -s-d)
	set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX -s)
endif()

if(WIN32)
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
else()
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION lib LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY ${INC_DIR} DESTINATION .)
