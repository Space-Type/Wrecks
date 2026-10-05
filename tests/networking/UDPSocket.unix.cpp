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

static void EXPECT_SOCKADDR_EQ(const sockaddr_in& a, const sockaddr_in& b)
{
    EXPECT_TRUE(::memcmp(&a.sin_addr, &b.sin_addr, sizeof(a.sin_addr)) == 0);
    EXPECT_EQ(a.sin_port, b.sin_port);
}

static void setRecvTimeout(int fd)
{
    timeval timeout{};
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    ASSERT_EQ(::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)), 0);
}

static void testingSocket(Wrecks::cstring_view host, std::uint16_t port, int& fd, sockaddr_in& addr)
{
    fd = 0;
    addr = { 0 };

    fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT_GE(fd, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(port);
    ASSERT_TRUE(::inet_aton(host.data(), &addr.sin_addr));

    int bindStatus = ::bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
    ASSERT_EQ(bindStatus, 0);

    setRecvTimeout(fd);
}

TEST(networking_UDPSocket, SendStructData)
{
    Wrecks::Networking::UDPSocket socket(SOCKET_HOST, SOCKET_PORT);

    TestData sent = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };
    TestData received;

    int receiverFD;
    sockaddr_in receiverAddr;
    testingSocket("127.0.0.1", 12345, receiverFD, receiverAddr);

    ASSERT_TRUE(socket.send(sent, receiverAddr));

    socklen_t addrLen = sizeof(sockaddr_in);
    sockaddr_in addr = socket.address();

    ssize_t bytesRead = ::recvfrom(receiverFD, &received, sizeof(received), 0, reinterpret_cast<sockaddr*>(&addr), &addrLen);

    EXPECT_EQ(bytesRead, sizeof(TestData));
    EXPECT_EQ(sent, received);

    ::close(receiverFD);
}

TEST(networking_UDPSocket, SendStructDataSimple)
{
    Wrecks::Networking::UDPSocket socket(SOCKET_HOST, SOCKET_PORT);

    TestData sent = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };
    TestData received;

    int receiverFD;
    sockaddr_in receiverAddr;
    testingSocket("127.0.0.1", 12344, receiverFD, receiverAddr);

    ASSERT_TRUE(socket.send(sent, "127.0.0.1", 12344));

    socklen_t addrLen = sizeof(sockaddr_in);
    sockaddr_in addr = socket.address();

    ssize_t bytesRead = ::recvfrom(receiverFD, &received, sizeof(received), 0, reinterpret_cast<sockaddr*>(&addr), &addrLen);

    EXPECT_EQ(bytesRead, sizeof(TestData));
    EXPECT_EQ(sent, received);

    ::close(receiverFD);
}

TEST(networking_UDPSocket, SendError)
{
    Wrecks::Networking::UDPSocket socket(SOCKET_HOST, SOCKET_PORT);
    TestData data = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };

    socket.close();
    EXPECT_FALSE(socket.send(data, "127.0.0.1", 23451));
}

TEST(networking_UDPSocket, ReceiveStructData)
{
    Wrecks::Networking::UDPSocket socket(SOCKET_HOST, 12343);
    socket.setNonBlocking();

    TestData sent = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };
    TestData received;

    int senderFD;
    sockaddr_in senderAddr;
    testingSocket("127.0.0.1", 12342, senderFD, senderAddr);

    auto bytesSent = ::sendto(senderFD, &sent, sizeof(TestData), 0, reinterpret_cast<const sockaddr*>(&socket.address()), sizeof(sockaddr_in));
    ASSERT_EQ(bytesSent, sizeof(TestData));

    sockaddr_in receivedSender;
    auto bytesReceived = socket.receive(received, &receivedSender);

    EXPECT_EQ(bytesReceived, sizeof(TestData));
    EXPECT_SOCKADDR_EQ(receivedSender, senderAddr);
    EXPECT_EQ(sent, received);

    ::close(senderFD);
}

TEST(networking_UDPSocket, ReceiveStructDataNoSender)
{
    Wrecks::Networking::UDPSocket socket(SOCKET_HOST, 12343);
    socket.setNonBlocking();

    TestData sent = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };
    TestData received;

    int senderFD;
    sockaddr_in senderAddr;
    testingSocket("127.0.0.1", 12342, senderFD, senderAddr);

    auto bytesSent = ::sendto(senderFD, &sent, sizeof(TestData), 0, reinterpret_cast<const sockaddr*>(&socket.address()), sizeof(sockaddr_in));
    ASSERT_EQ(bytesSent, sizeof(TestData));

    auto bytesReceived = socket.receive(received);

    EXPECT_EQ(bytesReceived, sizeof(TestData));
    EXPECT_EQ(sent, received);

    ::close(senderFD);
}

TEST(networking_UDPSocket, ReceiveError)
{
    Wrecks::Networking::UDPSocket socket(SOCKET_HOST, 12341);
    socket.setNonBlocking();

    TestData data;
    auto bytesReceived = socket.receive(data);  // random unused port

    EXPECT_EQ(bytesReceived, 0);
}
