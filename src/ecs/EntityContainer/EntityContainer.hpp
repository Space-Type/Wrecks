/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** EntityContainer class
*/

#pragma once

#include "../Entity.hpp"
#include <wrecks/common.hpp>
#include <wrecks/utils.hpp>
#include <vector>


namespace Wrecks::ECS {
    class EntityContainer {
        /**
         * Note:
         * The signatures vector should be
         * treated as a map<Entity, Signature>
         * in the sense that its indices are
         * entity IDs.
         */
        std::vector<Signature> m_Signatures;
        std::vector<Entity> m_NextID;

        public:
            using iterator = Utils::IndexIterator<typename std::vector<Signature>::iterator>;
            using const_iterator = Utils::IndexIterator<typename std::vector<Signature>::const_iterator>;

        public:
            EntityContainer() noexcept;

            [[nodiscard]] inline std::size_t size() const noexcept { return m_Signatures.size() - m_NextID.size(); }

            Entity create();
            void remove(Entity entity);

            void updateSignature(Entity entity, Signature newSignature) noexcept;
            Signature getSignature(Entity entity) const noexcept;

            inline iterator begin() noexcept { return iterator(m_Signatures.begin(), 0); }
            inline iterator end()   noexcept { return iterator(m_Signatures.end(), m_Signatures.size()); }

            inline const_iterator begin() const noexcept { return const_iterator(m_Signatures.begin(), 0); }
            inline const_iterator end()   const noexcept { return const_iterator(m_Signatures.end(), m_Signatures.size()); }

            inline const_iterator cbegin() const noexcept { return begin(); }
            inline const_iterator cend()   const noexcept { return end(); }
    };
}
