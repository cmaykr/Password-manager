#pragma once

#include <string>


struct User {
    std::string username;
    std::string password;
};

class UserInterface 
{
public:
    virtual void addUser(User const& user) = 0;

    virtual User getUser(std::string const& username) = 0;

    virtual void updateUser(std::string currentUsername, User newUserInfo) = 0;

    virtual void deleteUser(User user) = 0;
};