/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** element_index type function
*/

#pragma once

#include "IndexOf.hpp"


namespace Wrecks::Utils {
    template <typename T, typename Container>
    struct element_index;

    template <typename T, template <typename...> typename ContainerType, typename ...Elements>
    struct element_index<T, ContainerType<Elements...>> : index_of<T, Elements...> {};

    template <typename T, typename Container>
    inline constexpr std::size_t element_index_v = element_index<T, Container>::value;
}
