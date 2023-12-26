#include "CLIClient.hpp"

int main()
{
    std::string addr = "10.10.100.177";
    bool hasLoggedIn = false;
    std::string currentUser = "";

    Connect(currentUser, hasLoggedIn, addr);

    return 0;
}