#include "Ivault.hpp"
#include <map>
#include <iostream>
#include <utility>

class MockVaultInterface : VaultInterface
{
public:
    MockVaultInterface()
        : vault{}
    {}

    void addEntry(Entry newEntry) override
    {
        vault[newEntry.entryName] = newEntry;
    }

    Entry getEntry(Entry entry) override
    {
        return vault[entry.entryName];
    }

    void updateEntry(Entry entry) override
    {
        vault[entry.entryName] = entry;
    }

    void deleteEntry(Entry entry) override
    {
        vault.erase(entry.entryName);
    }

private:
    std::map<std::string, Entry> vault;
};