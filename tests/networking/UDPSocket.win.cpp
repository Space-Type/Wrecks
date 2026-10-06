/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** the unix version of
** UDPSocket class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>
#include <winsock2.h>
#include <ws2tcpip.h>

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

static void testingSocket(Wrecks::cstring_view host, std::uint16_t port, SOCKET& socket, sockaddr_in& addr)
{
    socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT_NE(socket, INVALID_SOCKET);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    ASSERT_TRUE(::inet_pton(AF_INET, host.data(), &addr.sin_addr));

    int status = ::bind(socket, reinterpret_cast<const sockaddr*>(&addr), sizeof(sockaddr_in));
    ASSERT_EQ(status, 0);
}

TEST(networking_UDPSocket, SendStructData)
{
    Wrecks::Networking::UDPSocket socket("0.0.0.0", 12345);

    TestData sent = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };
    TestData received;

    SOCKET receiver;
    sockaddr_in receiverAddr;
    testingSocket("127.0.0.1", 12344, receiver, receiverAddr);

    ASSERT_TRUE(socket.send(sent, receiverAddr));

    socklen_t addrLen = sizeof(sockaddr_in);
    sockaddr_in addr = socket.address();

    int bytesRead = ::recvfrom(receiver, reinterpret_cast<char*>(&received), sizeof(received), 0, reinterpret_cast<sockaddr*>(&addr), &addrLen);

    EXPECT_EQ(bytesRead, sizeof(TestData));
    EXPECT_EQ(sent, received);

    ::closesocket(receiver);
}

TEST(networking_UDPSocket, SendStructDataSimple)
{
    Wrecks::Networking::UDPSocket socket("0.0.0.0", 12343);

    TestData sent = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };
    TestData received;

    SOCKET receiver;
    sockaddr_in receiverAddr;
    testingSocket("127.0.0.1", 12342, receiver, receiverAddr);

    ASSERT_TRUE(socket.send(sent, "127.0.0.1", 12342));

    socklen_t addrLen = sizeof(sockaddr_in);
    sockaddr_in addr = socket.address();

    int bytesRead = ::recvfrom(receiver, reinterpret_cast<char*>(&received), sizeof(received), 0, reinterpret_cast<sockaddr*>(&addr), &addrLen);

    EXPECT_EQ(bytesRead, sizeof(TestData));
    EXPECT_EQ(sent, received);

    ::closesocket(receiver);
}

TEST(networking_UDPSocket, SendError)
{
    Wrecks::Networking::UDPSocket socket("0.0.0.0", 12341);
    TestData data = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };

    socket.close();
    EXPECT_FALSE(socket.send(data, "127.0.0.1", 9999));
}

TEST(networking_UDPSocket, ReceiveStructData)
{
    Wrecks::Networking::UDPSocket socket("127.0.0.1", 12340);
    socket.setNonBlocking();

    TestData sent = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };
    TestData received;

    SOCKET sender;
    sockaddr_in senderAddr;
    testingSocket("127.0.0.1", 12339, sender, senderAddr);

    auto bytesSent = ::sendto(sender, reinterpret_cast<const char*>(&sent), sizeof(TestData), 0, reinterpret_cast<const sockaddr*>(&socket.address()), sizeof(sockaddr_in));
    ASSERT_EQ(bytesSent, sizeof(TestData));

    sockaddr_in receivedSender;
    auto bytesReceived = socket.receive(received, &receivedSender);

    EXPECT_EQ(bytesReceived, sizeof(TestData));
    EXPECT_SOCKADDR_EQ(receivedSender, senderAddr);
    EXPECT_EQ(sent, received);

    ::closesocket(sender);
}

TEST(networking_UDPSocket, ReceiveStructDataNoSender)
{
    Wrecks::Networking::UDPSocket socket("127.0.0.1", 12338);
    socket.setNonBlocking();

    TestData sent = { .a = -87, .b = 7872.32528, .c = 'a', .d = 52.231f };
    TestData received;

    SOCKET sender;
    sockaddr_in senderAddr;
    testingSocket("127.0.0.1", 12337, sender, senderAddr);

    auto bytesSent = ::sendto(sender, reinterpret_cast<const char*>(&sent), sizeof(TestData), 0, reinterpret_cast<const sockaddr*>(&socket.address()), sizeof(sockaddr_in));
    ASSERT_EQ(bytesSent, sizeof(TestData));

    auto bytesReceived = socket.receive(received);

    EXPECT_EQ(bytesReceived, sizeof(TestData));
    EXPECT_EQ(sent, received);

    ::closesocket(sender);
}

TEST(networking_UDPSocket, ReceiveError)
{
    Wrecks::Networking::UDPSocket socket("0.0.0.0", 12336);
    socket.setNonBlocking();

    TestData data;
    auto bytesReceived = socket.receive(data);  // random unused port

    EXPECT_EQ(bytesReceived, 0);
}
