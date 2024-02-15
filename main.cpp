#include "CLIClient.hpp"

int main()
{
    std::string addr = "10.10.100.177";
    User _currentUser;

    Connect(_currentUser, addr);

    return 0;
}