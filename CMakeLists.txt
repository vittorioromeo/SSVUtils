cmake_minimum_required(VERSION 2.8)
project(SSVUtils)

set(SSVUTILS_BUILD_SHARED_LIB FALSE CACHE BOOL "Build shared libraries.")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type." FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" CACHE STRING "" FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -pg -Og" CACHE STRING "" FORCE)

if(SSVUTILS_BUILD_SHARED_LIB)
	set(LIB_TYPE SHARED)
else()
	set(LIB_TYPE STATIC)
endif()

set(SRC_DIR "src")
set(INC_DIR "include")

set(
	SOURCES
	${INC_DIR}/${PROJECT_NAME}/Delegate/Delegate.h
	${INC_DIR}/${PROJECT_NAME}/Encryption/Base64.h
	${INC_DIR}/${PROJECT_NAME}/Encryption/MD5.h
	${INC_DIR}/${PROJECT_NAME}/Encryption/ObfuscatedValue.h
	${INC_DIR}/${PROJECT_NAME}/FileSystem/FileSystem.h
	${INC_DIR}/${PROJECT_NAME}/Log/Log.h
	${INC_DIR}/${PROJECT_NAME}/MemoryManager/MemoryManager.h
	${INC_DIR}/${PROJECT_NAME}/MemoryManager/Traits/Traits.h
	${INC_DIR}/${PROJECT_NAME}/Timeline/Command.h
	${INC_DIR}/${PROJECT_NAME}/Timeline/Do.h
	${INC_DIR}/${PROJECT_NAME}/Timeline/Go.h
	${INC_DIR}/${PROJECT_NAME}/Timeline/Timeline.h
	${INC_DIR}/${PROJECT_NAME}/Timeline/TimelineManager.h
	${INC_DIR}/${PROJECT_NAME}/Timeline/Wait.h
	${INC_DIR}/${PROJECT_NAME}/Utils/Utils.h
	${INC_DIR}/${PROJECT_NAME}/Utils/UtilsContainers.h
	${INC_DIR}/${PROJECT_NAME}/Utils/UtilsMath.h
	${INC_DIR}/${PROJECT_NAME}/Utils/UtilsString.h
	${SRC_DIR}/${PROJECT_NAME}/Encryption/Base64.cpp
	${SRC_DIR}/${PROJECT_NAME}/Encryption/MD5.cpp
	${SRC_DIR}/${PROJECT_NAME}/FileSystem/FileSystem.cpp
	${SRC_DIR}/${PROJECT_NAME}/Log/Log.cpp
	${SRC_DIR}/${PROJECT_NAME}/Timeline/Command.cpp
	${SRC_DIR}/${PROJECT_NAME}/Timeline/Do.cpp
	${SRC_DIR}/${PROJECT_NAME}/Timeline/Go.cpp
	${SRC_DIR}/${PROJECT_NAME}/Timeline/Timeline.cpp
	${SRC_DIR}/${PROJECT_NAME}/Timeline/TimelineManager.cpp
	${SRC_DIR}/${PROJECT_NAME}/Timeline/Wait.cpp
	${SRC_DIR}/${PROJECT_NAME}/Utils/Utils.cpp
	${SRC_DIR}/${PROJECT_NAME}/Utils/UtilsContainers.cpp
	${SRC_DIR}/${PROJECT_NAME}/Utils/UtilsMath.cpp
	${SRC_DIR}/${PROJECT_NAME}/Utils/UtilsString.cpp
)

include_directories(${INC_DIR})
add_library(${PROJECT_NAME} ${LIB_TYPE} ${SOURCES})

if(SSVUTILS_BUILD_SHARED_LIB)
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)
else()
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -s-d)
	set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX -s)
endif()

if(WIN32)
	install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
else()
	install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION lib
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY ${INC_DIR}
	DESTINATION .)
