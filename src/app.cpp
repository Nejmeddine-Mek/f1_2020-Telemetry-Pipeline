#include <iostream>
#include <cstdint>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>

#include "../include/server.hpp"
#include "../include/parser.hpp"
/*this should be ran using the command line, where we specify the port number as follows
* app.exe -p xxx
*/
using namespace std;



void startServer(PacketQueue& queue,uint16_t portNumber,atomic<bool>& isRunning);
void parseUDP_DGRAM(PacketQueue& packetQueue, TaskQueue& taskQueue, Writer& writer, atomic<bool>& isRunning);
void writeTelemetry(TaskQueue& taskQueue, atomic<bool>& isRunning);

int main(int argc, char **argv){
    if (argc < 3) {
            cerr << "Error: Insufficient parameters. Usage: " << argv[0] << " -p <port_number>\n";
            return 1;
        }

    uint16_t portNumber = 20777; // Default F1 UDP port
    if (string(argv[1]) == "-p") {
        portNumber = static_cast<uint16_t>(atoi(argv[2]));
    } else {
        portNumber = static_cast<uint16_t>(atoi(argv[1]));
    }

    if (portNumber < 1024) {
        cerr << "Error: Port numbers below 1024 are reserved system ports.\n";
        return 2;
    }

    cout << "==========================================\n";
    cout << " F1 2020 UDP Telemetry Server             \n";
    cout << " Port: " << portNumber << "\n";
    cout << "==========================================\n";

    atomic<bool> isRunning = true;
    // Instantiate pipeline objects (No empty parentheses!)
    PacketQueue dgramQueue;
    TaskQueue taskQueue;
    Writer writer;

    // Launch worker threads
    thread serverThread(startServer, ref(dgramQueue), portNumber, ref(isRunning));
    thread parserThread(parseUDP_DGRAM, ref(dgramQueue), ref(taskQueue), ref(writer), ref(isRunning));
    thread writeThread(writeTelemetry, ref(taskQueue), ref(isRunning));

    cout << "Server actively listening. Press ENTER to stop...\n";
    cin.get();

    cout << "\nInitiating clean shutdown...\n";
    isRunning = false;

    // Unblock network socket and thread-safe queues
    dgramQueue.stop();
    taskQueue.stop();

    // Safely synchronize thread exit
    if (serverThread.joinable()) serverThread.join();
    if (parserThread.joinable()) parserThread.join();
    if (writeThread.joinable()) writeThread.join();

    cout << "All telemetry pipeline threads terminated cleanly.\n";
    return 0;
}

void startServer(PacketQueue& queue,uint16_t portNumber,atomic<bool>& isRunning){
    Server server(portNumber);
    server.initNetworkAPI();
    server.listen(queue,isRunning);
}
void parseUDP_DGRAM(PacketQueue& packetQueue, TaskQueue& taskQueue, Writer& writer, atomic<bool>& isRunning){
    parse(packetQueue, taskQueue,writer, isRunning);
}

void writeTelemetry(TaskQueue& taskQueue, atomic<bool>& isRunning){
    while(isRunning){
        TaskQueue::Task task;
        if(taskQueue.pop(task)){
            task();
        }
    }
}