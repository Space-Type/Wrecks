/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** Engine class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>


struct TestSystem {
    bool wasInvoked = false;

    void operator()()
    {
        wasInvoked = true;
    }
};

using TestSystemTuple = std::tuple<TestSystem>;

TEST(ecs_Engine, engineUpdate)
{
    Wrecks::ECS::Engine<TestSystemTuple> engine;
    const auto& testSystem = engine.system<TestSystem>();

    EXPECT_FALSE(testSystem.wasInvoked);
    engine.update();
    EXPECT_TRUE(testSystem.wasInvoked);
}
