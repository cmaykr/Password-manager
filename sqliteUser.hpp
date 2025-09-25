#pragma once

#include <string>
#include "Iuser.hpp"
#include <sqlite3.h>

class sqliteUser : public UserInterface
{
public:
    sqliteUser(std::string const& databaseFilename);
    ~sqliteUser();

    void addUser(User const& user) override;

    User getUser(User const& user) override;

    void updateUser(User const& currentUser, User const& newUserInfo) override;

    void deleteUser(User user) override;

private:
    sqlite3 *db;
};