#include "sqliteVault.hpp"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

sqliteVault::sqliteVault(std::string const& databaseFilename)
{
    int rc = sqlite3_open(databaseFilename.c_str(), &db);

    std::string sqlCommand {"CREATE TABLE vault ("
        "entryName varchar(255),"
        "username varchar(255),"
        "password varchar(255)"
        ")"};
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, sqlCommand.c_str(), callback, 0, &zErrMsg);

    std::cout << rc << std::endl;
    if (rc)
    {
        std::cout << "Error!" << std::endl;
    }
}

sqliteVault::~sqliteVault()
{
    while (sqlite3_close(db) == SQLITE_BUSY);
}
    
void sqliteVault::addPassword(std::string const& entryName, std::string const& password, std::string const& username)
{
    std::string sqlCommand {""};
    char *zErrMsg = 0;
    sqlite3_exec(db, sqlCommand.c_str(), callback, 0, &zErrMsg);
}

std::pair<std::string, std::string> sqliteVault::getPassword(std::string const& entryName)
{

}

void sqliteVault::updatePassword(std::string const& entryName, std::string const& password, std::string const& username)
{

}

void sqliteVault::deletePassword(std::string const& entryName)
{

}