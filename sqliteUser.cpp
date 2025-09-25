#include "sqliteUser.hpp"
#include <iostream>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

sqliteUser::sqliteUser(std::string const& databaseFilename)
{
    int rc = sqlite3_open(databaseFilename.c_str(), &db);

    std::string sqlCommand {"CREATE TABLE IF NOT EXISTS users ("
        "username varchar(255) PRIMARY KEY NOT NULL,"
        "password varchar(255) NOT NULL,"
        "vaultFileName varchar(255) UNIQUE NOT NULL"
        ")"};
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, sqlCommand.c_str(), callback, 0, &zErrMsg);

    std::cout << rc << std::endl;
    if (rc)
    {
        std::cout << "Error!" << zErrMsg << std::endl;
    }
}

sqliteUser::~sqliteUser()
{
    // TODO: Should wait for all transactions to finish, or wait a specific amount of time
    sqlite3_close(db);
}

void sqliteUser::addUser(User const& user)
{
    std::string sqlCommand {"INSERT INTO users (username, password, vaultFileName) VALUES"
        "(?, ?, ?)"
    };

    sqlite3_stmt *statement{};
    int rc = sqlite3_prepare_v2(db, sqlCommand.c_str(), sqlCommand.length(), &statement, nullptr);
    if (rc)
    {
        std::cout << "Error " << rc << "!" << std::endl;
    }
    if (sqlite3_bind_text(statement, 1, user.username.c_str(), user.username.length(), SQLITE_STATIC))
    {
        std::cout << "Error " << rc << "!" << std::endl;
    }
    if (sqlite3_bind_text(statement, 2, user.password.c_str(), user.password.length(), SQLITE_STATIC))
    {
        std::cout << "Error " << rc << "!" << std::endl;
    }
    if (sqlite3_bind_text(statement, 3, user.vaultFileName.c_str(), user.vaultFileName.length(), SQLITE_STATIC))
    {
        std::cout << "Error " << rc << "!" << std::endl;
    }

    sqlite3_step(statement);
    sqlite3_reset(statement);
    sqlite3_finalize(statement);
    char *zErrMsg = 0;
}

User sqliteUser::getUser(User const& user)
{
    std::string sqlCommand {"SELECT username, password, vaultFileName FROM users WHERE username = ?"};
    sqlite3_stmt *statement{};
    int rc = sqlite3_prepare_v2(db, sqlCommand.c_str(), sqlCommand.length(), &statement, nullptr);
    sqlite3_bind_text(statement, 1, user.username.c_str(), user.username.length(), SQLITE_STATIC);

    rc = sqlite3_step(statement);
    User gotUser{};
    if (rc == SQLITE_ROW)
    {
        gotUser.username = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)); 
        gotUser.password = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)); // This should not cause any troubles as only printable characters will be used.
        gotUser.vaultFileName = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)); // This should not cause any troubles as only printable characters will be used.
    }
    sqlite3_reset(statement);
    sqlite3_finalize(statement);
    return gotUser;
}

void sqliteUser::updateUser(User const& currentUser, User const& newUserInfo)
{
    std::string sqlCommand {"UPDATE vault SET username = ?, password = ?, vaultFileName = ? WHERE username = ?"};
    sqlite3_stmt *statement{};

    int rc = sqlite3_prepare_v2(db, sqlCommand.c_str(), sqlCommand.length(), &statement, nullptr);
    sqlite3_bind_text(statement, 1, newUserInfo.username.c_str(), newUserInfo.username.length(), SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, newUserInfo.password.c_str(), newUserInfo.password.length(), SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, newUserInfo.vaultFileName.c_str(), newUserInfo.vaultFileName.length(), SQLITE_STATIC);
    sqlite3_bind_text(statement, 4, currentUser.username.c_str(), currentUser.username.length(), SQLITE_STATIC);

    rc = sqlite3_step(statement);
    sqlite3_reset(statement);
    sqlite3_finalize(statement);
}

void sqliteUser::deleteUser(User user)
{
    std::string sqlCommand{"DELETE FROM users WHERE username = ?"};
    sqlite3_stmt *statement{};

    int rc = sqlite3_prepare_v2(db, sqlCommand.c_str(), sqlCommand.length(), &statement, nullptr);
    sqlite3_bind_text(statement, 1, user.username.c_str(), user.username.length(), SQLITE_STATIC);

    rc = sqlite3_step(statement);
    sqlite3_reset(statement);
    sqlite3_finalize(statement);
}