##
## EPITECH PROJECT, 2026
## Project - Wrecks
## File description:
## Main project build
## file
##

cmake_minimum_required(VERSION 3.15)

project(Wrecks-Library VERSION 1.0
                       DESCRIPTION "Wrecks library - R-Type project"
                       HOMEPAGE_URL "https://github.com/Space-Type/Wrecks"
                       LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo;DebugTests"
                              CACHE STRING "Supported build types" FORCE)

set(CMAKE_CXX_FLAGS_DEBUGTESTS "${CMAKE_CXX_FLAGS_DEBUG}"
                               CACHE STRING "Flags used by the C++ compiler during DebugTests builds.")

set(CMAKE_CXX_STANDARD 23)

set(WRECKS_SOURCES
    src/ecs/EntityContainer/EntityContainer.cpp
    src/ecs/EntityContainer/EntityContainer.hpp
    src/ecs/Component.hpp
    src/ecs/ComponentContainer.hpp
    src/ecs/ComponentManager.hpp
    src/ecs/Engine.hpp
    src/ecs/Entity.hpp
    src/ecs/System.hpp
    src/errors/Error/Error.cpp
    src/errors/Error/Error.hpp
    src/events/EventManager/EventManager.cpp
    src/events/EventManager/EventManager.hpp
    src/events/BaseEventReceiver.hpp
    src/events/Event.hpp
    src/events/IEventReceiver.hpp
    src/utils/ConcatTypes.hpp
    src/utils/ContainsType.hpp
    src/utils/ElementIndex.hpp
    src/utils/FilteredIterator.hpp
    src/utils/IndexIterator.hpp
    src/utils/IndexOf.hpp
    src/utils/ReplaceContainerType.hpp
    src/utils/SequenceContainer.hpp
    src/utils/VisitType.hpp)

add_library(wrecks STATIC ${WRECKS_SOURCES})
target_include_directories(wrecks PUBLIC include)

if (CMAKE_BUILD_TYPE STREQUAL "DebugTests")
    if (NOT WIN32)
        target_compile_options(wrecks PRIVATE --coverage)
        target_link_options(wrecks PRIVATE --coverage)
    endif()

    enable_testing()
    add_subdirectory(tests)
endif()

