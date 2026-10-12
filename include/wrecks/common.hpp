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
#include <bitset>


namespace Wrecks {

    /////////////////////////////////////////////////////////
    //                                                     //
    //                      CONSTANTS                      //
    //                                                     //
    /////////////////////////////////////////////////////////

    inline constexpr std::size_t COMPONENTS_MAX = 64;


    /////////////////////////////////////////////////////////
    //                                                     //
    //                        TYPES                        //
    //                                                     //
    /////////////////////////////////////////////////////////

    // String view for null-terminated strings
    using cstring_view = std::string_view;

    using Signature = std::bitset<COMPONENTS_MAX>;
    using Tick = std::size_t;
}
