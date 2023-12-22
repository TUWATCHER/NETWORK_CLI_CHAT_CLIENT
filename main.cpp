#include "CLIClient.hpp"

int main()
{
    std::string addr = "172.16.8.129";
    bool hasLoggedIn = false;
    std::string currentUser = "";

    Connect(currentUser, hasLoggedIn, addr);

    return 0;
}