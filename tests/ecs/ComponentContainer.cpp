/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** ComponentData class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>
#include <csignal>


struct TestComponent {
    int value;
};

TEST(ecs_ComponentData, insertData)
{
    Wrecks::ECS::Entity entity = { .id = 0 };

    Wrecks::ECS::ComponentContainer<TestComponent> data;
    EXPECT_EQ(data.size(), 0);

    data.insert(entity, { .value = 32 });
    EXPECT_EQ(data.size(), 1);

    EXPECT_EQ(data[entity.id].value, 32);

    const auto& cnstData = data;
    EXPECT_EQ(cnstData[entity.id].value, 32);
}

TEST(ecs_ComponentData, removeSoleEntity)
{
    Wrecks::ECS::ComponentContainer<TestComponent> data;
    Wrecks::ECS::Entity entity = { .id = 0 };

    data.insert(entity, { .value = 32 });
    ASSERT_EQ(data.size(), 1);

    data.remove(entity);
    EXPECT_EQ(data.size(), 0);
}

TEST(ecs_ComponentData, removeEntity)
{
    Wrecks::ECS::ComponentContainer<TestComponent> data;
    Wrecks::ECS::Entity entity = { .id = 0 };
    Wrecks::ECS::Entity entityToRemove = { .id = 1 };

    ASSERT_EQ(data.size(), 0);
    data.insert(entityToRemove, { .value = 52 });
    ASSERT_EQ(data.size(), 1);
    data.insert(entity, { .value = 32 });
    ASSERT_EQ(data.size(), 2);

    data.remove(entityToRemove);
    EXPECT_EQ(data.size(), 1);

    EXPECT_EQ(data[entity.id].value, 32);
}

TEST(ecs_ComponentData, accessMissingEntityDeathTest)
{
    Wrecks::ECS::ComponentContainer<TestComponent> data;
    Wrecks::ECS::Entity entity = { .id = 0 };

    EXPECT_EXIT((void)data[entity.id], testing::KilledBySignal(SIGABRT), "");
}

TEST(ecs_ComponentData, insertExistingEntityDeathTest)
{
    Wrecks::ECS::ComponentContainer<TestComponent> data;
    Wrecks::ECS::Entity entity = { .id = 0 };

    data.insert(entity, { .value = 32 });
    EXPECT_EXIT(data.insert(entity, { .value = 52 }), testing::KilledBySignal(SIGABRT), "");
}

TEST(ecs_ComponentData, removeEmptyContainerDeathTest)
{
    Wrecks::ECS::ComponentContainer<TestComponent> data;

    EXPECT_EQ(data.size(), 0);
    EXPECT_EXIT(data.remove({ .id = 0 }), testing::KilledBySignal(SIGABRT), "");
}

TEST(ecs_ComponentData, removeMissingEntityDeathTest)
{
    Wrecks::ECS::ComponentContainer<TestComponent> data;

    data.insert({ .id = 1 }, { .value = 5 });
    EXPECT_EXIT(data.remove({ .id = 0 }), testing::KilledBySignal(SIGABRT), "");
}
