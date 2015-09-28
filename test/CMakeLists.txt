# Add a custom target for the unit tests.
add_custom_target(tests COMMENT "Build all the unit tests.")

# `check` depends on `tests`.
add_dependencies(check tests)

# Return an unique name for a file target.
# Replaces slashes with `.`, assumes `.cpp` if the extension is not specified.
function(ssvu_target_name_for out file)
    if (NOT ARGV2)
        set(_extension ".cpp")
    else()
        set(_extension "${ARGV2}")
    endif()

    file(RELATIVE_PATH _relative ${CMAKE_SOURCE_DIR} ${file})
    string(REPLACE "${_extension}" "" _name ${_relative})
    string(REGEX REPLACE "/" "." _name ${_name})
    set(${out} "${_name}" PARENT_SCOPE)
endfunction()


# Adds a test, also as part of the `tests` target.
function(ssvu_add_unit_test name)
    add_test(${ARGV})
    add_dependencies(tests ${name})
endfunction()

# # Adds a test for a public header, making sure including it works properly.
# function(ssvu_add_header_test header)
#     string(REGEX REPLACE "/" "." _target "${header}")
#
#     if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
#         file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp" "
#             #include <${header}>
#             int main() { }
#         ")
#     endif()
#
#     add_executable(test.header.${_target} EXCLUDE_FROM_ALL
#                         "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
#
#     ssvu_add_test(test.header.${_target}
#                             ${CMAKE_CURRENT_BINARY_DIR}/test.header.${_target})
#
#     add_dependencies(tests test.header.${_target})
# endfunction()

# Glob all tests.
file(GLOB_RECURSE SSVU_TEST_SOURCES "*.cpp")

# # Glob all public headers. (Detail headers can be removed here.)
# file(GLOB_RECURSE ssvu_PUBLIC_HEADERS "${ssvu_SOURCE_DIR}/include/*.hpp")
# ssvu_list_remove_glob(SSVU_PUBLIC_HEADERS GLOB_RECURSE
#     "dummy")

# Include directories.
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})

# # Generate tests that include each public header.
# foreach(_header IN LISTS ssvu_PUBLIC_HEADERS)
#     file(RELATIVE_PATH _relative "${ssvu_SOURCE_DIR}/include" "${_header}")
#     ssvu_add_header_test("${_relative}")
# endforeach()

# Add all the unit tests.
foreach(_file IN LISTS SSVU_TEST_SOURCES)
    file(READ "${_file}" _contents)
    ssvu_target_name_for(_target "${_file}")

    add_executable(${_target} EXCLUDE_FROM_ALL "${_file}")
    ssvu_add_unit_test(${_target} ${CMAKE_CURRENT_BINARY_DIR}/${_target})
endforeach()

# TODO: rewrite
# TODO: add tests