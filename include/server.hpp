
#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdint>
#include <winsock2.h> // Needed for SOCKET and WinSock types
#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <mutex>
#include "./packetQueue.hpp"
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;

class Server {
private:
    uint16_t port;        // Standard port type (uint16_t is preferred over uint64_t)
    SOCKET serverSocket;  // Defined in <winsock2.h>
    bool isRunning;

public:
    explicit Server(uint16_t portNumber);
    ~Server();

    int initNetworkAPI();
    bool initSocket();
    bool cleanSocket();
    // Note: templated member functions or 'auto' parameters usually require
    // implementation in the header or using explicit types/templates.
    
    void listen(PacketQueue& queue, const bool& isRunning) ;

    void stop();
};

#endif // SERVER_HPP