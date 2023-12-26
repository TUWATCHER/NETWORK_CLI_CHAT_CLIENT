#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "ClientCommands.hpp"

#define PORT 7777
#define CONNECTED 1

int connection, activity;
struct sockaddr_in serveraddress;
char *end;
int socket_file_descriptor;


void Connect(string& _currentUser, bool& status, string& addr)
{ 

    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1)
    {
        std::cout << "Failed to create socket!\n";
        exit(EXIT_FAILURE);
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = inet_addr(addr.c_str());
    serveraddress.sin_port = htons(PORT);
    
    connection = connect(socket_file_descriptor, (struct sockaddr*)& serveraddress, sizeof(serveraddress));
    if (connection == -1)
    {
        std::cout << "Failed to establish connectivity!\n";
        exit(EXIT_FAILURE);
    }
    else 
    {
        std::cout << "Connection with " << inet_ntoa(serveraddress.sin_addr)<< " has been established!\n";
    }

    bzero(serverResponse, sizeof(serverResponse));
    read(socket_file_descriptor, serverResponse, sizeof(serverResponse));
    std::cout << serverResponse << std::endl;

    bzero(serverResponse, sizeof(serverResponse));

    while (!Terminator)
    {

        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(socket_file_descriptor, &reads);

        #if !defined(_WIN32)
            FD_SET(0, &reads);
        #endif

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

        activity = select(socket_file_descriptor+1, &reads, 0, 0, &timeout);
        if (activity < 0)
        {
            std::cout << "Select error!\n";
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(socket_file_descriptor, &reads)) 
        
        {
            char read[4096];
            int bytes_received = recv(socket_file_descriptor, read, 4096, 0);
            if (bytes_received < 1) {
                printf("Connection closed by peer.\n");
                break;
            }
            
           // printf("%.*s", bytes_received, read);
        }

        #if defined(_WIN32)
            if(_kbhit()) {
        #else
        if(FD_ISSET(0, &reads))
        {
        #endif            
            if (status)
            {
                UserMenu(socket_file_descriptor, _currentUser, status);
            }
            else
            {
                LoginMenu(socket_file_descriptor, _currentUser, status);
            }
            
            
        }
    } //END WHILE(TRUE)

    printf("Closing socket...\n");
    close(socket_file_descriptor);

    #if defined(_WIN32)
        WSACleanup();
    #endif

    std::cout << "Finished.\n";
    
}




