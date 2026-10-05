/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** the unix version of
** UDPSocket class
*/

#include "wrecks/common.hpp"
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <wrecks.hpp>
#include <arpa/inet.h>

constexpr Wrecks::cstring_view SOCKET_HOST = "0.0.0.0";
constexpr std::uint16_t        SOCKET_PORT = 51487;

struct TestData {
    int a;
    double b;
    char c;
    float d;

    constexpr bool operator==(const TestData& other) const
    {
        return
            (a == other.a) &&
            (b == other.b) &&
            (c == other.c) &&
            (d == other.d);
    }
};

void setRecvTimeout(int fd) {
    timeval timeout{};
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    ASSERT_EQ(::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)), 0);
}

TEST(networking_UDPSocket, SendStructData)
{
    Wrecks::Networking::UDPSocket socket(SOCKET_HOST, SOCKET_PORT);

    TestData sent = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };
    TestData received;

    int receiverFD = ::socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in receiverAddr = { 0 };
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = ::htons(12345);
    ::inet_aton("127.0.0.1", &receiverAddr.sin_addr);

    (void)!!::bind(receiverFD, reinterpret_cast<sockaddr*>(&receiverAddr), sizeof(sockaddr_in));
    setRecvTimeout(receiverFD);

    ASSERT_TRUE(socket.send(sent, receiverAddr));

    socklen_t addrLen = sizeof(sockaddr_in);
    sockaddr_in addr = socket.address();

    ssize_t bytesRead = ::recvfrom(receiverFD, &received, sizeof(received), 0, reinterpret_cast<sockaddr*>(&addr), &addrLen);

    EXPECT_EQ(bytesRead, sizeof(TestData));
    EXPECT_EQ(sent, received);
}

TEST(networking_UDPSocket, SendStructDataSimple)
{
    Wrecks::Networking::UDPSocket socket(SOCKET_HOST, SOCKET_PORT);

    TestData sent = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };
    TestData received;

    int receiverFD = ::socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in receiverAddr = { 0 };
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = ::htons(12344);
    ::inet_aton("127.0.0.1", &receiverAddr.sin_addr);

    (void)!!::bind(receiverFD, reinterpret_cast<sockaddr*>(&receiverAddr), sizeof(sockaddr_in));
    setRecvTimeout(receiverFD);

    ASSERT_TRUE(socket.send(sent, "127.0.0.1", 12344));

    socklen_t addrLen = sizeof(sockaddr_in);
    sockaddr_in addr = socket.address();

    ssize_t bytesRead = ::recvfrom(receiverFD, &received, sizeof(received), 0, reinterpret_cast<sockaddr*>(&addr), &addrLen);

    EXPECT_EQ(bytesRead, sizeof(TestData));
    EXPECT_EQ(sent, received);
}

TEST(networking_UDPSocket, SendError)
{
    Wrecks::Networking::UDPSocket socket(SOCKET_HOST, SOCKET_PORT);
    TestData data = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };

    socket.close();
    EXPECT_FALSE(socket.send(data, "127.0.0.1", 23451));
}
