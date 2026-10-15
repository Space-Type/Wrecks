/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** IndexIterator class
*/

#include <gtest/gtest.h>
#include <type_traits>
#include <wrecks.hpp>


TEST(utils_IndexIterator, mutableIterator)
{
    using iterator = Wrecks::Utils::IndexIterator<typename std::vector<int>::iterator>;
    std::vector<int> data = {{ 15, 16, 17, 18 }};

    iterator it = { data.begin(), 0 };
    iterator end = { data.end(), data.size() };

    for (; it != end; ++it) {
        auto [index, value] = *it;
        EXPECT_EQ(value, index + 15);
        value = index;
    }

    EXPECT_EQ(data[0], 0);
    EXPECT_EQ(data[1], 1);
    EXPECT_EQ(data[2], 2);
    EXPECT_EQ(data[3], 3);
}

TEST(utils_IndexIterator, constIterator)
{
    using const_iterator = Wrecks::Utils::IndexIterator<typename std::vector<int>::const_iterator>;
    std::vector<int> data = {{ 15, 16, 17, 18 }};

    const_iterator it = { data.cbegin(), 0 };
    const_iterator end = { data.cend(), data.size() };

    for (; it != end; ++it) {
        auto [index, value] = *it;
        EXPECT_EQ(value, index + 15);
        EXPECT_TRUE(std::is_const_v<std::remove_reference_t<decltype(value)>>);
    }
}
