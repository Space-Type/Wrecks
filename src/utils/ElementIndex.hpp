/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** element_index type function
*/

#pragma once

#include "IndexOf.hpp"
#include <variant>
#include <tuple>


namespace Wrecks::Utils {
    template <typename T, typename TupleOrVariant>
    struct element_index;

    template <typename T, typename ...Elements>
    struct element_index<T, std::tuple<Elements...>> : index_of<T, Elements...> {};

    template <typename T, typename ...Elements>
    struct element_index<T, std::variant<Elements...>> : index_of<T, Elements...> {};

    template <typename T, typename TupleOrVariant>
    inline constexpr std::size_t element_index_v = element_index<T, TupleOrVariant>::value;
}
