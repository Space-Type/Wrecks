/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** index_of type function
*/

#pragma once

#include <cstddef>


namespace Wrecks::Utils {
    template <typename T, typename ...Elements>
    struct index_of;

    template <typename T, typename ...Elements>
    struct index_of<T, T, Elements...> {
        static constexpr std::size_t value = 0;
    };

    template <typename T, typename U, typename ...Elements>
    struct index_of<T, U, Elements...> {
        static constexpr std::size_t value = 1 + index_of<T, Elements...>::value;
    };

    template <typename T, typename ...Elements>
    inline constexpr std::size_t index_of_v = index_of<T, Elements...>::value;
}
