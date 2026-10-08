/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** EventManager class
*/

#pragma once

#include "../IEventReceiver.hpp"
#include <vector>


namespace Wrecks::Events {
    /**
     * Event manager.
     *
     * Allows sending events to every
     * event receiver subscribed to
     * he instance.
     */
    class EventManager {
        /**
         * Note:
         * We use a vector instead of using a set/unordered_set,
         * since the "main" operation of the event manager is to
         * send events. As such, using a container with the
         * fastest iteration speed is best for performance, and,
         * as it so happens, vectors are best for it.
         *
         * This does mean that subscribing and unsubscribing
         * is slower than with a set/unordered_set, since we
         * need to loop over every subscribed element to ensure
         * it doesn't already exist (which is slow).
         */
        std::vector<IEventReceiver*> m_Subscribers;

        public:
            EventManager();

            void sendEvent(Event event);

            void subscribe(IEventReceiver& receiver);
            void unsubscribe(IEventReceiver& receiver);

        private:
            long findIndex(IEventReceiver* receiver) const;
    };
}
