/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Linux implementation for
** UDPSocket class
*/

#include "UDPSocket.hpp"
#include <wrecks/errors.hpp>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


Wrecks::Networking::UDPSocket::UDPSocket(cstring_view host, std::uint16_t port)
    : m_Hostname(host)
    , m_Port(port)
    , m_FD(-1)
{
    m_FD = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (m_FD < 0)
        throw NetworkError::fromErrno();

    auto addr = constructAddress(host, port);

    int status = ::bind(m_FD, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (status < 0)
        throw NetworkError::fromErrno();
}

Wrecks::Networking::UDPSocket::UDPSocket(UDPSocket&& other) noexcept
    : m_Hostname()
    , m_Port(0)
    , m_FD(-1)
{
    swap(other);
}

Wrecks::Networking::UDPSocket::~UDPSocket()
{
    ::close(m_FD);
}

void Wrecks::Networking::UDPSocket::swap(UDPSocket& other) noexcept
{
    std::swap(m_Hostname, other.m_Hostname);
    std::swap(m_Port, other.m_Port);
    std::swap(m_FD, other.m_FD);
}

auto Wrecks::Networking::UDPSocket::constructAddress(cstring_view host, std::uint16_t port) -> sockaddr_in
{
    sockaddr_in addr = { 0 };

    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(m_Port);

    if (!::inet_aton(m_Hostname.data(), &addr.sin_addr))
        throw NetworkError("invalid hostname: \"" + std::string(host) + "\"");

    return addr;
}

bool Wrecks::Networking::UDPSocket::sendBytes(const char* data, std::size_t length, const sockaddr_in& addr) noexcept
{
    auto bytesSent = ::sendto(
        m_FD,
        data, length,
        0,
        reinterpret_cast<const sockaddr*>(&addr),
        sizeof(sockaddr_in)
    );

    return bytesSent == length;
}
