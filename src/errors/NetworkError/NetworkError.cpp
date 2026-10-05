/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Implementation for
** NetworkError class
*/

#include "NetworkError.hpp"

#ifndef _WIN32
    #include <cstring>
    #include <cerrno>
#endif

Wrecks::NetworkError::NetworkError(std::string_view msg)
    : Error(msg)
{

}

auto Wrecks::NetworkError::fromErrno() -> NetworkError {
    #ifdef _WIN32
        // FIXME: implement for windows (if possible)
        return NetworkError("network error");
    #else
        return NetworkError(::strerror(errno));
    #endif
}
