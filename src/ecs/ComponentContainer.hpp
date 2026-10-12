/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** ComponentContainer class
*/

#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include <unordered_map>
#include <cassert>
#include <vector>


namespace Wrecks::ECS {
    template <Component T>
    class ComponentContainer {
        public:
            using ComponentType = T;

        private:
            std::vector<ComponentType> m_Data;
            std::unordered_map<Entity::ID, std::size_t> m_EntityToIndex;
            std::unordered_map<Entity::ID, std::size_t> m_IndexToEntity;

        public:
            ComponentContainer() noexcept
                : m_Data()
                , m_EntityToIndex()
            {
            }

            /**
             * Determine the amount of components
             * stored inside of the container
             */
            [[nodiscard]] constexpr std::size_t size() const noexcept { return m_Data.size(); }

            /**
             * Adds component data for a
             * new entity.
             *
             * Note:
             * The entity must not already exist in
             * the array.
             */
            void insert(Entity entity, ComponentType&& data)
            {
                assert(m_EntityToIndex.find(entity.id) == m_EntityToIndex.end());

                setIndex(entity.id, m_Data.size());
                m_Data.push_back(std::move(data));
            }

            /**
             * Removes component data for
             * an existing entity.
             *
             * Note:
             * The entity must exist in the array.
             */
            void remove(Entity entity) noexcept
            {
                assert(!m_Data.empty());
                assert(m_EntityToIndex.find(entity.id) != m_EntityToIndex.end());

                auto index = m_EntityToIndex.at(entity.id);
                auto lastIndex = m_Data.size() - 1;

                if (index != lastIndex) {
                    auto lastEntity = m_IndexToEntity.at(lastIndex);

                    m_Data[index] = std::move(m_Data[lastIndex]);
                    setIndex(lastEntity, index);
                }

                m_Data.pop_back();
            }

            [[nodiscard]] const ComponentType& operator[](Entity::ID id) const noexcept
            {
                assert(m_EntityToIndex.find(id) != m_EntityToIndex.cend());
                return m_Data[m_EntityToIndex.at(id)];
            }

            [[nodiscard]] ComponentType& operator[](Entity::ID id) noexcept
            {
                assert(m_EntityToIndex.find(id) != m_EntityToIndex.end());
                return m_Data[m_EntityToIndex.at(id)];
            }

        private:
            inline void setIndex(Entity::ID id, std::size_t index)
            {
                m_EntityToIndex[id] = index;
                m_IndexToEntity[index] = id;
            }
    };
}
