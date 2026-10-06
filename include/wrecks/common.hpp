/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** some common utilities
*/

#pragma once

#include <string_view>
#include <cstdint>


namespace Wrecks {
    // View into a null terminated string
    using cstring_view = std::string_view;

    using Tick = std::size_t;
}
