/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** NetworkError class
*/

#pragma once

#include "../Error/Error.hpp"


namespace Wrecks {
    class NetworkError : public Error {
        public:
            NetworkError(std::string_view msg);

            static NetworkError fromErrno();
    };
}
