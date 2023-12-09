#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <inttypes.h>

#define MESSAGE_LENGTH 1024

using std::string;

static bool Terminator = false;
char clientRequest[MESSAGE_LENGTH];
char serverResponse[MESSAGE_LENGTH];

void Register(const int& fd, const std::string& username, const std::string& password);
bool Login(const int& fd, const std::string& username, const std::string& password);
void LoginMenu(const int& fd, string& _currentUser, bool& status);
void UserMenu(const string& _currentUser, bool& status);

void Register(const int& fd, const std::string& username, const std::string& password)
{
    std::cout << "Registering user!\n";
    std::string UID = "101#UIDB#" + username + "#UIDE#PWDB#" + password + "#PWDE#";
    std::cout << "UID: " << UID << std::endl;

    bzero(clientRequest, sizeof(clientRequest)); 
    strncpy(clientRequest, UID.c_str(), sizeof(clientRequest));       
  
    ssize_t bytes = send(fd, clientRequest, sizeof(clientRequest), 0);
    if (bytes >= 0)
        {
               std::cout << "Data was sent successfuly!\n";
        }
    
    bzero(serverResponse, sizeof(serverResponse));
    bytes = recv(fd, serverResponse, sizeof(serverResponse), 0);
    if (bytes >= 0)
    {
        std::cout << "Received response from server: " << bytes << std::endl;
    }

    if (strncmp("1010", serverResponse, 4) == 0)
    {
        std::cout << "User failed to Register!\n";                        
    }
    else if (strncmp("1011", serverResponse, 4) == 0)
    {
        std::cout << "User registered successfully!\n";                        
    }
    else 
    {
        std::cout << "Failed to parse response from server\n";
    }
}
bool Login(const int& fd, const std::string& username, const std::string& password)
{
    std::cout << "Logging user!\n";
    std::string UID = "111#UIDB#" + username + "#UIDE#PWDB#" + password + "#PWDE#";
    std::cout << "UID: " << UID << std::endl;
    int32_t securityToken = 0;

    bzero(clientRequest, sizeof(clientRequest)); 
    strncpy(clientRequest, UID.c_str(), sizeof(clientRequest));       
  
    ssize_t bytes = send(fd, clientRequest, sizeof(clientRequest), 0);
    if (bytes >= 0)
        {
               std::cout << "Data was sent successfuly!\n";
        }
    
    bzero(serverResponse, sizeof(serverResponse));
    bytes = recv(fd, serverResponse, sizeof(serverResponse), 0);
    if (bytes >= 0)
    {
        std::cout << "Received response from server: " << bytes << std::endl;
    }

    if (strncmp("1111", serverResponse, 4) == 0)
    {
        std::cout << "User has logged in!\n";
        return true;                      
    }    
    else 
    {
        std::cout << "User failed to log in!\n";
        return false;
    }
}

void LoginMenu(const int& fd, string& _currentUser, bool& status)
{
    std::string username, password;
    int menuOperator = 0;
    std::cout << "Welcome to CLI Chat, please choose your option:\n";
    std::cout << "\t0 - Exit\n"
        << "\t1 - Register\n"
        << "\t2 - Login\n"
        << "\t3 - Forgot Password\n";  
      
    while (!(std::cin >> menuOperator))
    {

        std::cin.clear();

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid input! Please try again with valid input: ";
    }

    switch (menuOperator)
    {
    case 0:
        std::cout << "Closing application!\n";
        Terminator = true;
        break;
    case 1:
        std::cout << "Please enter username: ";
        std::cin >> username;
        std::cout << "Please enter password: ";
        std::cin >> password;
        Register(fd, username, password);
        break;
    case 2:      
        std::cout << "Please enter username: ";
        std::cin >> username;
        std::cout << "Please enter password: ";
        std::cin >> password;
        status = Login(fd, username, password);
        if (status)
        {
            _currentUser = username;
        }
        break;
    default:
        std::cout << "Wrong statement!\n";
        break;
    }
}
void UserMenu(const string& _currentUser, bool& status)
{
    string username, password;
    int menuOperator;
    
    std::cout << "Welcome user: " << _currentUser << "\nPlease choose your option:\n";
    std::cout << "\t1 - Show Messages\n"
        << "\t2 - Send Message\n"
        << "\t3 - Logout\n"
        << "\t4 - Delete User\n"
        << "\t5 - Change Password\n"
        << "\t0 - Exit\n";
    std::cin >> menuOperator;
    switch (menuOperator)
    {
    case 0:
        Terminator = true;
        break;
    case 1:
        std::cout << "Messages: \n";
        //ShowMessage(_ChatDB, _currentUser);
        break;
    case 2:
        std::cout << "Send message\n";
        //CreateMessage(_UserDB, _ChatDB, _currentUser);
        break;
    case 3:
        std::cout << "You has logout\n";
        //Logout(_currentUser, status);
        break;
    case 4:
    {
        std::cout << "Are you sure you want to delete your user?(Y/N): ";
        char choise = 'Y';        
        std::cin >> choise;
        /*
        if (toupper(choise) == 'Y')
        {
            DeleteUser(_currentUser, _UserDB, status);
            std::cout << "User has been successfuly deleted!\n";
            break;
        } 
        */       
        break;
    }
    case 5:
        std::cout << "Please enter old password: ";
        std::cin >> password;
        break;
    default:
        std::cout << "Wrong statement!\n";
        break;
    }

}
int32_t S32(const char *s)
{
  int32_t i;
  char c ;
  int scanned = sscanf(s, "%" SCNd32 "%c", &i, &c);
  if (scanned == 1) return i;
  if (scanned > 1) {
    // TBD about extra data found
    return i;
    }
  // TBD failed to scan;  
  return 0;  
}