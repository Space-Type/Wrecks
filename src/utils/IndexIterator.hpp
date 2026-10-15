/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** IndexIterator class
*/

#pragma once

#include <type_traits>
#include <utility>


namespace Wrecks::Utils {
    template <typename T>
    class IndexIterator {
        T m_BaseIterator;
        std::size_t m_Index;

        static constexpr bool IS_CONST = std::is_const_v<std::remove_reference_t<decltype(*m_BaseIterator)>>;
        using Element = std::remove_cvref_t<decltype(*m_BaseIterator)>;

        public:
            constexpr IndexIterator(T base, std::size_t index = 0)
                : m_BaseIterator(base)
                , m_Index(index)
            {
            }

            constexpr std::pair<std::size_t, Element&> operator*() noexcept
                requires (!IS_CONST)
            {
                return { m_Index, *m_BaseIterator };
            }

            constexpr std::pair<std::size_t, const Element&> operator*() const noexcept
            {
                return { m_Index, *m_BaseIterator };
            }

            constexpr IndexIterator& operator++() noexcept
            {
                ++m_BaseIterator;
                ++m_Index;
                return *this;
            }

            constexpr IndexIterator operator++(int) noexcept
            {
                auto cpy = *this;
                ++(*this);
                return cpy;
            }

            constexpr IndexIterator& operator--() noexcept
                requires (--m_BaseIterator)
            {
                --m_BaseIterator;
                --m_Index;
                return *this;
            }

            constexpr IndexIterator operator--(int) noexcept
                requires (--m_BaseIterator)
            {
                auto cpy = *this;
                --(*this);
                return cpy;
            }

            constexpr bool operator==(const IndexIterator& other) const noexcept { return m_BaseIterator == other.m_BaseIterator; }
            constexpr bool operator!=(const IndexIterator& other) const noexcept { return m_BaseIterator != other.m_BaseIterator; }
    };
}
