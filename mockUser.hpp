#pragma once

#include "Iuser.hpp"
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

    User getUser(User const& user) override
    {
        User gottenUser = mockdb[user.username];

        return gottenUser;
    }

    void updateUser(User const& currentUser, User const& newUserInfo) override
    {
        User user = mockdb[currentUser.username];

        user.password = newUserInfo.password;

        mockdb[currentUser.username] = user;
    }

    void deleteUser(User user) override
    {
        mockdb.erase(user.username);
    }

private:
    std::map<std::string, User> mockdb;
};