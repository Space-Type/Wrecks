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
        enum { UPDATE, FIXED_UPDATE, RENDER } stage;
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


    ///////////////////////////////////////////////////////
    //                                                   //
    //                 System Separation                 //
    //                                                   //
    ///////////////////////////////////////////////////////

    template <SystemTuple T>
    struct separate_systems;

    template <System T, System ...U>
        requires (T::ATTRIBUTES.stage == SystemAttributes::UPDATE)
    struct separate_systems<std::tuple<T, U...>> {
        using update = Utils::concat_types_t<std::tuple<T>, typename separate_systems<std::tuple<U...>>::update>;
        using fixed_update = typename separate_systems<std::tuple<U...>>::fixed_update;
        using render = typename separate_systems<std::tuple<U...>>::render;
    };

    template <System T, System ...U>
        requires (T::ATTRIBUTES.stage == SystemAttributes::FIXED_UPDATE)
    struct separate_systems<std::tuple<T, U...>> {
        using update = typename separate_systems<std::tuple<U...>>::update;
        using fixed_update = Utils::concat_types_t<std::tuple<T>, typename separate_systems<std::tuple<U...>>::fixed_update>;
        using render = typename separate_systems<std::tuple<U...>>::render;
    };

    template <System T, System ...U>
        requires (T::ATTRIBUTES.stage == SystemAttributes::RENDER)
    struct separate_systems<std::tuple<T, U...>> {
        using update  = typename separate_systems<std::tuple<U...>>::update;
        using fixed_update  = typename separate_systems<std::tuple<U...>>::fixed_update;
        using render = Utils::concat_types_t<std::tuple<T>, typename separate_systems<std::tuple<U...>>::render>;
    };

    template <>
    struct separate_systems<std::tuple<>> {
        using update       = std::tuple<>;
        using fixed_update = std::tuple<>;
        using render       = std::tuple<>;
    };


    ///////////////////////////////////////////////////////
    //                                                   //
    //                  System Ordering                  //
    //                                                   //
    ///////////////////////////////////////////////////////

    template <SystemTuple T>
    struct sort_systems_pass;

    template <System A, System B, System ...Rest>
        requires (A::ATTRIBUTES.order <= B::ATTRIBUTES.order)
    struct sort_systems_pass<std::tuple<A, B, Rest...>> {
        using rest_type = sort_systems_pass<std::tuple<B, Rest...>>;

        static constexpr bool has_swapped = rest_type::has_swapped;
        using type = Utils::concat_types_t<std::tuple<A>, typename rest_type::type>;
    };

    template <System A, System B, System ...Rest>
        requires (A::ATTRIBUTES.order > B::ATTRIBUTES.order)
    struct sort_systems_pass<std::tuple<A, B, Rest...>> {
        static constexpr bool has_swapped = true;
        using type = Utils::concat_types_t<std::tuple<B>, typename sort_systems_pass<std::tuple<A, Rest...>>::type>;
    };

    template <System T>
    struct sort_systems_pass<std::tuple<T>> {
        static constexpr bool has_swapped = false;
        using type = std::tuple<T>;
    };

    template <>
    struct sort_systems_pass<std::tuple<>> {
        static constexpr bool has_swapped = false;
        using type = std::tuple<>;
    };

    template <SystemTuple T>
    struct sort_systems;

    template <SystemTuple T>
        requires (!sort_systems_pass<T>::has_swapped)
    struct sort_systems<T> : sort_systems_pass<T> {};

    template <SystemTuple T>
        requires (sort_systems_pass<T>::has_swapped)
    struct sort_systems<T> : sort_systems<typename sort_systems_pass<T>::type> {};

    template <SystemTuple T>
    using sort_systems_t = typename sort_systems<T>::type;
}
