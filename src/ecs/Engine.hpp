/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** Engine class
*/

#pragma once

#include "Entity.hpp"
#include "System.hpp"
#include <vector>


namespace Wrecks::ECS {
    template <SystemTuple Systems>
    class Engine {
        Systems m_Systems;
        std::vector<Entity> m_Entities;

        public:
            Engine() noexcept
                : m_Systems()
                , m_Entities()
            {}

            template <System T>
            constexpr const T& system() const noexcept { return std::get<T>(m_Systems); }

            void update()
            {
                std::apply([&](auto& system){
                    Signature signature = system.signature();
                    system();
                }, m_Systems);
            }
    };
}
