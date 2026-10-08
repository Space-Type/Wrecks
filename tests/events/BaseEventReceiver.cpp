/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** BaseEventReceiver class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>
#include <type_traits>


TEST(events_BaseEventReceiver, isConcreteClass)
{
    EXPECT_FALSE(std::is_abstract_v<Wrecks::Events::BaseEventReceiver>);
}
