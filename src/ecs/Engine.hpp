/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** Engine class
*/

#pragma once

#include "System.hpp"


namespace Wrecks::ECS {
    template <SystemTuple SystemsTuple>
    class Engine {
        SystemsTuple m_Systems;

        public:
            Engine() noexcept {}

            template <System T>
            constexpr const T& system() const noexcept { return std::get<T>(m_Systems); }

            void update()
            {
                std::apply([](auto& system){
                    system();
                }, m_Systems);
            }
    };
}
