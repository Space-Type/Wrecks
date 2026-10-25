/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** System utils
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>

using Wrecks::ECS::SystemAttributes;

struct SystemTemplate {
    constexpr Wrecks::Signature signature() const noexcept { return 0b11; }
    void setEngine(void*) noexcept {}
    void operator()(Wrecks::ECS::Entity entity) {}
};

struct State0  : SystemTemplate { static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = SystemAttributes::STATE,  .order = 0 }; };
struct State1  : SystemTemplate { static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = SystemAttributes::STATE,  .order = 1 }; };
struct State2  : SystemTemplate { static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = SystemAttributes::STATE,  .order = 2 }; };
struct Render0 : SystemTemplate { static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = SystemAttributes::RENDER, .order = 0 }; };
struct Render1 : SystemTemplate { static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = SystemAttributes::RENDER, .order = 1 }; };
struct Render2 : SystemTemplate { static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = SystemAttributes::RENDER, .order = 2 }; };


TEST(ecs_System, systemSeparation)
{
    using Systems = std::tuple<State0, Render0, State1, Render1>;
    using SplitSystems = Wrecks::ECS::separate_systems<Systems>;

    // Note: EXPECT() functions don't support commas
    constexpr bool stateTupleValid  = std::is_same_v<typename SplitSystems::state, std::tuple<State0, State1>>;
    EXPECT_TRUE(stateTupleValid);

    constexpr bool renderTupleValid = std::is_same_v<typename SplitSystems::render, std::tuple<Render0, Render1>>;
    EXPECT_TRUE(renderTupleValid);
}

TEST(ecs_System, systemSeparationSingle)
{
    using Systems = std::tuple<State0>;
    using SplitSystems = Wrecks::ECS::separate_systems<Systems>;

    // Note: EXPECT() functions don't support commas
    constexpr bool stateTupleValid  = std::is_same_v<typename SplitSystems::state, std::tuple<State0>>;
    EXPECT_TRUE(stateTupleValid);

    constexpr bool renderTupleValid = std::is_same_v<typename SplitSystems::render, std::tuple<>>;
    EXPECT_TRUE(renderTupleValid);
}

TEST(ecs_System, emptySystemSeparation)
{
    using Systems = std::tuple<>;
    using SplitSystems = Wrecks::ECS::separate_systems<Systems>;

    // Note: EXPECT() functions don't support commas
    constexpr bool stateTupleValid  = std::is_same_v<typename SplitSystems::state, std::tuple<>>;
    EXPECT_TRUE(stateTupleValid);

    constexpr bool renderTupleValid = std::is_same_v<typename SplitSystems::render, std::tuple<>>;
    EXPECT_TRUE(renderTupleValid);
}

TEST(ecs_System, sortTuple)
{
    using Systems         = std::tuple<Render2, State1, Render1, Render0, State2, State0>;
    using ExpectedSystems = std::tuple<Render0, State0, State1, Render1, Render2, State2>;
    using SortedSystems   = Wrecks::ECS::sort_systems_t<Systems>;

    // Note: EXPECT() functions don't support commas
    constexpr bool isSorted = std::is_same_v<SortedSystems, ExpectedSystems>;
    EXPECT_TRUE(isSorted);
}

TEST(ecs_System, sortSinglePassTuple)
{
    using Systems = std::tuple<State1, State0, State2>;
    using SortedSystems = Wrecks::ECS::sort_systems_t<Systems>;

    // Note: EXPECT() functions don't support commas
    constexpr bool isSorted = std::is_same_v<SortedSystems, std::tuple<State0, State1, State2>>;
    EXPECT_TRUE(isSorted);
}

TEST(ecs_System, sortEmptyTuple)
{
    using Systems = std::tuple<>;
    using SortedSystems = Wrecks::ECS::sort_systems_t<Systems>;

    // Note: EXPECT() functions don't support commas
    constexpr bool isSorted = std::is_same_v<SortedSystems, std::tuple<>>;
    EXPECT_TRUE(isSorted);
}
