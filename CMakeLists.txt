cmake_minimum_required(VERSION 2.8)
project(SSVUtils)
enable_testing()

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/../SSVCMake/cmake/modules/"
    "${CMAKE_SOURCE_DIR}/extlibs/SSVCMake/cmake/modules/"
    "${CMAKE_SOURCE_DIR}/../vrm_pp/cmake/"
    "${CMAKE_SOURCE_DIR}/extlibs/vrm_pp/cmake/"
    "${CMAKE_MODULE_PATH}")

include(SSVCMake)

SSVCMake_findExtlib(vrm_pp)
SSVCMake_setDefaults()
SSVCMake_setAndInstallHeaderOnly()

add_custom_target(check 
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Build and then run all the tests and examples.")

add_subdirectory(test)
