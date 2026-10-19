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
    struct SystemAttributes {
        enum { STATE, RENDER } stage;
    };
    
    template <typename T>
    concept System =
        std::is_nothrow_default_constructible_v<T> &&
        requires (T& mutSystem, const T& cnstSystem, Entity entity, void* engine)
        {
            { T::ATTRIBUTES } -> std::same_as<const SystemAttributes&>;
            { cnstSystem.signature() } -> std::same_as<Signature>;
            { mutSystem.setEngine(engine) };
            { mutSystem(entity) };
        };

    template <typename T>
    struct is_system_tuple : std::false_type {};

    template <System ...Element>
    struct is_system_tuple<std::tuple<Element...>> : std::true_type {};

    template <typename T>
    concept SystemTuple = is_system_tuple<T>::value;
}
