#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

#include "mockUserInterface.hpp"
#include "mockVaultInterface.hpp"
#include "sqliteVault.hpp"

MockUserInterface userInterface;
sqliteVault vault{"test.db"};

void userMenu(User loggedInUser);

std::string hiddenCin()
{
    std::string input{};
    termios st{};
    termios newst{};
    tcgetattr(STDIN_FILENO, &st);
    
    newst = st;
    newst.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newst);
    std::cin >> input;

    tcsetattr(STDIN_FILENO, TCSANOW, &st);

    return input;
}

void createUser()
{
    std::string username{};
    std::string password{};

    std::cout << "Enter login details for the user below." << std::endl;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    password = hiddenCin();

    User user {username, password};

    userInterface.addUser(user);
}

int main(int argc, char** argv)
{
    userInterface = MockUserInterface();
    std::string input{};
    bool exit {false};

    while (exit == false)
    {
        std::cout << "Select action: \n"
        "1. Login as user\n"
        "2. Create new user and vault\n"
        "3. Exit program\n";

        std::cout << "Choice: ";
        std::cin >> input;

        if (input.size() == 1 && std::isdigit(input.at(0)))
        {
            switch (std::atoi(input.c_str()))
            {
                case 1:
                {
                    bool loggedIn {false};
                    int tries{};
                    std::string username{};
                    std::string password{};
                    User loggedInUser{};

                    do
                    {
                        std::cout << "Username: ";
                        std::cin >> username;
                        std::cout << "Password: ";
                        password = hiddenCin();
                        std::cout << std::endl;

                        loggedInUser = userInterface.getUser(username);
                        if (loggedInUser.username == username && loggedInUser.password == password)
                        {
                            userMenu(loggedInUser);
                            break;
                        }
                        else
                        {
                            std::cout << "Wrong password. Try again." << std::endl;
                            tries++;
                        }
                    } while (tries < 3);

                    if (tries == 3)
                    {
                        std::cout << "3 incorrect login attempts" << std::endl;
                    }
                    break;
                }
                case 2:
                    createUser();
                    break;
                case 3:
                default:
                    exit = true;
                    break;
            }
        }
    }

    return 0;
}

void userMenu(User loginedUser)
{
    std::string input{};
    bool loggedIn = true;
    while (loggedIn)
    {
        std::cout << "Select action: \n"
        "1. Get a entry\n"
        "2. Create new entry\n"
        "3. Update existing entry\n"
        "4. Delete a entry\n"
        "5. Log out\n";

        std::cin >> input;

        if (input.size() == 1 && std::isdigit(input.at(0)))
        {
            switch (std::atoi(input.c_str()))
            {
                case 1:
                {
                    std::cout << "Name of entry: ";
                    std::cin >> input;
                    Entry entry = vault.getEntry(Entry{input});

                    std::cout << "Username: " << entry.username << std::endl;
                    std::cout << "Password: " << entry.password << std::endl;
                    break;
                }
                case 2:
                {
                    std::string entryName{};
                    std::string username{};
                    std::string password{};

                    std::cout << "Name of entry: ";
                    std::cin >> entryName;
                    std::cout << "Username in entry: ";
                    std::cin >> username;
                    std::cout << "Password in entry: ";
                    password = hiddenCin();

                    Entry newEntry{entryName, username, password};
                    vault.addEntry(newEntry);
                    std::cout << std::endl;
                    break;
                }
                case 3:
                {
                    std::string entryName{};
                    std::string username{};
                    std::string password{};

                    std::cout << "Name of entry to update: ";
                    std::cin >> entryName;
                    Entry updateEntry{vault.getEntry(Entry{entryName, "", ""})};
                    if (!updateEntry.entryName.empty())
                    {
                        std::cout << "New username: ";
                        std::cin >> username;
                        std::cout << "New password: ";
                        password = hiddenCin();
                        
                        updateEntry.username = username;
                        updateEntry.password = password;
                        vault.updateEntry(updateEntry);
                    }
                    else
                    {
                        std::cout << "Entry does not exist." << std::endl;
                    }
                    break;
                }
                case 4:
                {
                    std::cout << "Name of entry to delete: ";
                    std::cin >> input;
                    // std::pair<std::string, std::string> entry = vault.getEntry(input);
                    vault.deleteEntry(Entry{input});
                    break;
                }
                case 5:
                default:
                    loggedIn = false;
                    break;
            }
        }

    }
}