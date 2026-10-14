/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** ContainsType concept
*/

#pragma once

#include <type_traits>


namespace Wrecks::Utils {
    template <typename T, typename ...Elements>
    struct contains_type {
        static constexpr bool value = (std::is_same_v<T, Elements> || ...);
    };

    template <typename T, typename ...Elements>
    inline constexpr bool contains_type_v = contains_type<T, Elements...>::value;

    template <typename T, typename ...Elements>
    concept ContainsType = contains_type_v<T, Elements...>;
}
