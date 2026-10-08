/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** Event variant
*/

#pragma once

#include <variant>


namespace Wrecks::Events {
    struct EntityDeleteEvent {};

    using Event = std::variant<EntityDeleteEvent>;
}
