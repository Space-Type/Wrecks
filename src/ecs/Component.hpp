/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** Component concept
*/

#pragma once

#include <type_traits>


namespace Wrecks::ECS {
    template <typename T>
    concept Component =
        std::is_move_constructible_v<T> &&
        std::is_move_assignable_v<T>;
}
