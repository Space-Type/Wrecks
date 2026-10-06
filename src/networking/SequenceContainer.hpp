/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** SequenceContainer class
*/

#pragma once

#include <wrecks/common.hpp>
#include <type_traits>
#include <array>


namespace Wrecks::Networking {

    /**
     * Sequence container class.
     *
     * Note:
     * For big performance improvements,
     * the specified size should be a
     * power of 2.
     */
    template <typename T, std::size_t S = 1024>
    requires (std::is_default_constructible_v<T>)
    class SequenceContainer {
        static constexpr std::size_t s_Size = S;
        using Element = T;

        using iterator = std::array<Element, s_Size>::iterator;
        using const_iterator = std::array<Element, s_Size>::const_iterator;

        std::array<Element, s_Size> m_Data;
        Tick m_LastTick;

        public:
            constexpr SequenceContainer() noexcept : m_Data() {}

            /**
             * Returns the total size of
             * the container.
             */
            static constexpr std::size_t size() noexcept { return s_Size; }

            /**
             * Replays every element of
             * the sequence starting from
             * the specified tick.
             *
             * Note:
             * Uses CRTP to allow for
             * static polymorphism on
             * `replayTick()`.
             */
            void replayFrom(this auto& self, Tick tick)
            {
                // Nothing to replay
                if (tick > self.m_LastTick)
                    return;

                for (Tick current = tick; tick < self.m_LastTick; tick++)
                    self.replayTick(self[tick]);

                // Note: needs to be handled
                // separately, as `self.m_LastTick`
                // may be `SIZE_MAX`, which would cause
                // an infinite loop if the condition
                // is `tick <= self.m_LastTick`, since
                // `tick` would overflow to 0.
                self.replayTick(self[self.m_LastTick]);
            }

            /**
             * Retrieves the a mutable element
             * at the specified tick, no matter
             * its value.
             *
             * Note:
             * This should only be used
             * to add / modify elements (not read).
             */
            constexpr Element& operator[](Tick tick) noexcept
            {
                m_LastTick = tick;
                return m_Data[tick % s_Size];
            }

            /**
             * Retrieves the sequence element
             * at the specified tick, no matter
             * its value.
             */
            constexpr const Element& operator[](Tick tick) const noexcept { return m_Data[tick % s_Size]; }

            [[nodiscard]] constexpr iterator begin() noexcept { return m_Data.begin(); }
            [[nodiscard]] constexpr iterator end() noexcept { return m_Data.end(); }

            [[nodiscard]] constexpr const_iterator begin() const noexcept { return m_Data.begin(); }
            [[nodiscard]] constexpr const_iterator end() const noexcept { return m_Data.end(); }

            [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return m_Data.cbegin(); }
            [[nodiscard]] constexpr const_iterator cend() const noexcept { return m_Data.cend(); }

        private:
            /**
             * Function to replay a tick.
             *
             * The purpose of this function
             * is to allow static polymorphism
             * in the class' `replayFrom()` function.
             *
             * However, in order for this to work,
             * the function inheriting from this class
             * *must* mark the base class as a friend class.
             */
            constexpr void replayTick(const Element& element) {}
    };
}
