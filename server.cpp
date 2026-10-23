#include "./server.hpp"


Server::Server(uint16_t portNumber) 
    :port(portNumber), serverSocket(INVALID_SOCKET), isRunning(false){
        /**
         * for now, there is nothing to initialize yet
         */
}

Server::~Server(){
    stop();
}
    int Server::initNetworkAPI(){
        /*we firstly need to create a socket */
        initSocket();
        serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if(serverSocket == INVALID_SOCKET){
            std::cerr << "Failed to create socket, try again later\n";
            return 1;
        }
        /* Now we shall bind the socket to a port*/
        sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddress),sizeof(serverAddress)) == SOCKET_ERROR){
            std::cerr << "failed to bind socket to port: " << port << " make sure all your inputs are correct and try again\n";
            closesocket(serverSocket);
            return 2;
        }
        return 0;
    }
    
    bool Server::initSocket(){
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2,2), &wsaData);
        return result == 0; 
    }
    
    bool Server::cleanSocket(){
        return WSACleanup() == 0;
    }

    void Server::listen(PacketQueue& queue, const bool& isRunning) {
        sockaddr_in clientAddress{};
        int clientLen = sizeof(clientAddress);
        std::vector<uint8_t> localTemp(2048);

        while (isRunning) {
            int bytesReceived = recvfrom(
                serverSocket,
                reinterpret_cast<char*>(localTemp.data()),
                static_cast<int>(localTemp.size()),
                0,
                reinterpret_cast<sockaddr*>(&clientAddress),
                &clientLen
            );

            if (bytesReceived == SOCKET_ERROR) {
                int err = WSAGetLastError();
                // WSAEINTR or WSAENOTSOCK usually happen when closesocket() is called to exit
                if (err != WSAEINTR && err != WSAENOTSOCK) {
                    std::cerr << "recvfrom failed with error code: " << err << "\n";
                }
                break;
            }

            if (bytesReceived > 0) {
                localTemp.resize(bytesReceived);
                
                // Push into the queue (handles mutex locking & condition variable notification)
                queue.push(localTemp);

                localTemp.resize(2048); // Reset capacity for next recvfrom
            }
        }
    }
    
    void Server::stop() {
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET; // Prevent double-closing
        }
        
        // Clean up Winsock subsystem
        cleanSocket(); 
    }
