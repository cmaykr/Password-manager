#pragma once

#include <string>
#include <iostream>
#include <utility>


struct Entry {
    std::string entryName;
    std::string username;
    std::string password;
};

class VaultInterface 
{
public:
    virtual void addEntry(Entry newEntry) = 0;

    virtual Entry getEntry(Entry entry) = 0;

    virtual void updateEntry(Entry entry) = 0;

    virtual void deleteEntry(Entry entry) = 0;
};