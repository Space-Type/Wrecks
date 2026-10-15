/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** Engine class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>


struct TestComponentA { int a; };
struct TestComponentB {};

struct TestSystem;

struct TestSystem {
    bool wasInvoked = false;

    constexpr Wrecks::Signature signature() const noexcept { return 0b11; }
    void setEngine(void*) const noexcept {}

    void operator()(Wrecks::ECS::Entity entity)
    {
        wasInvoked = true;
    }
};

using Systems = std::tuple<TestSystem>;
using Components = Wrecks::ECS::ComponentManager<TestComponentA, TestComponentB>;
using Engine = Wrecks::ECS::Engine<Systems, Components>;


TEST(ecs_Engine, createEntity)
{
    Engine engine;
    auto entity = engine.createEntity();

    engine.addComponent(entity, TestComponentA{ .a = 42 });
    engine.addComponent(entity, TestComponentB{});
    EXPECT_EQ(engine.entitySignature(entity), 0b11);

    const auto& component = engine.get<TestComponentA>();
    EXPECT_EQ(component.size(), 1);
    EXPECT_EQ(component[entity].a, 42);

    engine.removeComponent<TestComponentA>(entity);
    EXPECT_EQ(engine.entitySignature(entity), 0b10);
}

TEST(ecs_Engine, deleteEntity)
{
    Engine engine;
    auto entity = engine.createEntity();
    auto entity2 = engine.createEntity();
    auto entity3 = engine.createEntity();

    engine.addComponent(entity, TestComponentA{ .a = 1 });
    engine.addComponent(entity, TestComponentB{});
    EXPECT_EQ(engine.entitySignature(entity), 0b11);
    EXPECT_EQ(engine.get<TestComponentA>().size(), 1);
    EXPECT_EQ(engine.get<TestComponentB>().size(), 1);

    engine.deleteEntity(entity);

    auto newEntity = engine.createEntity();
    EXPECT_EQ(entity, newEntity);
    EXPECT_EQ(engine.entitySignature(entity), 0b00);
    EXPECT_EQ(engine.get<TestComponentA>().size(), 0);
    EXPECT_EQ(engine.get<TestComponentB>().size(), 0);

    engine.deleteEntity(newEntity);
    engine.deleteEntity(entity2);
    engine.deleteEntity(entity3);
}

TEST(ecs_Engine, systemUpdate)
{
    Engine engine;
    const auto& testSystem = engine.get<TestSystem>();
    auto entity = engine.createEntity();

    engine.addComponent(entity, TestComponentA{ .a = 1 });
    engine.addComponent(entity, TestComponentB{});

    EXPECT_FALSE(testSystem.wasInvoked);
    engine.update();
    EXPECT_TRUE(testSystem.wasInvoked);
}

TEST(ecs_Engine, noSystemUpdate)
{
    Engine engine;
    const auto& testSystem = engine.get<TestSystem>();
    auto entity = engine.createEntity();

    engine.addComponent(entity, TestComponentB{});
    ASSERT_EQ(engine.entitySignature(entity), 0b10);

    EXPECT_FALSE(testSystem.wasInvoked);
    engine.update();
    EXPECT_FALSE(testSystem.wasInvoked);
}
