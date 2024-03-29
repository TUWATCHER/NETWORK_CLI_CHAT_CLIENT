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

struct User
{
    std::string _username;
    std::string _password;
    bool _status;
};

void Register(const int& fd, const User& _currentUser);
void Login(const int& fd, User& _currentUser);
void LoginMenu(const int& fd, User& _currentUser);
void UserMenu(const int& fd, User& _currentUser);
void LogOut(User& _currentUser);
void DeleteUser(const int& fd, User& _currentUser);
void SendMessage(const int& fd, const string& fromUser);
void CheckMessage(const int& fd, const string& _currentUser);
void ShowUsers(const int& fd);

void Register(const int& fd, const User& _currentUser)
{
    std::cout << "Registering user!\n";
    std::string UID = "101#UIDB#" + _currentUser._username +
                    "#UIDE#PWDB#" + _currentUser._password + "#PWDE#";
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
void Login(const int& fd, User& _currentUser)
{
    std::cout << "Logging user!\n";
    std::string UID = "111#UIDB#" + _currentUser._username + 
                    "#UIDE#PWDB#" + _currentUser._password + "#PWDE#";
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

    if (strncmp("1111", serverResponse, 4) == 0)
    {
        std::cout << "User has logged in!\n";
        _currentUser._status = true;                     
    }    
    else 
    {
        std::cout << "User failed to log in!\n";
        _currentUser._status = false;
    }
}

void LogOut(User& _currentUser)
{
    _currentUser._username = "";
    _currentUser._password = "";
    _currentUser._status = false;
}

void DeleteUser(const int& fd, User& _currentUser)
{
    std::cout << "Deleting user " << _currentUser._username << std::endl;
    std::string UID = "000#UIDB#" + _currentUser._username;
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
        LogOut(_currentUser);                
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

void LoginMenu(const int& fd, User& _currentUser)
{
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
        std::cin >> _currentUser._username;
        std::cout << "Please enter password: ";
        std::cin >> _currentUser._password;
        Register(fd, _currentUser);
        break;
    case 2:      
        std::cout << "Please enter username: ";
        std::cin >> _currentUser._username;
        std::cout << "Please enter password: ";
        std::cin >> _currentUser._password;
        Login(fd, _currentUser);
        break;
    default:
        std::cout << "Wrong statement!\n";
        break;
    }
}
void UserMenu(const int& fd, User& _currentUser)
{
    int menuOperator;
    
    std::cout << "Welcome user: " << _currentUser._username << "\nPlease choose your option:\n";
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
        CheckMessage(fd, _currentUser._username);
        break;
    }
    case 2:
    {
        std::cout << "Send message\n";
        SendMessage(fd, _currentUser._username);
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
            DeleteUser(fd, _currentUser);            
            break;         
        }
        break;
    }
    case 5:
    {
        //Change password not impemented!
        std::cout << "Please enter old password: ";
        break;
    }
    case 6:
    {
        std::cout << "You have loged out\n";
        LogOut(_currentUser);
        break;
    }
    default:
        std::cout << "Wrong statement!\n";
        break;
    }

}
