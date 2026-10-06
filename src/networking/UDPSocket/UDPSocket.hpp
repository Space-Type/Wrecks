/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** UDPSocket class
*/

#pragma once

#include <wrecks/common.hpp>
#include <string_view>
#include <cstdint>

// include sockaddr_in (since it's common on all platforms)
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netinet/in.h>
#endif

namespace Wrecks::Networking {
    class UDPSocket {
        sockaddr_in m_Address;

        #ifdef _WIN32
            SOCKET m_Socket;
        #else
            int m_FD;
        #endif

        public:
            UDPSocket(cstring_view host, std::uint16_t port);
            UDPSocket(const UDPSocket& cpy) = delete;
            UDPSocket(UDPSocket&& other) noexcept;
            UDPSocket() noexcept;
            ~UDPSocket();


            [[nodiscard]] cstring_view host() const noexcept;
            [[nodiscard]] std::uint16_t port() const noexcept;
            [[nodiscard]] inline const sockaddr_in& address() const noexcept { return m_Address; }

            void open(cstring_view host, std::uint16_t port);
            void close() noexcept;
            [[nodiscard]] bool isOpen() const noexcept;

            void setNonBlocking(bool setting = true);

            /**
             * Sends data to the destination
             * address. Returns true if all data
             * was sent, and false if not.
             */
            template <typename T>
            bool send(const T& data, const sockaddr_in& dest) noexcept { return sendBytes(reinterpret_cast<const char*>(&data), sizeof(T), dest); }

            /**
             * Receives data available on the socket.
             * Returns the amount of bytes
             * that were received, or 0 otherwise.
             * If the sender is a non-null pointer
             * and the return value isn't 0, its
             * value will be filled with the address
             * of the packet's sender.
             */
            template <typename T>
            size_t receive(T& data, sockaddr_in* sender = nullptr) noexcept { return receiveBytes(reinterpret_cast<char*>(&data), sizeof(T), sender); }

            /**
             * Simpler version of `send()`, which is
             * easier to use, but incurs a performance
             * cost. Should not be used in performance-
             * sensitive contexts.
             */
            template <typename T>
            bool send(const T& data, cstring_view host, std::uint16_t port)
            {
                auto addr = constructAddress(host, port);
                return send(data, addr);
            }

            void swap(UDPSocket& other) noexcept;
            inline void operator=(UDPSocket&& other) noexcept { swap(other); }

        private:
            static sockaddr_in constructAddress(cstring_view host, std::uint16_t port);
            bool sendBytes(const char* data, std::size_t length, const sockaddr_in& dest) noexcept;
            size_t receiveBytes(char* data, std::size_t size, sockaddr_in* dest) noexcept;
    };
}

template <>
inline void std::swap(Wrecks::Networking::UDPSocket& a, Wrecks::Networking::UDPSocket& b) noexcept
{
    a.swap(b);
}
