cmake_minimum_required(VERSION 3.10)

project(banking)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

add_library(banking
    banking/Account.cpp
    banking/Transaction.cpp
)

target_include_directories(banking PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/banking
)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(third-party/gtest)

    add_executable(banking_tests
        tests/test_banking.cpp
    )

    target_link_libraries(banking_tests
        banking
        gtest
        gtest_main
        gmock
        pthread
    )

    add_test(NAME banking_tests COMMAND banking_tests)
endif()
