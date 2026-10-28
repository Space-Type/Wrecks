/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** ContainerElements type function
*/

#pragma once

#include <cstddef>


namespace Wrecks::Utils {
    /**
     * Allows you to determine how
     * many template arguments were
     * given to a template type.
     */
    template <typename T>
    struct container_elements;

    template <template <typename...> typename Container, typename A, typename ...Rest>
    struct container_elements<Container<A, Rest...>> {
        static constexpr std::size_t value = 1 + container_elements<Container<Rest...>>::value;
    };

    template <template <typename...> typename Container>
    struct container_elements<Container<>> {
        static constexpr std::size_t value = 0;
    };

    /**
     * The quantity of template arguments
     * given to a specified container
     * type T.
     */
    template <typename T>
    inline constexpr std::size_t container_elements_v = container_elements<T>::value;

}
