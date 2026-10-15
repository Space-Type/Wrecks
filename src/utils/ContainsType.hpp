/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** ContainsType concept
*/

#pragma once

#include <type_traits>
#include <tuple>


namespace Wrecks::Utils {
    template <typename T, typename ...Elements>
    struct contains_type {
        static constexpr bool value = (std::is_same_v<T, Elements> || ...);
    };

    template <typename T, typename ...Elements>
    inline constexpr bool contains_type_v = contains_type<T, Elements...>::value;

    template <typename T, typename ...Elements>
    concept ContainsType = contains_type_v<T, Elements...>;


    template <typename T, typename Tuple>
    struct tuple_contains_type;

    template <typename T, typename ...Elements>
    struct tuple_contains_type<T, std::tuple<Elements...>> : contains_type<T, Elements...> {};

    template <typename T, typename Tuple>
    inline constexpr bool tuple_contains_type_v = tuple_contains_type<T, Tuple>::value;

    template <typename T, typename Tuple>
    concept TupleContainsType = tuple_contains_type_v<T, Tuple>;
}
