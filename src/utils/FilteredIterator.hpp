/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** FilteredIterator class
*/

#pragma once

#include <functional>


namespace Wrecks::Utils {
    template <typename T, typename Fn>
    class FilteredIterator {
        T m_BaseIter;
        T m_End;  // note: required to avoid skipping past end
        Fn m_Predicate;

        static constexpr bool IS_CONST = std::is_const_v<std::remove_reference_t<decltype(*m_BaseIter)>>;
        using Element = decltype(*m_BaseIter);

        public:
            constexpr FilteredIterator(T baseIter, T end, Fn predicate)
                : m_BaseIter(baseIter)
                , m_End(end)
                , m_Predicate(std::move(predicate))
            {
                skipToNext();
            }

            constexpr Fn& predicate() noexcept { return m_Predicate; }
            constexpr const Fn& predicate() const noexcept { return m_Predicate; }

            constexpr Element operator*() noexcept requires (!IS_CONST) { return *m_BaseIter; }
            constexpr Element operator*() const noexcept { return *m_BaseIter; }

            constexpr FilteredIterator& operator++() noexcept
            {
                ++m_BaseIter;
                skipToNext();
                return *this;
            }

            constexpr FilteredIterator operator++(int) noexcept
            {
                auto cpy = *this;
                ++(*this);
                return cpy;
            }

            constexpr bool operator==(const FilteredIterator& other) const noexcept { return m_BaseIter == other.m_BaseIter; }
            constexpr bool operator!=(const FilteredIterator& other) const noexcept { return m_BaseIter != other.m_BaseIter; }

        private:
            /**
             * Moves the iterator to the
             * first value that satisfies
             * the predicate.
             * If the current value satisfies
             * it, nothing happens.
             */
            constexpr void skipToNext() noexcept
            {
                while (m_BaseIter != m_End && !std::invoke(m_Predicate, *m_BaseIter)) {
                    ++m_BaseIter;
                }
            }
    };
}

