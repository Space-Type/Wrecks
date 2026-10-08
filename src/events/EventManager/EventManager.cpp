/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Implementation for
** EventManager class
*/

#include "EventManager.hpp"
#include <algorithm>
#include <iterator>


Wrecks::Events::EventManager::EventManager()
    : m_Subscribers()
{

}

/**
 * Sends the specified event to all
 * subscribed event receivers.
 */
void Wrecks::Events::EventManager::sendEvent(Event event)
{
    std::visit([&](const auto& data){
        for (auto* subscriber : m_Subscribers) {
            subscriber->handleEvent(data);
        }
    }, event);
}

/**
 * Subscribes the receiver to
 * events sent to this event
 * manager. If the receiver is
 * already subscribed, this does
 * nothing.
 */
void Wrecks::Events::EventManager::subscribe(IEventReceiver& receiver)
{
    long index = findIndex(&receiver);
    if (index >= 0)
        return;

    m_Subscribers.push_back(&receiver);
}

/**
 * Unsubscribes a receiver from
 * events sent to this event manager.
 * If the receiver wasn't subscribed
 * to begin with, nothing is done.
 */
void Wrecks::Events::EventManager::unsubscribe(IEventReceiver& receiver)
{
    if (m_Subscribers.empty())
        return;

    auto index = findIndex(&receiver);
    if (index < 0)
        return;

    decltype(index) lastIndex = m_Subscribers.size() - 1;

    if (index != lastIndex)
        m_Subscribers[index] = m_Subscribers[lastIndex];
    m_Subscribers.pop_back();
}

/**
 * Finds the index of the specified
 * receiver (if present), and returns
 * it.
 *
 * A negative index means the specified
 * receiver isn't subscribed to the
 * manager.
 */
long Wrecks::Events::EventManager::findIndex(IEventReceiver* receiver) const
{
    auto it = std::find(m_Subscribers.cbegin(), m_Subscribers.cend(), receiver);
    if (it == m_Subscribers.cend())
        return - 1;  // receiver is not subscribed

    return std::distance(m_Subscribers.cbegin(), it);
}
