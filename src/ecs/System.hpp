/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** System concept
*/

#pragma once

#include "Entity.hpp"
#include <wrecks/common.hpp>
#include <type_traits>
#include <tuple>


namespace Wrecks::ECS {
    template <typename T>
    concept System =
        std::is_nothrow_default_constructible_v<T> &&
        requires (T& mutSystem, const T& cnstSystem, Entity entity, void* engine)
        {
            { cnstSystem.signature() } -> std::same_as<Signature>;
            { mutSystem.setEngine(engine) };
            { mutSystem(entity) };
        };

    template <typename T>
    struct is_system_tuple : std::false_type {};

    template <System ...S>
    struct is_system_tuple<std::tuple<S...>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_system_tuple_v = is_system_tuple<T>::value;

    template <typename T>
    concept SystemTuple = is_system_tuple_v<T>;
}
