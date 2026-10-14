/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** tuple_index type function
*/

#pragma once

#include <cstdint>
#include <tuple>


namespace Wrecks::Utils {
    template <typename T, typename Tuple>
    struct tuple_index;

    template <typename T, typename ...Elements>
    struct tuple_index<T, std::tuple<T, Elements...>> {  // T first (i.e. found)
        static constexpr std::size_t value = 0;
    };

    template <typename T, typename U, typename ...Elements>
    struct tuple_index<T, std::tuple<U, Elements...>> {  // T not first (go next)
        static constexpr std::size_t value = 1 + tuple_index<T, std::tuple<Elements...>>::value;
    };

    template <typename T>
    struct tuple_index<T, std::tuple<>> {  // T not found
        static constexpr std::size_t value = SIZE_MAX;
    };

    template <typename T, typename Tuple>
    inline constexpr std::size_t tuple_index_v = tuple_index<T, Tuple>::value;
}
