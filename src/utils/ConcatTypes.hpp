/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** concat_types_t type function
*/

#pragma once


namespace Wrecks::Utils {
    template <typename A, typename B>
    struct concat_types;

    template <template <typename...> typename T, typename ...ElementsA, typename ...ElementsB>
    struct concat_types<T<ElementsA...>, T<ElementsB...>> {
        using type = T<ElementsA..., ElementsB...>;
    };

    template <typename A, typename B>
    using concat_types_t = typename concat_types<A, B>::type;
}
