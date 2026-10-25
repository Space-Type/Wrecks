/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** Engine class
*/

#pragma once

#include "EntityContainer/EntityContainer.hpp"
#include "ComponentManager.hpp"
#include "System.hpp"


namespace Wrecks::ECS {
    template <SystemTuple Systems, ComponentManagerType Components>
    class Engine {
        using SplitSystems = separate_systems<Systems>;

        using UpdateSystems = sort_systems_t<typename SplitSystems::update>;
        using FixedUpdateSystems = sort_systems_t<typename SplitSystems::fixed_update>;
        using RenderSystems = sort_systems_t<typename SplitSystems::render>;

        UpdateSystems m_UpdateSystems;
        FixedUpdateSystems m_FixedUpdateSystems;
        RenderSystems m_RenderSystems;
        Components m_Components;
        EntityContainer m_Entities;

        public:
            Engine() noexcept
                : m_UpdateSystems()
                , m_FixedUpdateSystems()
                , m_RenderSystems()
                , m_Components()
                , m_Entities()
            {}

            /* ----------------- Entities ----------------- */

            inline Signature entitySignature(Entity entity) const noexcept { return m_Entities.getSignature(entity); }

            inline Entity createEntity() { return m_Entities.create(); }

            void deleteEntity(Entity entity)
            {
                m_Components.forEach([&, entity](auto& componentType){
                    using ComponentType = std::remove_cvref_t<decltype(componentType)>::ComponentType;

                    Signature componentSig = Signature();
                    componentSig.set(m_Components.template index<ComponentType>());

                    if (!m_Entities.hasSignature(entity, componentSig))
                        return;

                    componentType.remove(entity);
                });

                m_Entities.remove(entity);
            }

            /* ---------------- Components ---------------- */

            template <Component C>
            void addComponent(Entity entity, C&& data)
            {
                // Note: don't use auto, since LSPs struggle with concepts
                ComponentContainer<C>& dataContainer = m_Components.template get<C>();
                dataContainer.insert(entity, std::move(data));
                m_Entities.addSignature(entity, m_Components.template index<C>());
            }

            template <Component C>
            void removeComponent(Entity entity)
            {
                // Note: don't use auto, since LSPs struggle with concepts
                ComponentContainer<C>& dataContainer = m_Components.template get<C>();
                dataContainer.remove(entity);
                m_Entities.removeSignature(entity, m_Components.template index<C>());
            }

            /* ------------- Update Functions ------------- */

            void update(DeltaTime dt)
            {
                forEachSystem(m_UpdateSystems, [&](auto& system){
                    system.setEngine(this);
                    for (const auto [entity, signature] : m_Entities.filter(system.signature())) {
                        system(entity, dt);
                    }
                });
            }

            void fixedUpdate(DeltaTime dt)
            {
                forEachSystem(m_FixedUpdateSystems, [&](auto& system){
                    system.setEngine(this);
                    for (const auto [entity, signature] : m_Entities.filter(system.signature())) {
                        system(entity, dt);
                    }
                });
            }

            void render(DeltaTime dt)
            {
                forEachSystem(m_RenderSystems, [&](auto& system){
                    system.setEngine(this);
                    for (const auto [entity, signature] : m_Entities.filter(system.signature())) {
                        system(entity, dt);
                    }
                });
            }

            /* ------------------ Getters ----------------- */

            template <Utils::ContainerContainsType<UpdateSystems> S>
            inline S& get() { return std::get<S>(m_UpdateSystems); }

            template <Utils::ContainerContainsType<UpdateSystems> S>
            inline const S& get() const { return std::get<S>(m_UpdateSystems); }

            template <Utils::ContainerContainsType<FixedUpdateSystems> S>
            inline S& get() { return std::get<S>(m_FixedUpdateSystems); }

            template <Utils::ContainerContainsType<FixedUpdateSystems> S>
            inline const S& get() const { return std::get<S>(m_FixedUpdateSystems); }

            template <Utils::ContainerContainsType<RenderSystems> S>
            inline S& get() { return std::get<S>(m_RenderSystems); }

            template <Utils::ContainerContainsType<RenderSystems> S>
            inline const S& get() const { return std::get<S>(m_RenderSystems); }

            template <Utils::ContainerContainsType<Components> C>
            inline ComponentContainer<C>& get() { return m_Components.template get<C>(); }

            template <Utils::ContainerContainsType<Components> C>
            inline const ComponentContainer<C>& get() const { return m_Components.template get<C>(); }

        private:
            template <SystemTuple Tuple, typename Fn>
            static void forEachSystem(Tuple& systems, Fn&& fnc)
            {
                std::apply([fnc = std::move(fnc)](auto& ...systemType){
                    (fnc(systemType), ...);
                }, systems);
            }
    };
}
