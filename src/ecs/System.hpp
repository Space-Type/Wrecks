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
#include <wrecks/utils.hpp>
#include <type_traits>
#include <tuple>


namespace Wrecks::ECS {
    struct SystemAttributes {
        enum { STATE, RENDER } stage;
        unsigned int order;
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

    template <SystemTuple T>
    struct separate_systems;

    template <System T, System ...U>
        requires (T::ATTRIBUTES.stage == SystemAttributes::STATE)
    struct separate_systems<std::tuple<T, U...>> {
        using state  = Utils::concat_types_t<std::tuple<T>, typename separate_systems<std::tuple<U...>>::state>;
        using render = typename separate_systems<std::tuple<U...>>::render;
    };

    template <System T, System ...U>
        requires (T::ATTRIBUTES.stage == SystemAttributes::RENDER)
    struct separate_systems<std::tuple<T, U...>> {
        using state  = typename separate_systems<std::tuple<U...>>::state;
        using render = Utils::concat_types_t<std::tuple<T>, typename separate_systems<std::tuple<U...>>::render>;
    };

    template <>
    struct separate_systems<std::tuple<>> {
        using state  = std::tuple<>;
        using render = std::tuple<>;
    };

    template <typename T>
    using state_systems_t = typename separate_systems<T>::state;

    template <typename T>
    using render_systems_t = typename separate_systems<T>::render;
}
