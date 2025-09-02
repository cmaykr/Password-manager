#include <string>

class VaultInterface 
{
public:
    virtual void addPassword(std::string const& entryName, std::string const& password, std::string const& username) = 0;

    virtual std::string getPassword(std::string const& entryName) = 0;

    virtual void updatePassword(std::string const& entryName, std::string const& password, std::string const& username) = 0;

    virtual void deletePassword(std::string const& entryName) = 0;
};