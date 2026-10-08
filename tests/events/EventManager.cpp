/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** EventManager class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>

class TestClass : public Wrecks::Events::BaseEventReceiver {
    unsigned int m_InvokeCount;

    public:
        TestClass() : m_InvokeCount(0) {}

        inline unsigned int invokeCount() const noexcept { return m_InvokeCount; }

        void handleEvent(const Wrecks::Events::EntityDeleteEvent& evt) noexcept override
        {
            m_InvokeCount++;
        }
};

TEST(events_EventManager, simpleEvent)
{
    TestClass test;
    Wrecks::Events::EventManager evtManager;

    evtManager.subscribe(test);
    evtManager.sendEvent(Wrecks::Events::EntityDeleteEvent{});

    // ensure event was received
    EXPECT_EQ(test.invokeCount(), 1);
}

TEST(events_EventManager, multipleSubscribers)
{
    TestClass test1;
    TestClass test2;
    Wrecks::Events::EventManager evtManager;

    evtManager.subscribe(test1);
    evtManager.subscribe(test2);
    evtManager.sendEvent(Wrecks::Events::EntityDeleteEvent{});

    EXPECT_EQ(test1.invokeCount(), 1);
    EXPECT_EQ(test2.invokeCount(), 1);
}

TEST(events_EventManager, multipleEvents)
{
    TestClass test;
    Wrecks::Events::EventManager evtManager;

    evtManager.subscribe(test);
    evtManager.sendEvent(Wrecks::Events::EntityDeleteEvent{});
    evtManager.sendEvent(Wrecks::Events::EntityDeleteEvent{});

    // ensure events were both received
    EXPECT_EQ(test.invokeCount(), 2);
}

TEST(events_EventManager, multipleSubscribes)
{
    TestClass test;
    Wrecks::Events::EventManager evtManager;

    evtManager.subscribe(test);
    EXPECT_NO_THROW(evtManager.subscribe(test));
    evtManager.sendEvent(Wrecks::Events::EntityDeleteEvent{});

    // ensure event was only received once
    EXPECT_EQ(test.invokeCount(), 1);
}

TEST(events_EventManager, unsubscribe)
{
    TestClass test;
    Wrecks::Events::EventManager evtManager;

    evtManager.subscribe(test);
    evtManager.unsubscribe(test);
    evtManager.sendEvent(Wrecks::Events::EntityDeleteEvent{});

    EXPECT_EQ(test.invokeCount(), 0);
}

TEST(events_EventManager, multipleUnsubscribe)
{
    TestClass test;
    Wrecks::Events::EventManager evtManager;

    evtManager.subscribe(test);
    evtManager.unsubscribe(test);
    EXPECT_NO_FATAL_FAILURE(evtManager.unsubscribe(test));
    evtManager.sendEvent(Wrecks::Events::EntityDeleteEvent{});

    EXPECT_EQ(test.invokeCount(), 0);
}

TEST(events_EventManager, noSubscribers)
{
    Wrecks::Events::EventManager evtManager;
    EXPECT_NO_FATAL_FAILURE(evtManager.sendEvent(Wrecks::Events::EntityDeleteEvent{}));
}

TEST(events_EventManager, typicalUse)
{
    Wrecks::Events::EventManager evtManager;
    std::array<TestClass, 10> subscribers;

    for (auto& sub : subscribers)
        evtManager.subscribe(sub);

    evtManager.sendEvent(Wrecks::Events::EntityDeleteEvent{});

    for (auto& sub : subscribers)
        EXPECT_EQ(sub.invokeCount(), 1);

    evtManager.unsubscribe(subscribers[2]);
    evtManager.unsubscribe(subscribers[5]);

    evtManager.sendEvent(Wrecks::Events::EntityDeleteEvent{});

    EXPECT_EQ(subscribers[0].invokeCount(), 2);
    EXPECT_EQ(subscribers[2].invokeCount(), 1);
    EXPECT_EQ(subscribers[5].invokeCount(), 1);

    for (auto& sub : subscribers)
        evtManager.unsubscribe(sub);
}
