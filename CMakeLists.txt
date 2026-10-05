##
## EPITECH PROJECT, 2026
## Project - Wrecks
## File description:
## Main project build
## file
##

cmake_minimum_required(VERSION 3.15)

project(raytracer VERSION 1.0
                  DESCRIPTION "Wrecks library - R-Type project"
                  HOMEPAGE_URL "https://github.com/Space-Type/Wrecks"
                  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(WRECKS_SOURCES
    src/networking/UDPSocket/UDPSocket.hpp
    src/networking/UDPSocket/UDPSocket.unix.cpp
    src/networking/UDPSocket/UDPSocket.win.cpp)

if (WIN32)
    set(EXPLUDED_PLATFORM "unix")
else()
    set(EXPLUDED_PLATFORM "win")
endif()

list(FILTER WRECKS_SOURCES EXCLUDE REGEX "\\.${EXCLUDED_PLATFORM}\\.[ch]pp$")

add_library(wrecks STATIC ${WRECKS_SOURCES})
target_include_directories(wrecks PUBLIC include)
