/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** ComponentManager class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>

struct TestComponentA {
    int nb;
};

struct TestComponentB {
    char c;
};

using ComponentManager = Wrecks::ECS::ComponentManager<TestComponentA, TestComponentB>;


TEST(ecs_ComponentManager, getComponents)
{
    ComponentManager manager;

    auto& componentTypeA = manager.get<TestComponentA>();
    componentTypeA.insert(0, { .nb = 5 });
    componentTypeA.insert(1, { .nb = 6 });
    ASSERT_EQ(componentTypeA.size(), 2);

    auto& componentTypeB = manager.get<TestComponentB>();
    componentTypeB.insert(0, { .c = 0 + '0' });
    componentTypeB.insert(1, { .c = 1 + '0' });
    ASSERT_EQ(componentTypeB.size(), 2);

    for (auto [entity, component] : manager.get<TestComponentA>()) {
        EXPECT_EQ(entity + 5, component.nb);
    }

    for (auto [entity, component] : manager.get<TestComponentB>()) {
        EXPECT_EQ(entity + '0', component.c);
    }
}

#include <tuple>

TEST(ecs_ComponentManager, forEach)
{
    ComponentManager manager;

    auto& componentTypeA = manager.get<TestComponentA>();
    componentTypeA.insert(0, { .nb = 5 });
    componentTypeA.insert(1, { .nb = 6 });
    ASSERT_EQ(componentTypeA.size(), 2);

    auto& componentTypeB = manager.get<TestComponentB>();
    componentTypeB.insert(0, { .c = 0 + '0' });
    componentTypeB.insert(1, { .c = 1 + '0' });
    ASSERT_EQ(componentTypeB.size(), 2);

    manager.forEach([](auto& type){
        if constexpr (std::is_same_v<std::remove_cvref_t<decltype(type)>, TestComponentA>) {
            for (auto [entity, component] : type) {
                EXPECT_EQ(entity + 5, component.nb);
            }
        } else if constexpr (std::is_same_v<std::remove_cvref_t<decltype(type)>, TestComponentB>) {
            for (auto [entity, component] : type) {
                EXPECT_EQ(entity + '0', component.c);
            }
        }
    });
}
