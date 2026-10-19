/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** System concept
*/

#pragma once

#include "Entity.hpp"
#include "ComponentManager.hpp"
#include <wrecks/common.hpp>
#include <type_traits>
#include <tuple>


namespace Wrecks::ECS {
    template <typename Systems, ComponentManagerType Components>
    class Engine;

    struct SystemAttributes {
        enum { STATE, RENDER } stage;
    };
    
    template <typename T>
    concept System =
        std::is_nothrow_default_constructible_v<T> &&
        requires (T& mutSystem, const T& cnstSystem, Entity entity, Engine<std::tuple<>, ComponentManager<>> engine)
        {
            { T::ATTRIBUTES } -> std::same_as<const SystemAttributes&>;
            { cnstSystem.signature() } -> std::same_as<Signature>;
            { mutSystem.template operator()<decltype(engine)>(engine, entity) };
        };
}
