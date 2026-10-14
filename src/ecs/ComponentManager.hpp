/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** ComponentManager class
*/

#pragma once

#include "ComponentContainer.hpp"
#include "Component.hpp"
#include <wrecks/utils.hpp>
#include <tuple>


namespace Wrecks::ECS {
    template <Component ...T>
    class ComponentManager {
        using ComponentMap = std::tuple<ComponentContainer<T>...>;

        ComponentMap m_ComponentTypes;

        public:
            ComponentManager() noexcept
                : m_ComponentTypes()
            {
            }

            template <Utils::ContainsType<T...> C>
            constexpr ComponentContainer<C>& get() noexcept { return std::get<ComponentContainer<C>>(m_ComponentTypes); }

            template <typename Fn>
            inline decltype(auto) forEach(Fn&& fnc) noexcept
            {
                return std::apply([fnc = std::move(fnc)](auto& ...types){
                    (fnc(types), ...);
                }, m_ComponentTypes);
            }
    };
}
