/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** Address struct
*/

#pragma once

#include <wrecks/common.hpp>
#include <ostream>

// include sockaddr_in
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netinet/in.h>
#endif

namespace Wrecks::Networking {
    struct Address : sockaddr_in {
        static Address construct(cstring_view host, std::uint16_t port);

        [[nodiscard]] cstring_view host() const noexcept;
        [[nodiscard]] std::uint16_t port() const noexcept;
    };
}

std::ostream& operator<<(std::ostream& output, const Wrecks::Networking::Address& addr);
