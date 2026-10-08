/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** IEventReceiver interface
*/

#pragma once

#include "Event.hpp"


namespace Wrecks::Events {
    /**
     * Event Receiver interface.
     *
     *
     */
    class IEventReceiver {
        public:
            virtual ~IEventReceiver() = default;

            virtual void handleEvent(const EntityDeleteEvent& evt) noexcept = 0;
    };
}
