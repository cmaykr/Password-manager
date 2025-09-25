#pragma once

#include <string>


struct User {
    std::string username;
    std::string password;
    std::string vaultFileName;
};

class UserInterface 
{
public:
    virtual void addUser(User const& user) = 0;

    virtual User getUser(User const& user) = 0;

    virtual void updateUser(User const& currentUser, User const& newUserInfo) = 0;

    virtual void deleteUser(User user) = 0;
};