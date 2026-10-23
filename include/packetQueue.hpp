#ifndef PACKETQUEUE_HPP
#define PACKETQUEUE_HPP
#include <queue>
#include <mutex>
#include <condition_variable>
#include <cstdint>
#include <vector>

class PacketQueue {
private:
    std::queue<std::vector<uint8_t>> queue;
    std::mutex mtx;
    std::condition_variable cv;
public:
    void push(std::vector<uint8_t> packet);
    std::vector<uint8_t> pop();
};

#endif