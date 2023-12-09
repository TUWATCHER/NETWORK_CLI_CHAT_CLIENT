#include "CLIClient.h"

int main()
{
    bool hasLoggedIn = false;
    std::string currentUser;

    Connect(currentUser, hasLoggedIn);

    std::cout << "CURRENT USER: " << currentUser << std::endl;
    return 0;
}