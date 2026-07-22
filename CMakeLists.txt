cmake_minimum_required(VERSION 3.24)

project(algorithm_impl VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

# ------------------------------------------------------------------
# Algorithm Lib
# ------------------------------------------------------------------
set(IMPL_LIB_TARGET "algorithm_impl")

add_library(${IMPL_LIB_TARGET} INTERFACE)

target_include_directories(
    ${IMPL_LIB_TARGET}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(
    ${IMPL_LIB_TARGET}
    INTERFACE
        cxx_std_20
)

# ------------------------------------------------------------------
# 测试
# ------------------------------------------------------------------

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        GIT_SHALLOW TRUE
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(TEST_TARGET "algorithm_tests")

    FetchContent_MakeAvailable(googletest)

    add_executable(
        ${TEST_TARGET}
        tests/sort_tests.cpp
    )

    target_link_libraries(
        ${TEST_TARGET}
        PRIVATE
            ${IMPL_LIB_TARGET}
            GTest::gtest_main
    )

    if(MSVC)
        target_compile_options(
            ${TEST_TARGET}
            PRIVATE
                /W4
                /permissive-
        )
    else()
        target_compile_options(
            ${TEST_TARGET}
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )
    endif()

    include(GoogleTest)

    gtest_discover_tests(${TEST_TARGET})
endif()