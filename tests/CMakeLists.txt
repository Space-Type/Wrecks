##
## EPITECH PROJECT, 2026
## Project - Wrecks
## File description:
## Unit test build
## file
##

cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

FetchContent_MakeAvailable(googletest)

set(TEST_SOURCES
    ecs/ComponentContainer.cpp
    ecs/Engine.cpp
    ecs/EntityContainer.cpp
    errors/Error.cpp
    errors/NetworkError.cpp
    events/BaseEventReceiver.cpp
    events/EventManager.cpp
    networking/SequenceContainer.cpp
    networking/UDPSocket.cpp
    networking/UDPSocket.unix.cpp
    networking/UDPSocket.win.cpp
    utils/IndexIterator.cpp)

if (WIN32)
    set(EXCLUDED_PLATFORM "unix")
else()
    set(EXCLUDED_PLATFORM "win")
endif()

list(FILTER TEST_SOURCES EXCLUDE REGEX "\\.${EXCLUDED_PLATFORM}\\.[ch]pp$")

add_executable(wrecks_tests ${TEST_SOURCES})

target_link_libraries(wrecks_tests PRIVATE wrecks
                                   PRIVATE GTest::gtest_main)

target_compile_options(wrecks_tests PRIVATE --coverage)
target_link_options(wrecks_tests PRIVATE --coverage)

include(GoogleTest)
gtest_discover_tests(wrecks_tests)
