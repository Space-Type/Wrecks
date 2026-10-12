/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** System concept
*/

#pragma once

#include <type_traits>
#include <wrecks/common.hpp>
#include <tuple>


namespace Wrecks::ECS {
    template <typename T>
    concept System = requires (T& mutSystem, const T& cnstSystem)
    {
        { cnstSystem.signature() } -> std::same_as<Signature>;
        { mutSystem() };
    } && std::is_nothrow_default_constructible_v<T>;

    template <typename T>
    struct is_system_tuple {
        static constexpr bool value = false;
    };

    template <System ...S>
    struct is_system_tuple<std::tuple<S...>> {
        static constexpr bool value = true;
    };

    template <typename T>
    inline constexpr bool is_system_tuple_v = is_system_tuple<T>::value;

    template <typename T>
    concept SystemTuple = is_system_tuple_v<T>;
}
