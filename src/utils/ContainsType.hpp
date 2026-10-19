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

    template <typename T, typename Container>
    struct container_contains_type;

    template <typename T, template <typename...> typename Container, typename ...Elements>
    struct container_contains_type<T, Container<Elements...>> : contains_type<T, Elements...> {};

    template <typename T, typename Container>
    inline constexpr bool container_contains_type_v = container_contains_type<T, Container>::value;

    template <typename T, typename Container>
    concept ContainerContainsType = container_contains_type_v<T, Container>;
}
