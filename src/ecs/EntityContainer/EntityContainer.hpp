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
#include <functional>
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

        private:
            template <typename Iterable>
            class FilteredContainerView;

        public:
            using iterator = Utils::IndexIterator<typename std::vector<Signature>::iterator>;
            using const_iterator = Utils::IndexIterator<typename std::vector<Signature>::const_iterator>;

        public:
            EntityContainer() noexcept;

            [[nodiscard]] inline std::size_t size() const noexcept { return m_Signatures.size() - m_NextID.size(); }

            Entity create();
            void remove(Entity entity);

            void addSignature(Entity entity, unsigned int bitPosition) noexcept;
            void removeSignature(Entity entity, unsigned int bitPosition) noexcept;

            void setSignature(Entity entity, Signature newSignature) noexcept;
            [[nodiscard]] Signature getSignature(Entity entity) const noexcept;
            [[nodiscard]] bool hasSignature(Entity entity, Signature sig) const noexcept;

            inline FilteredContainerView<EntityContainer> filter(Signature signature) noexcept { return { signature, *this }; }
            inline FilteredContainerView<const EntityContainer> filter(Signature signature) const noexcept { return { signature, *this }; }

            inline iterator begin() noexcept { return iterator(m_Signatures.begin(), 0); }
            inline iterator end()   noexcept { return iterator(m_Signatures.end(), m_Signatures.size()); }

            inline const_iterator begin() const noexcept { return const_iterator(m_Signatures.begin(), 0); }
            inline const_iterator end()   const noexcept { return const_iterator(m_Signatures.end(), m_Signatures.size()); }

            inline const_iterator cbegin() const noexcept { return begin(); }
            inline const_iterator cend()   const noexcept { return end(); }

        private:
            template <typename Iterable>
            class FilteredContainerView {
                Iterable& m_Container;
                std::function<bool(std::pair<Entity, const Signature&>)> m_SignatureFilter;

                public:
                    using iterator = Utils::FilteredIterator<decltype(m_Container.begin()), decltype(m_SignatureFilter)>;
                    using const_iterator = Utils::FilteredIterator<decltype(m_Container.cbegin()), decltype(m_SignatureFilter)>;

                public:
                    FilteredContainerView(Signature sig, Iterable& container) noexcept
                        : m_Container(container)
                        , m_SignatureFilter([sig](std::pair<Entity, const Signature&> data) noexcept {
                            return (sig & data.second) == sig;
                        })
                    {
                    }

                    inline iterator begin() noexcept requires (!std::is_const_v<Iterable>) { return iterator(m_Container.begin(), m_Container.end(), m_SignatureFilter); }
                    inline iterator end()   noexcept requires (!std::is_const_v<Iterable>) { return iterator(m_Container.end(), m_Container.end(), m_SignatureFilter); }

                    inline const_iterator begin() noexcept { return const_iterator(m_Container.begin(), m_Container.end(), m_SignatureFilter); }
                    inline const_iterator end()   noexcept { return const_iterator(m_Container.end(), m_Container.end(), m_SignatureFilter); }

                    inline const_iterator cbegin() noexcept { return const_iterator(m_Container.begin(), m_Container.end(), m_SignatureFilter); }
                    inline const_iterator cend()   noexcept { return const_iterator(m_Container.end(), m_Container.end(), m_SignatureFilter); }
            };
    };
}
