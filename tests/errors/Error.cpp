/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Unit tests for
** Error class
*/

#include <gtest/gtest.h>
#include <wrecks.hpp>


TEST(errors_Error, stringViewMessage)
{
    std::string_view message = "bob";

    EXPECT_THROW(
        try {
            throw Wrecks::Error(message);
        } catch (const Wrecks::Error& error) {
            EXPECT_EQ(message, error.what());
            throw error;
        },
        Wrecks::Error
    );
}
