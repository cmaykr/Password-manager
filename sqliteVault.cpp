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

    std::string sqlCommand {"CREATE TABLE IF NOT EXISTS vault ("
        "entryName varchar(255),"
        "username varchar(255),"
        "password varchar(255)"
        ")"};
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, sqlCommand.c_str(), callback, 0, &zErrMsg);

    std::cout << rc << std::endl;
    if (rc)
    {
        std::cout << "Error!" << zErrMsg << std::endl;
    }
}

sqliteVault::~sqliteVault()
{
    sqlite3_close(db);
    while (sqlite3_close(db) == SQLITE_BUSY);
}
    
void sqliteVault::addEntry(Entry newEntry)
{
    std::string sqlCommand {"INSERT INTO vault (entryName, username, password) VALUES"
        "(?, ?, ?)"
    };

    sqlite3_stmt *statement{};
    int rc = sqlite3_prepare_v2(db, sqlCommand.c_str(), sqlCommand.length(), &statement, nullptr);
    if (rc)
    {
        std::cout << "Error " << rc << "!" << std::endl;
    }
    if (sqlite3_bind_text(statement, 1, newEntry.entryName.c_str(), newEntry.entryName.length(), SQLITE_STATIC))
    {
        std::cout << "Error " << rc << "!" << std::endl;
    }
    if (sqlite3_bind_text(statement, 2, newEntry.username.c_str(), newEntry.username.length(), SQLITE_STATIC))
    {
        std::cout << "Error " << rc << "!" << std::endl;
    }
    if (sqlite3_bind_text(statement, 3, newEntry.password.c_str(), newEntry.password.length(), SQLITE_STATIC))
    {
        std::cout << "Error " << rc << "!" << std::endl;
    }

    sqlite3_step(statement);
    sqlite3_reset(statement);
    sqlite3_finalize(statement);
    char *zErrMsg = 0;
    //sqlite3_exec(db, sqlCommand.c_str(), callback, 0, &zErrMsg);
}

Entry sqliteVault::getEntry(Entry entry)
{
    std::string sqlCommand {"SELECT entryName, username, password FROM vault WHERE entryName = ?"};
    sqlite3_stmt *statement{};
    int rc = sqlite3_prepare_v2(db, sqlCommand.c_str(), sqlCommand.length(), &statement, nullptr);
    sqlite3_bind_text(statement, 1, entry.entryName.c_str(), entry.entryName.length(), SQLITE_STATIC);

    rc = sqlite3_step(statement);
    Entry gotEntry{};
    if (rc == SQLITE_ROW)
    {
        gotEntry.entryName = entry.entryName;
        gotEntry.username = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)); 
        gotEntry.password = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)); // This should not cause any troubles as only printable characters will be used.
    }
    return gotEntry;
}

void sqliteVault::updateEntry(Entry entry)
{
    std::string sqlCommand {"UPDATE vault SET username = ?, password = ? WHERE entryName = ?"};
    sqlite3_stmt *statement{};

    int rc = sqlite3_prepare_v2(db, sqlCommand.c_str(), sqlCommand.length(), &statement, nullptr);
    sqlite3_bind_text(statement, 1, entry.username.c_str(), entry.username.length(), SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, entry.password.c_str(), entry.password.length(), SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, entry.entryName.c_str(), entry.entryName.length(), SQLITE_STATIC);

    rc = sqlite3_step(statement);
    sqlite3_reset(statement);
    sqlite3_finalize(statement);

}

void sqliteVault::deleteEntry(Entry entry)
{
    std::string sqlCommand{"DELETE FROM vault WHERE entryName = ?"};
    sqlite3_stmt *statement{};

    int rc = sqlite3_prepare_v2(db, sqlCommand.c_str(), sqlCommand.length(), &statement, nullptr);
    sqlite3_bind_text(statement, 1, entry.entryName.c_str(), entry.entryName.length(), SQLITE_STATIC);

    rc = sqlite3_step(statement);
    sqlite3_reset(statement);
    sqlite3_finalize(statement);
}