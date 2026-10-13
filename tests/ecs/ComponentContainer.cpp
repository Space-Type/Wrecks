/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** ComponentData class
*/

#include <gtest/gtest.h>
#include <type_traits>
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

    data.insert(entityToRemove, { .value = 52 });
    data.insert(entity, { .value = 32 });
    ASSERT_EQ(data.size(), 2);

    data.remove(entityToRemove);
    EXPECT_EQ(data.size(), 1);

    EXPECT_EQ(data[entity.id].value, 32);
}

TEST(ecs_ComponentData, removeAndAddBack)
{
    Wrecks::ECS::ComponentContainer<TestComponent> data;

    data.insert({ .id = 5 }, { .value = 2 });
    data.insert({ .id = 6 }, { .value = 1 });
    data.insert({ .id = 7 }, { .value = 0 });
    ASSERT_EQ(data.size(), 3);

    data.remove({ .id = 6 });
    EXPECT_EQ(data.size(), 2);

    data.insert({ .id = 6 }, { .value = 3 });
    EXPECT_EQ(data.size(), 3);
    EXPECT_EQ(data[5].value, 2);
    EXPECT_EQ(data[6].value, 3);
    EXPECT_EQ(data[7].value, 0);
}

TEST(ecs_ComponentData, mutableIterator)
{
    Wrecks::ECS::ComponentContainer<TestComponent> data;

    data.insert({ .id =  5 }, { .value = 0 });
    data.insert({ .id =  2 }, { .value = 1 });
    data.insert({ .id =  9 }, { .value = 2 });
    data.insert({ .id = 17 }, { .value = 3 });
    ASSERT_EQ(data.size(), 4);

    data.remove({ .id = 2 });

    for (auto [id, cmpData] : data) {
        EXPECT_FALSE(std::is_const_v<std::remove_reference_t<decltype(cmpData)>>);

        switch (id) {
            case 5:  EXPECT_EQ(cmpData.value, 0); cmpData.value = 15; break;
            case 2:  EXPECT_EQ(cmpData.value, 1); break;
            case 9:  EXPECT_EQ(cmpData.value, 2); break;
            case 17: EXPECT_EQ(cmpData.value, 3); break;
            default: EXPECT_TRUE(false); break;
        }
    }

    EXPECT_EQ(data[5].value, 15);
}

TEST(ecs_ComponentData, constIterator)
{
    Wrecks::ECS::ComponentContainer<TestComponent> data;
    const auto& cnstData = data;

    data.insert({ .id =  5 }, { .value = 0 });
    data.insert({ .id =  2 }, { .value = 1 });
    data.insert({ .id =  9 }, { .value = 2 });
    data.insert({ .id = 17 }, { .value = 3 });
    ASSERT_EQ(data.size(), 4);

    data.remove({ .id = 2 });

    for (auto [id, cmpData] : cnstData) {
        EXPECT_TRUE(std::is_const_v<std::remove_reference_t<decltype(cmpData)>>);

        switch (id) {
            case 5:  EXPECT_EQ(cmpData.value, 0); break;
            case 2:  EXPECT_EQ(cmpData.value, 1); break;
            case 9:  EXPECT_EQ(cmpData.value, 2); break;
            case 17: EXPECT_EQ(cmpData.value, 3); break;
            default: EXPECT_TRUE(false); break;
        }
    }
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
