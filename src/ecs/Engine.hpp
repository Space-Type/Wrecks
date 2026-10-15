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
        Systems m_Systems;
        Components m_Components;
        EntityContainer m_Entities;

        public:
            Engine() noexcept
                : m_Systems()
                , m_Components()
                , m_Entities()
            {}

            template <Utils::TupleContainsType<Systems> S>
            inline S& get() { return std::get<S>(m_Systems); }

            template <Utils::TupleContainsType<Systems> S>
            inline const S& get() const { return std::get<S>(m_Systems); }

            template <ECS::ManagedComponent<Components> C>
            inline ComponentContainer<C>& get() { return m_Components.template get<C>(); }

            template <ECS::ManagedComponent<Components> C>
            inline const C& get() const { return m_Components.template get<C>(); }

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

            void update()
            {
                forEachSystem([](auto& system){
                    system();
                });
            }

        private:
            template <typename Fn>
            void forEachSystem(Fn&& fnc)
            {
                std::apply([fnc = std::move(fnc)](auto& ...systemType){
                    (fnc(systemType), ...);
                }, m_Systems);
            }
    };

    struct SystemA {
        Signature signature() const noexcept { return 0b111111; }
        void operator()() {}
    };
}
