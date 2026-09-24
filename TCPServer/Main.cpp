//#include <SDL.h>
//#include <SDL3_net/SDL_net.h>
//
//#include <thread>
//#include <chrono>
//
//#include <cstdio>
//#include <cstring>
//#include <iostream>
//
//#define BUFFER_SIZE 1024
//
//NET_Server* server;
//NET_StreamSocket* socket; 
//
//int main(int argc, char *argv[]) {
//    bool done = false;
//    char data[BUFFER_SIZE];
//    
//    if (NET_Init() == false) {
//        std::cerr << SDL_GetError() << std::endl;
//        exit(EXIT_FAILURE);
//    }
//    
//    /// Create a "listener" waiting for and in coming connection on port 0x1A2C
//    server = NET_CreateServer(nullptr, 0x1a2c, 0);
//    if (server == nullptr) {
//        std::cerr << SDL_GetError() << std::endl;
//        NET_Quit;
//        exit(EXIT_FAILURE);
//    }
//
//    /// Wait for a connection
//    std::cout << "Waiting for a connection\n";
//    NET_WaitUntilInputAvailable((void**)&server, 1, -1);
//    NET_AcceptClient(server, &socket);
//
//
//    while( NET_AcceptClient(server, &socket) == false){
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//    }
//
//    /// I have a connection 
//    std::cout << "I have a connection\n";
//
//    NET_WaitUntilInputAvailable((void**)&socket, 1, -1);
//    NET_ReadFromStreamSocket(socket, data, BUFFER_SIZE);
//    std::cout << data << std::endl;
//
//
//    NET_DestroyServer(server);
//    
//    /// Let's work this next part out together, Assignment 1
//    
//    NET_DestroyStreamSocket(socket);
//    NET_Quit();
//    return 0;
//}



#include <SDL.h>
#include <SDL3_net/SDL_net.h>

#include <thread>
#include <chrono>

#include <cstdio>
#include <cstring>
#include <iostream>

#define BUFFER_SIZE 1024

NET_Server* server;
NET_StreamSocket* socket;

int main(int argc, char* argv[]) {
    bool done = false;
    char data[BUFFER_SIZE];

    if (NET_Init() == false) {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    /// Create a "listener" waiting for an incoming connection on port 0x1A2C
    server = NET_CreateServer(nullptr, 0x1a2c, 0);
    if (server == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        NET_Quit();
        exit(EXIT_FAILURE);
    }

    /// Wait for a connection
    std::cout << "Waiting for a connection\n";
    NET_WaitUntilInputAvailable((void**)&server, 1, -1);
    while (NET_AcceptClient(server, &socket) == false) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    /// I have a connection 
    std::cout << "I have a connection\n";

    NET_DestroyServer(server);

    /// Assignment 1: keep receiving and sending until someone types "quit"
    while (!done) {
        /// Wait for the client to send something, then read it
        NET_WaitUntilInputAvailable((void**)&socket, 1, -1);
        NET_ReadFromStreamSocket(socket, data, BUFFER_SIZE);
        std::cout << "Client: " << data << std::endl;

        if (strcmp(data, "quit") == 0) {
            done = true;
            break;
        }

        /// Now it's our turn to type a reply and send it
        std::cout << "You: ";
        std::cin.getline(data, BUFFER_SIZE);
        NET_WriteToStreamSocket(socket, data, (int)strlen(data) + 1);

        if (strcmp(data, "quit") == 0) {
            done = true;
        }
    }

    NET_DestroyStreamSocket(socket);
    NET_Quit();
    return 0;
}