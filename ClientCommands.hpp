#pragma once

#include <string>
#include <iostream>
#include <limits>
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
void UserMenu(const int& fd, string& _currentUser, bool& status);
void LogOut(string& _currentUser, bool& status);
void DeleteUser(const int& fd, string& _currentUser, bool& status);
void SendMessage(const int& fd, const string& fromUser);
void CheckMessage(const int& fd, const string& _currentUser);
void ShowUsers(const int& fd);

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

void LogOut(string& _currentUser, bool& status)
{
    _currentUser = "";
    status = false;
}

void DeleteUser(const int& fd, string& _currentUser, bool& status)
{
    std::cout << "Deleting user " << _currentUser << std::endl;
    std::string UID = "000#UIDB#" + _currentUser;
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

    if (strncmp("0001", serverResponse, 4) == 0)
    {
        std::cout << "User has been deleted!\n";     
        status = false;                 
    }    
    else 
    {
        std::cout << "Failed to delete user!\n";
    }
}

void SendMessage(const int &fd, const string &fromUser)
{
    string toUser, message, clientMessage;

    std::cout << "Enter recepient: ";
    std::cin >> toUser;
    std::cout << "Enter message: \n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, message);
    clientMessage = "200#FROM#" + fromUser + 
                    "#TO#" + toUser +
                    "#MSG#" + message; 

    bzero(clientRequest, sizeof(clientRequest));
    strncpy(clientRequest, clientMessage.c_str(), sizeof(clientRequest));

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

    if (strncmp("2001", serverResponse, 4) == 0)
    {
        std::cout << "Message has been sent!\n";                      
    }    
    else 
    {
        std::cout << "Failed to send message!\n";
    }
}

void CheckMessage(const int &fd, const string &_currentUser)
{
    std::string MSG = "210#UIDB#" + _currentUser;

    bzero(clientRequest, sizeof(clientRequest));
    strncpy(clientRequest, MSG.c_str(), sizeof(clientRequest));

    ssize_t bytes = send(fd, clientRequest, sizeof(clientRequest), 0);
    if (bytes >= 0)
    {
        std::cout << "Data was sent successfuly!\n";
    }
    
    bzero(serverResponse, sizeof(serverResponse));
    recv(fd, serverResponse, sizeof(serverResponse), 0);
    std::cout << serverResponse;   
    
}

void ShowUsers(const int & fd)
{
    bzero(clientRequest, sizeof(clientRequest));
    strncpy(clientRequest, "500", sizeof(clientRequest));

    ssize_t bytes = send(fd, clientRequest, sizeof(clientRequest), 0);
    if (bytes >= 0)
    {
        std::cout << "Data was sent successfuly!\n";
    }
    
    bzero(serverResponse, sizeof(serverResponse));
    recv(fd, serverResponse, sizeof(serverResponse), 0);
    std::cout << serverResponse;
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
void UserMenu(const int& fd, string& _currentUser, bool& status)
{
    string username, password;
    int menuOperator;
    
    std::cout << "Welcome user: " << _currentUser << "\nPlease choose your option:\n";
    std::cout << "\t1 - Show Messages\n"
        << "\t2 - Send Message\n"
        << "\t3 - Show Users\n"
        << "\t4 - Delete User\n"
        << "\t5 - Change Password\n"
        << "\t6 - Logout\n"
        << "\t0 - Exit\n";
    std::cin >> menuOperator;
    switch (menuOperator)
    {
    case 0:
        Terminator = true;
        break;
    case 1:
    {
        std::cout << "Messages: \n";
        CheckMessage(fd, _currentUser);
        break;
    }
    case 2:
    {
        std::cout << "Send message\n";
        SendMessage(fd, _currentUser);
        break;
    }
    case 3:
    {
        std::cout << "Registered users: \n";
        ShowUsers(fd);
        break;
    }
    case 4:
    {
        std::cout << "Are you sure you want to delete your user?(Y/N): ";
        char choise = 'Y';        
        std::cin >> choise;
        
        if (toupper(choise) == 'Y')
        {
            DeleteUser(fd, _currentUser, status);            
            break;         
        }
        break;
    }
    case 5:
    {
        std::cout << "Please enter old password: ";
        std::cin >> password;
        break;
    }
    case 6:
    {
        std::cout << "You have loged out\n";
        LogOut(_currentUser, status);
        break;
    }
    default:
        std::cout << "Wrong statement!\n";
        break;
    }

}
