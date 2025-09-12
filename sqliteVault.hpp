#include "vaultInterface.hpp"
#include <sqlite3.h>

class sqliteVault : public VaultInterface
{
public:
    sqliteVault(std::string const& databaseFilename);
    ~sqliteVault();

    void addPassword(std::string const& entryName, std::string const& password, std::string const& username);

    std::pair<std::string, std::string> getPassword(std::string const& entryName);

    void updatePassword(std::string const& entryName, std::string const& password, std::string const& username);

    void deletePassword(std::string const& entryName);

private:
    sqlite3 *db;
};