#pragma once

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <limits>
#include <sys/socket.h>
#include <arpa/inet.h>


#define MESSAGE_LENGTH 1024
#define PORT 7777
#define CONNECTED 1

using std::string;

void ConnectServer(bool& status);

int socket_file_descriptor;
int connection;
struct sockaddr_in serveraddress, client;
char clientRequest[MESSAGE_LENGTH];
char serverResponse[MESSAGE_LENGTH];
std::string message;



void ConnectServer(bool& status)
{
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1)
    {
        std::cout << "Failed to create socket!\n";
        exit(1);
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddress.sin_port = htons(PORT);
    
    connection = connect(socket_file_descriptor, (struct sockaddr*)& serveraddress, sizeof(serveraddress));
    if (connection == -1)
    {
        std::cout << "Failed to establish connectivity!\n";
        exit(1);
    }
    else 
    {
        std::cout << "Connection with " << inet_ntoa(serveraddress.sin_addr)<< " has been established!\n";
    }

    bzero(serverResponse, sizeof(serverResponse));
    read(socket_file_descriptor, serverResponse, sizeof(serverResponse));
    std::cout << serverResponse << std::endl;

    bzero(serverResponse, sizeof(serverResponse));

    while (true)
    {
        std::cout << "Type your message (Type 0000 for exit):\n";
        bzero(clientRequest, sizeof(clientRequest));
        std::cin >> message;
        if (message == "0000")
        {
            std::cout << "Terminating program!\n";
            break;
        }                
        strcpy(clientRequest, message.c_str());

        ssize_t bytes = send(socket_file_descriptor, clientRequest, sizeof(clientRequest), 0);
        if (bytes >= 0)
            {
                std::cout << "Data was sent successfuly!\n";
            }
        
        bzero(serverResponse, sizeof(serverResponse));
        read(socket_file_descriptor, serverResponse, sizeof(serverResponse));
        std::cout << serverResponse << std::endl;

    }
    close(socket_file_descriptor);

}