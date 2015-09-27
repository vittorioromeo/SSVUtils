cmake_minimum_required(VERSION 2.8)
project(SSVUtils)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/../SSVCMake/cmake/modules/"
    "${CMAKE_SOURCE_DIR}/extlibs/SSVCMake/cmake/modules/"
    "${CMAKE_SOURCE_DIR}/../vrm_pp/cmake/"
    "${CMAKE_SOURCE_DIR}/extlibs/vrm_pp/cmake/"
    "${CMAKE_MODULE_PATH}")

include(SSVCMake)

find_package(vrm_pp)

SSVCMake_setDefaults()
SSVCMake_setAndInstallHeaderOnly()
