#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

#include "mockUserInterface.hpp"
#include "mockVaultInterface.hpp"
#include "sqliteVault.hpp"

MockUserInterface userInterface;
MockVaultInterface vault;

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

    sqliteVault sqlite{"test.db"};

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
        "1. Get a password\n"
        "2. Create new password\n"
        "3. Delete a password\n"
        "4. Log out\n";

        std::cin >> input;

        if (input.size() == 1 && std::isdigit(input.at(0)))
        {
            switch (std::atoi(input.c_str()))
            {
                case 1:
                {
                    std::cout << "Name of entry: ";
                    std::cin >> input;
                    std::pair<std::string, std::string> entry = vault.getPassword(input);

                    std::cout << "Username: " << entry.first << std::endl;
                    std::cout << "Password: " << entry.second << std::endl;
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

                    vault.addPassword(entryName, password, username);
                    std::cout << std::endl;
                    break;
                }
                case 3:
                {
                    std::cout << "Name of entry to delete: ";
                    std::cin >> input;
                    // std::pair<std::string, std::string> entry = vault.getPassword(input);
                    vault.deletePassword(input);
                    break;
                }
                case 4:
                default:
                    loggedIn = false;
                    break;
            }
        }

    }
    
}