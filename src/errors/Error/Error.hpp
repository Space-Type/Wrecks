/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** Error class
*/

#pragma once

#include <string_view>
#include <string>


namespace Wrecks {
    class Error : public std::exception {
        std::string m_Message;

        public:
            Error(std::string_view msg);

            inline const char* what() const noexcept override { return m_Message.c_str(); }
    };
}
