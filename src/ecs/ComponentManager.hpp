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

            template <Utils::ContainsType<T...> C>
            constexpr const ComponentContainer<C>& get() const noexcept { return std::get<ComponentContainer<C>>(m_ComponentTypes); }

            template <Utils::ContainsType<T...> C>
            static consteval std::size_t index() noexcept { return Utils::element_index_v<ComponentContainer<C>, ComponentMap>; }

            static consteval Signature mask() noexcept
            {
                Signature mask;

                for (std::size_t i = 0; i < Utils::container_elements_v<ComponentMap>; i++)
                    mask.set(i);

                return mask;
            }

            template <typename Fn>
            inline decltype(auto) forEach(Fn&& fnc) noexcept
            {
                return std::apply([fnc = std::move(fnc)](auto& ...types){
                    (fnc(types), ...);
                }, m_ComponentTypes);
            }

            template <typename Fn>
            inline decltype(auto) forEach(Fn&& fnc) const noexcept
            {
                return std::apply([fnc = std::move(fnc)](auto& ...types){
                    (fnc(types), ...);
                }, m_ComponentTypes);
            }
    };

    template <typename T>
    struct is_component_manager : std::false_type {};

    template <Component ...T>
    struct is_component_manager<ComponentManager<T...>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_component_manager_v = is_component_manager<T>::value;

    template <typename T>
    concept ComponentManagerType = is_component_manager_v<T>;
}
