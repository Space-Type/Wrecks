/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** Engine class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>


struct Engine;

struct TestComponentA { int a; };
struct TestComponentB {};


struct TestSystem {
    bool wasInvoked = false;

    constexpr Wrecks::Signature signature() const noexcept { return 0b11; }

    void setEngine(void*) noexcept {}

    void operator()(Wrecks::ECS::Entity entity)
    {
        wasInvoked = true;
    }
};

struct TestStateSystem : TestSystem {
    static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = Wrecks::ECS::SystemAttributes::STATE };
};

struct TestRenderSystem : TestSystem {
    static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = Wrecks::ECS::SystemAttributes::RENDER };
};

using Systems = std::tuple<TestStateSystem, TestRenderSystem>;
using Components = Wrecks::ECS::ComponentManager<TestComponentA, TestComponentB>;
struct Engine : Wrecks::ECS::Engine<Systems, Components> {
    using Wrecks::ECS::Engine<Systems, Components>::Engine;
};


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
    const auto& testStateSystem = engine.get<TestStateSystem>();
    const auto& testRenderSystem = engine.get<TestRenderSystem>();
    auto entity = engine.createEntity();

    engine.addComponent(entity, TestComponentA{ .a = 1 });
    engine.addComponent(entity, TestComponentB{});

    EXPECT_FALSE(testStateSystem.wasInvoked);
    engine.update();
    EXPECT_TRUE(testStateSystem.wasInvoked);

    EXPECT_FALSE(testRenderSystem.wasInvoked);
    engine.render();
    EXPECT_TRUE(testRenderSystem.wasInvoked);
}

TEST(ecs_Engine, systemUpdateNoSystems)
{
    Wrecks::ECS::Engine<std::tuple<>, Components> engine;
    auto entity = engine.createEntity();

    engine.addComponent(entity, TestComponentA{ .a = 1 });
    engine.addComponent(entity, TestComponentB{});

    EXPECT_NO_FATAL_FAILURE(
        engine.update();
        engine.render();
    );
}

TEST(ecs_Engine, noSystemUpdate)
{
    Engine engine;
    const auto& testStateSystem = engine.get<TestStateSystem>();
    auto entity = engine.createEntity();

    engine.addComponent(entity, TestComponentB{});
    ASSERT_EQ(engine.entitySignature(entity), 0b10);

    EXPECT_FALSE(testStateSystem.wasInvoked);
    engine.update();
    EXPECT_FALSE(testStateSystem.wasInvoked);
}
