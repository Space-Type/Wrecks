/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** tuple_index type function
*/

#pragma once

#include "IndexOf.hpp"
#include <tuple>


namespace Wrecks::Utils {
    template <typename T, typename Tuple>
    struct tuple_index;

    template <typename T, typename ...Elements>
    struct tuple_index<T, std::tuple<Elements...>> : index_of<T, Elements...> {};

    template <typename T, typename Tuple>
    inline constexpr std::size_t tuple_index_v = tuple_index<T, Tuple>::value;
}
