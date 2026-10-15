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
        private:
            template <typename I>
                requires (std::is_same_v<I, typename std::vector<T>::iterator> || std::is_same_v<I, typename std::vector<T>::const_iterator>)
            class ComponentIterator;

        public:
            using ComponentType = T;
            using iterator = ComponentIterator<typename std::vector<ComponentType>::iterator>;
            using const_iterator = ComponentIterator<typename std::vector<ComponentType>::const_iterator>;

        private:
            std::vector<ComponentType> m_Data;
            std::unordered_map<Entity, std::size_t> m_EntityToIndex;
            std::unordered_map<std::size_t, Entity> m_IndexToEntity;

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
                assert(m_EntityToIndex.find(entity) == m_EntityToIndex.end());  // GCOVR_EXCL_BR_LINE

                setIndex(entity, m_Data.size());
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
                assert(!m_Data.empty());  // GCOVR_EXCL_BR_LINE
                assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end());  // GCOVR_EXCL_BR_LINE

                auto index = m_EntityToIndex.at(entity);
                auto lastIndex = m_Data.size() - 1;

                if (index != lastIndex) {
                    auto lastEntity = m_IndexToEntity.at(lastIndex);

                    m_Data[index] = std::move(m_Data[lastIndex]);
                    setIndex(lastEntity, index);
                }

                m_EntityToIndex.erase(entity);
                m_IndexToEntity.erase(lastIndex);

                m_Data.pop_back();
            }

            [[nodiscard]] const ComponentType& operator[](Entity entity) const noexcept
            {
                assert(m_EntityToIndex.find(entity) != m_EntityToIndex.cend());  // GCOVR_EXCL_BR_LINE
                return m_Data[m_EntityToIndex.at(entity)];
            }

            [[nodiscard]] ComponentType& operator[](Entity entity) noexcept
            {
                assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end());  // GCOVR_EXCL_BR_LINE
                return m_Data[m_EntityToIndex.at(entity)];
            }

            [[nodiscard]] constexpr iterator begin() noexcept { return iterator(m_Data.begin(), 0, m_IndexToEntity); }
            [[nodiscard]] constexpr iterator end()   noexcept { return iterator(m_Data.end(), m_Data.size(), m_IndexToEntity); }

            [[nodiscard]] constexpr const_iterator begin() const noexcept { return const_iterator(m_Data.begin(), 0, m_IndexToEntity); }
            [[nodiscard]] constexpr const_iterator end()   const noexcept { return const_iterator(m_Data.end(), m_Data.size(), m_IndexToEntity); }

            [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return begin(); }
            [[nodiscard]] constexpr const_iterator cend()   const noexcept { return end(); }

        private:
            inline void setIndex(Entity id, std::size_t index)
            {
                m_EntityToIndex[id] = index;
                m_IndexToEntity[index] = id;
            }

        private:
            template <typename I>
                requires (std::is_same_v<I, typename std::vector<ComponentType>::iterator> || std::is_same_v<I, typename std::vector<ComponentType>::const_iterator>)
            class ComponentIterator {
                using BaseIterator = I;

                BaseIterator m_BaseIterator;
                std::size_t m_Index;
                const std::unordered_map<std::size_t, Entity>& m_IndexToEntity;

                public:
                    constexpr ComponentIterator(BaseIterator base, std::size_t index, const std::unordered_map<std::size_t, Entity>& indexMap) noexcept
                        : m_BaseIterator(base)
                        , m_Index(index)
                        , m_IndexToEntity(indexMap)
                    {
                    }

                    constexpr std::pair<Entity, ComponentType&> operator*() noexcept
                        requires (std::is_same_v<I, typename std::vector<ComponentType>::iterator>)  // only for non-const
                    {
                        assert(m_IndexToEntity.find(m_Index) != m_IndexToEntity.end());  // GCOVR_EXCL_BR_LINE
                        return { m_IndexToEntity.at(m_Index), *m_BaseIterator };
                    }

                    constexpr std::pair<Entity, const ComponentType&> operator*() const noexcept
                    {
                        assert(m_IndexToEntity.find(m_Index) != m_IndexToEntity.end());  // GCOVR_EXCL_BR_LINE
                        return { m_IndexToEntity.at(m_Index), *m_BaseIterator };
                    }

                    constexpr ComponentIterator& operator++() noexcept
                    {
                        ++m_BaseIterator;
                        ++m_Index;
                        return *this;
                    }

                    constexpr ComponentIterator operator++(int) noexcept
                    {
                        auto cpy = *this;
                        ++(*this);
                        return cpy;
                    }

                    constexpr ComponentIterator& operator--() noexcept
                    {
                        assert(m_Index != 0);  // GCOVR_EXCL_BR_LINE

                        --m_BaseIterator;
                        --m_Index;
                        return *this;
                    }

                    constexpr ComponentIterator& operator--(int) noexcept
                    {
                        auto cpy = *this;
                        --(*this);
                        return cpy;
                    }

                    constexpr bool operator==(const ComponentIterator& other) const noexcept { return m_BaseIterator == other.m_BaseIterator; }
                    constexpr bool operator!=(const ComponentIterator& other) const noexcept { return m_BaseIterator != other.m_BaseIterator; }
            };
    };
}
