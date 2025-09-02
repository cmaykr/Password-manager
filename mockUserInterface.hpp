#pragma once

#include "userInterface.hpp"
#include <map>

class MockUserInterface : UserInterface
{
public:
    MockUserInterface()
        : mockdb()
    {}

    void addUser(User const& user) override
    {
        mockdb[user.username] = user;
    }

    User getUser(std::string const& username) override
    {
        User user = mockdb[username];

        return user;
    }

    void updateUser(std::string currentUsername, User newUserInfo) override
    {
        User user = mockdb[currentUsername];

        user.password = newUserInfo.password;

        mockdb[currentUsername] = user;
    }

    void deleteUser(User user) override
    {
        mockdb.erase(user.username);
    }

private:
    std::map<std::string, User> mockdb;
};