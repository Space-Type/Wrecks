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
    ecs/ComponentManager.cpp
    ecs/Engine.cpp
    ecs/EntityContainer.cpp
    errors/Error.cpp
    events/BaseEventReceiver.cpp
    events/EventManager.cpp
    utils/FilteredIterator.cpp
    utils/IndexIterator.cpp
    utils/SequenceContainer.cpp)


add_executable(wrecks_tests ${TEST_SOURCES})

target_link_libraries(wrecks_tests PRIVATE wrecks
                                   PRIVATE GTest::gtest_main)

set_target_properties(wrecks_tests PROPERTIES MAP_IMPORTED_CONFIG_DEBUGTESTS Debug)
target_compile_options(wrecks_tests PRIVATE --coverage)
target_link_options(wrecks_tests PRIVATE --coverage)

include(GoogleTest)
gtest_discover_tests(wrecks_tests)
