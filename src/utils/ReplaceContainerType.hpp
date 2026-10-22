/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** replace_container_type_t type function
*/

#pragma once


namespace Wrecks::Utils {
    template <typename New, typename Old>
    struct replace_container_type;

    template <template <typename...> typename New, template <typename...> typename Old, typename ...Elements>
    struct replace_container_type<New<>, Old<Elements...>> {
        using type = New<Elements...>;
    };

    template <typename New, typename Old>
    using replace_container_type_t = typename replace_container_type<New, Old>::type;
}
