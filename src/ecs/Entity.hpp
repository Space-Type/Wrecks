/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** Entity class
*/

#pragma once

#include <type_traits>
#include <wrecks/common.hpp>


namespace Wrecks::ECS {
    struct Entity {
        using ID = std::uint32_t;

        ID id;
    };
}
