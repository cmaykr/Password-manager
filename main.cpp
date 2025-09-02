#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

#include "mockUserInterface.hpp"

MockUserInterface userInterface;

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
                        loggedIn = true;
                    }
                    else
                    {
                        std::cout << "Wrong password. Try again." << std::endl;
                        tries++;
                    }
                } while (loggedIn == false && tries < 3);

                if (loggedIn)
                    userMenu(loggedInUser);
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
    std::cout << "Successfully logged in!" << std::endl;
}