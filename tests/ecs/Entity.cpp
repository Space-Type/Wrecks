/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** Entity struct
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>


TEST(ecs_Entity, entityTraits)
{
    EXPECT_TRUE(std::is_trivially_copyable_v<Wrecks::ECS::Entity>);
}
