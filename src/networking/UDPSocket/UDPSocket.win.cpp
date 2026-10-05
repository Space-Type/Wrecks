/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Windows implementation for
** UDPSocket class
*/

#include "UDPSocket.hpp"
#include <wrecks/errors.hpp>
#include <winsock2.h>
#include <ws2tcpip.h>


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
    , m_Socket(INVALID_SOCKET)
{
}

Wrecks::Networking::UDPSocket::~UDPSocket()
{
    close();
}

auto Wrecks::Networking::UDPSocket::host() const noexcept -> cstring_view
{
    constexpr std::size_t hostSize = 16;
    static char hostname[hostSize];

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
    m_Socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (m_Socket == INVALID_SOCKET)
        throw NetworkError("failed to open socket");  // TODO: replace with ::fromErrno

    m_Address = constructAddress(host, port);

    int status = ::bind(m_Socket, reinterpret_cast<const sockaddr*>(&m_Address), sizeof(m_Address));
    if (status != 0)
        throw NetworkError("failed to bind");  // TODO: replace with ::fromErrno
}

void Wrecks::Networking::UDPSocket::close() noexcept
{
    if (m_Socket == INVALID_SOCKET)
        return;

    ::closesocket(m_Socket);

    m_Address = {};
    m_Socket = INVALID_SOCKET;
}

bool Wrecks::Networking::UDPSocket::isOpen() const noexcept
{
    return m_Socket != INVALID_SOCKET;
}

void Wrecks::Networking::UDPSocket::setNonBlocking(bool setting)
{
    unsigned long enabled = setting;
    int success = ::ioctlsocket(m_Socket, FIONBIO, &enabled);

    if (success != 0)
        throw NetworkError("failed to change socket blocking mode");  // TODO: set ::fromErrno
}

void Wrecks::Networking::UDPSocket::swap(UDPSocket& other) noexcept
{
    std::swap(m_Address, other.m_Address);
    std::swap(m_Socket, other.m_Socket);
}

auto Wrecks::Networking::UDPSocket::constructAddress(cstring_view host, std::uint16_t port) -> sockaddr_in
{
    sockaddr_in addr = {};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (!::inet_pton(AF_INET, host.data(), &addr.sin_addr))
        throw NetworkError("invalid hostname");  // TODO: replace with ::fromErrno

    return addr;
}

bool Wrecks::Networking::UDPSocket::sendBytes(const char* data, std::size_t length, const sockaddr_in& dest) noexcept
{
    int bytesSent = ::sendto(m_Socket, data, static_cast<int>(length), 0, reinterpret_cast<const sockaddr*>(&dest), sizeof(sockaddr_in));

    if (bytesSent == SOCKET_ERROR)
        close();

    return bytesSent > 0 && static_cast<std::size_t>(bytesSent) == length;
}

size_t Wrecks::Networking::UDPSocket::receiveBytes(char* data, std::size_t size, sockaddr_in* dest) noexcept
{
    socklen_t addrLen = sizeof(sockaddr_in);
    int bytesReceived = ::recvfrom(m_Socket, data, size, 0, reinterpret_cast<sockaddr*>(dest), dest ? &addrLen : nullptr);

    if (bytesReceived == SOCKET_ERROR)
        close();

    return static_cast<std::size_t>(bytesReceived < 0 ? 0 : bytesReceived);
}
