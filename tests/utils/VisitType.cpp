/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** visitType function
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>


using TestVariant = std::variant<int, float, double, char>;


TEST(utils_VisitType, intIndex)
{
    EXPECT_NO_THROW(Wrecks::Utils::visitType<TestVariant>([](auto type){
        using VariantType = typename decltype(type)::type;
        bool same = std::is_same_v<VariantType, int>;
        EXPECT_TRUE(same);
    }, 0));
}

TEST(utils_VisitType, floatIndex)
{
    EXPECT_NO_THROW(Wrecks::Utils::visitType<TestVariant>([](auto type){
        using VariantType = typename decltype(type)::type;
        bool same = std::is_same_v<VariantType, float>;
        EXPECT_TRUE(same);
    }, 1));
}

TEST(utils_VisitType, doubleIndex)
{
    EXPECT_NO_THROW(Wrecks::Utils::visitType<TestVariant>([](auto type){
        using VariantType = typename decltype(type)::type;
        bool same = std::is_same_v<VariantType, double>;
        EXPECT_TRUE(same);
    }, 2));
}

TEST(utils_VisitType, charIndex)
{
    EXPECT_NO_THROW(Wrecks::Utils::visitType<TestVariant>([](auto type){
        using VariantType = typename decltype(type)::type;
        bool same = std::is_same_v<VariantType, char>;
        EXPECT_TRUE(same);
    }, 3));
}

TEST(utils_VisitType, invalidIndex)
{
    EXPECT_THROW(
        Wrecks::Utils::visitType<TestVariant>([](auto type){}, 50),
        std::out_of_range
    );
}
