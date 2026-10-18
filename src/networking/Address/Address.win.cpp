/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Windows implementation
** for Address struct
*/

#include "Address.hpp"
#include <wrecks/errors.hpp>
#include <ws2tcpip.h>


auto Wrecks::Networking::Address::construct(cstring_view host, std::uint16_t port) -> Address
{
    Address addr = {};

    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(port);

    if (!::inet_aton(host.data(), &addr.sin_addr))
        throw NetworkError("invalid hostname: \"" + std::string(host) + "\"");

    return addr;
}

auto Wrecks::Networking::Address::host() const noexcept -> cstring_view
{
    constexpr std::size_t hostSize = 16;  // 3x4 (numbers) + 3 (dots) + 1 ('\0')
    static char hostname[hostSize];

    std::fill_n(hostname, hostSize, '\0');
    const char* res = ::inet_ntop(AF_INET, &sin_addr, hostname, hostSize - 1);

    return res
        ? hostname
        : "<unknown host>";
}

auto Wrecks::Networking::Address::port() const noexcept -> std::uint16_t
{
    return ::ntohs(sin_port);
}
