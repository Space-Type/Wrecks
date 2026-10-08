/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** NetworkError class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>


TEST(errors_NetworkError, message)
{
    std::string_view message = "bob";

    EXPECT_THROW(
        try {
            throw Wrecks::NetworkError(message);
        } catch (const Wrecks::NetworkError& error) {
            EXPECT_EQ(message, error.what());
            throw error;
        },
        Wrecks::Error
    );
}

TEST(errors_NetworkError, isErrorBase)
{
    EXPECT_THROW(throw Wrecks::NetworkError("test"), Wrecks::Error);
}
