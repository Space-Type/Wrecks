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
    : UDPSocket()
{
    open(host, port);
}

Wrecks::Networking::UDPSocket::UDPSocket(UDPSocket&& other) noexcept
    : UDPSocket()
{
    swap(other);
}

Wrecks::Networking::UDPSocket::UDPSocket() noexcept
    : m_Address{ 0 }
    , m_FD(-1)
{
}

Wrecks::Networking::UDPSocket::~UDPSocket()
{
    close();
}

auto Wrecks::Networking::UDPSocket::host() const noexcept -> cstring_view
{
    constexpr std::size_t hostSize = 16;  // 3x4 (numbers) + 3 (dots) + 1 ('\0')
    static char hostname[hostSize] = { 0 };

    std::fill_n(hostname, hostSize, '\0');
    const char* res = ::inet_ntop(AF_INET, &m_Address.sin_addr, hostname, hostSize - 1);

    return res
        ? hostname
        : "<unknown host>";
}

auto Wrecks::Networking::UDPSocket::port() const noexcept -> std::uint16_t
{
    return ::ntohs(m_Address.sin_port);
}

void Wrecks::Networking::UDPSocket::open(cstring_view host, std::uint16_t port)
{
    m_FD = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (m_FD < 0)
        throw NetworkError::fromErrno();

    m_Address = constructAddress(host, port);

    int status = ::bind(m_FD, reinterpret_cast<sockaddr*>(&m_Address), sizeof(m_Address));
    if (status < 0)
        throw NetworkError::fromErrno();
}

void Wrecks::Networking::UDPSocket::close() noexcept
{
    if (m_FD < 0)
        return;  // not necessary but might help speed up

    ::close(m_FD);

    m_Address = { 0 };
    m_FD = -1;
}

bool Wrecks::Networking::UDPSocket::isOpen() const noexcept
{
    return m_FD >= 0;
}

void Wrecks::Networking::UDPSocket::swap(UDPSocket& other) noexcept
{
    std::swap(m_Address, other.m_Address);
    std::swap(m_FD, other.m_FD);
}

auto Wrecks::Networking::UDPSocket::constructAddress(cstring_view host, std::uint16_t port) -> sockaddr_in
{
    sockaddr_in addr = { 0 };

    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(port);

    if (!::inet_aton(host.data(), &addr.sin_addr))
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

    if (bytesSent < 0)
        close();

    return bytesSent >= 0 && bytesSent == length;
}

size_t Wrecks::Networking::UDPSocket::receiveBytes(char* data, std::size_t size, const sockaddr_in& dest) noexcept
{
    sockaddr_in addr = dest;
    socklen_t addrLen = sizeof(sockaddr_in);

    auto bytesReceived = ::recvfrom(
        m_FD,
        data, size,
        0,
        reinterpret_cast<sockaddr*>(&addr), &addrLen
    );

    if (bytesReceived < 0) {
        close();
        return 0;
    }

    return static_cast<size_t>(bytesReceived);
}
