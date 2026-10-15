/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** EntityContainer class
*/

#include "wrecks/common.hpp"
#include <gtest/gtest.h>
#include <type_traits>
#include <wrecks.hpp>


TEST(ecs_EntityContainer, addSignature)
{
    Wrecks::ECS::EntityContainer entities;
    auto entity = entities.create();

    entities.addSignature(entity, 2);
    entities.addSignature(entity, 0);
    EXPECT_EQ(entities.getSignature(entity), 0b101);

    entities.removeSignature(entity, 2);
    entities.addSignature(entity, 3);
    EXPECT_EQ(entities.getSignature(entity), 0b1001);
}

TEST(ecs_EntityContainer, hasSignature)
{
    Wrecks::ECS::EntityContainer entities;
    auto entity = entities.create();

    entities.addSignature(entity, 2);
    entities.addSignature(entity, 0);
    ASSERT_EQ(entities.getSignature(entity), 0b101);
    EXPECT_TRUE(entities.hasSignature(entity, 0b100));
    EXPECT_TRUE(entities.hasSignature(entity, 0b001));
    EXPECT_TRUE(entities.hasSignature(entity, 0b101));
    EXPECT_FALSE(entities.hasSignature(entity, 0b011));

    entities.removeSignature(entity, 2);
    entities.addSignature(entity, 3);
    ASSERT_EQ(entities.getSignature(entity), 0b1001);
    EXPECT_TRUE(entities.hasSignature(entity, 0b0001));
    EXPECT_TRUE(entities.hasSignature(entity, 0b1000));
    EXPECT_TRUE(entities.hasSignature(entity, 0b1001));
    EXPECT_FALSE(entities.hasSignature(entity, 0b1011));
}

TEST(ecs_EntityContainer, completeEntityCreation)
{
    Wrecks::ECS::EntityContainer entities;

    auto entity1 = entities.create();
    auto entity2 = entities.create();
    auto entity3 = entities.create();
    ASSERT_EQ(entities.size(), 3);

    for (auto [entity, signature] : entities) {
        // make sure the signature is empty
        EXPECT_EQ(signature, Wrecks::Signature());
        EXPECT_TRUE(entity == entity1 || entity == entity2 || entity == entity3);

        entities.setSignature(entity, 0b1101);
    }

    EXPECT_EQ(entities.getSignature(entity1), 0b1101);
    EXPECT_EQ(entities.getSignature(entity2), 0b1101);
    EXPECT_EQ(entities.getSignature(entity3), 0b1101);

    entities.remove(entity1);
    EXPECT_EQ(entities.size(), 2);

    // ensure entity creation reuses unused IDs
    EXPECT_EQ(entity1, entities.create());
    EXPECT_EQ(entities.getSignature(entity1), Wrecks::Signature());
}

TEST(ecs_EntityContainer, constIterator)
{
    Wrecks::ECS::EntityContainer entities;
    const auto& cnstEntities = entities;

    auto entity1 = entities.create();
    auto entity2 = entities.create();
    auto entity3 = entities.create();
    ASSERT_EQ(entities.size(), 3);

    for (auto [entity, signature] : cnstEntities) {
        EXPECT_TRUE(std::is_const_v<std::remove_reference_t<decltype(signature)>>);

        // make sure the signature is empty
        EXPECT_EQ(signature, Wrecks::Signature());
        EXPECT_TRUE(entity == entity1 || entity == entity2 || entity == entity3);
    }
}
