/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** Engine class
*/

#include "wrecks/common.hpp"
#include <gtest/gtest.h>
#include <wrecks.hpp>


struct TestSystem {
    bool wasInvoked = false;

    constexpr Wrecks::Signature signature() const noexcept { return Wrecks::Signature(0b1); }

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
