#include "vaultInterface.hpp"
#include <map>
#include <iostream>
#include <utility>

class MockVaultInterface : VaultInterface
{
public:
    MockVaultInterface()
        : vault{}
    {}

    void addPassword(std::string const& entryName, std::string const& password, std::string const& username) override
    {
        vault[entryName] = std::make_pair(username, password);
    }

    std::pair<std::string, std::string> getPassword(std::string const& entryName) override
    {
        return vault[entryName];
    }

    void updatePassword(std::string const& entryName, std::string const& password, std::string const& username) override
    {
        vault[entryName] = std::make_pair(username, password);
    }

    void deletePassword(std::string const& entryName) override
    {
        vault.erase(entryName);
    }

private:
    std::map<std::string, std::pair<std::string, std::string>> vault;
};