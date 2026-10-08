/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** BaseEventReceiver class
*/

#pragma once

#include "IEventReceiver.hpp"

namespace Wrecks::Events {
    class BaseEventReceiver : public IEventReceiver {
        public:
            constexpr BaseEventReceiver() noexcept {}

            constexpr void handleEvent(const EntityDeleteEvent& evt) noexcept override {}
    };
}
