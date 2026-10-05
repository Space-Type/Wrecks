/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** UDPSocket class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>

constexpr Wrecks::cstring_view VALID_HOST = "0.0.0.0";
constexpr std::uint16_t VALID_PORT = 51869;

static void EXPECT_SOCKADDR_EQ(const sockaddr_in& a, const sockaddr_in& b)
{
    EXPECT_TRUE(::memcmp(&a.sin_addr, &b.sin_addr, sizeof(a.sin_addr)) == 0);
    EXPECT_EQ(a.sin_port, b.sin_port);
}

TEST(networking_UDPSocket_common, ValidBind)
{
    Wrecks::Networking::UDPSocket socket;

    ASSERT_NO_THROW(socket.open(VALID_HOST, VALID_PORT));
    EXPECT_TRUE(socket.isOpen());
    EXPECT_EQ(socket.host(), VALID_HOST);
    EXPECT_EQ(socket.port(), VALID_PORT);
}

TEST(networking_UDPSocket_common, CloseValidSocket)
{
    Wrecks::Networking::UDPSocket socket;

    ASSERT_NO_THROW(socket.open(VALID_HOST, VALID_PORT));
    socket.close();
    EXPECT_FALSE(socket.isOpen());
}

TEST(networking_UDPSocket_common, InvalidPort)
{
    // 0 is a valid port (=> uses first available port)
    EXPECT_THROW(Wrecks::Networking::UDPSocket(VALID_HOST, 1), Wrecks::NetworkError);
}

TEST(networking_UDPSocket_common, InvalidHost)
{
    EXPECT_THROW(Wrecks::Networking::UDPSocket("pipicaca", VALID_PORT), Wrecks::NetworkError);
}

TEST(networking_UDPSocket_common, SocketMove)
{
    sockaddr_in empty = { 0 };
    Wrecks::Networking::UDPSocket mainSocket(VALID_HOST, VALID_PORT);
    sockaddr_in addr = mainSocket.address();
    Wrecks::Networking::UDPSocket newSocket(std::move(mainSocket));

    EXPECT_SOCKADDR_EQ(mainSocket.address(), empty);
    EXPECT_SOCKADDR_EQ(newSocket.address(), addr);

    std::swap(mainSocket, newSocket);

    EXPECT_SOCKADDR_EQ(mainSocket.address(), addr);
    EXPECT_SOCKADDR_EQ(newSocket.address(), empty);
}
