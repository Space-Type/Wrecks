/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** UDPSocket class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>

using Wrecks::Utils::SequenceContainer;

TEST(utils_SequenceContainer, sequenceSize)
{
    constexpr std::size_t size = 52;

    const SequenceContainer<int> defaultContainer;
    const SequenceContainer<int, size> sizedContainer;

    EXPECT_EQ(defaultContainer.size(), 1024);
    EXPECT_EQ(sizedContainer.size(), size);
}

TEST(utils_SequenceContainer, mutableIterator)
{
    constexpr int value = 42;
    SequenceContainer<int> container;

    std::fill(container.begin(), container.end(), value);

    for (auto& element : container) {
        EXPECT_EQ(element, value);
        element += 1;  // ensure mutating it doesn't cause memory errors
        EXPECT_EQ(element, value + 1);
    }
}

TEST(utils_SequenceContainer, constIterator)
{
    constexpr int value = 42;
    SequenceContainer<int> containerStore;

    std::fill(containerStore.begin(), containerStore.end(), value);

    const SequenceContainer<int>& container = containerStore;

    for (auto& element : container) {
        EXPECT_EQ(element, value);
        EXPECT_TRUE(std::is_const<std::remove_reference_t<decltype(element)>>());
    }

    // test explicit const iterator functions
    EXPECT_TRUE(std::is_const<std::remove_reference_t<decltype(*container.cbegin())>>());
    EXPECT_TRUE(std::is_const<std::remove_reference_t<decltype(*container.cend())>>());
}

template <int V>
class TestContainer : public SequenceContainer<int> {
    friend class SequenceContainer<int>;

    public:
        using SequenceContainer<int>::SequenceContainer;

    private:
        void replayTick(const int& value) { EXPECT_EQ(value, V); }
};

TEST(utils_SequenceContainer, tickReplay)
{
    constexpr int value = 42;
    TestContainer<value> container;

    container[8952] = value;
    container[8953] = value;
    container[8954] = value;
    container[8955] = value;
    container[8956] = value;

    container.replayFrom(8952);
}

TEST(utils_SequenceContainer, futureTickReplay)
{
    constexpr int value = 42;
    TestContainer<value> container;

    container[8952] = value;
    container[8953] = value;
    container[8954] = value;
    container[8955] = value;
    container[8956] = value;

    container.replayFrom(8957);
}

TEST(utils_SequenceContainer, tickReplayEdgeCase)
{
    constexpr int value = 42;
    TestContainer<value> container;

    container[SIZE_MAX - 2] = value;
    container[SIZE_MAX - 1] = value;
    container[SIZE_MAX - 0] = value;

    // If replayFrom() is implement wrong,
    // this will cause an infinite loop.
    container.replayFrom(SIZE_MAX);
}
