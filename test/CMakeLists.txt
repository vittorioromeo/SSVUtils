# Add a custom target for the unit tests.
add_custom_target(tests COMMENT "Build all the unit tests.")

# `check` depends on `tests`.
add_dependencies(check tests)

# Include directories.
include_directories(${SSVUTILS_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})

# TODO:
# Generate all the header unit tests.
# vrm_cmake_generate_public_header_tests_glob("*.hpp" "${SSVUTILS_SOURCE_DIR}/include")

# Generate all the unit tests.
vrm_cmake_generate_unit_tests_glob("*.cpp")