#include "Ivault.hpp"
#include <sqlite3.h>

class sqliteVault : public VaultInterface
{
public:
    sqliteVault(std::string const& databaseFilename);
    ~sqliteVault();

    void addEntry(Entry newEntry) override;

    Entry getEntry(Entry entry) override;

    void updateEntry(Entry entry) override;

    void deleteEntry(Entry entry) override;

private:
    sqlite3 *db;
};