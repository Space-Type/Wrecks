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


TEST(utils_FilteredIterator, mutableIterator)
{
    std::vector<int> data = {{ 5, 11, 52, -3, 7, 96 }};

    Wrecks::Utils::FilteredIterator it(data.begin(), data.end(), [](int value){
        return value > 10;
    });

    Wrecks::Utils::FilteredIterator end(data.end(), data.end(), it.predicate());

    for (; it != end; ++it) {
        EXPECT_GT(*it, 10);
        *it -= 10;
    }

    EXPECT_EQ(data[0], 5);
    EXPECT_EQ(data[1], 1);
    EXPECT_EQ(data[2], 42);
    EXPECT_EQ(data[3], -3);
    EXPECT_EQ(data[4], 7);
    EXPECT_EQ(data[5], 86);
}

TEST(utils_FilteredIterator, constIterator)
{
    std::vector<int> data = {{ 14, 5, 11, 52, -3, 7, 96 }};

    Wrecks::Utils::FilteredIterator it(data.cbegin(), data.cend(), [](int value){
        return value > 10;
    });

    Wrecks::Utils::FilteredIterator end(data.cend(), data.cend(), it.predicate());

    for (; it != end; ++it) {
        EXPECT_TRUE(std::is_const_v<std::remove_reference_t<decltype(*it)>>);
        EXPECT_GT(*it, 10);
    }
}
