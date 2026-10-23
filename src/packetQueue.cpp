#include "../include/packetQueue.hpp"

    // Producer (Server Thread) calls this
    void PacketQueue::push(std::vector<uint8_t> packet) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(std::move(packet));
        }
        cv.notify_one(); // Wake up the parsing thread!
    }

    // Consumer (Parser Thread) calls this
    std::vector<uint8_t> PacketQueue::pop() {
        std::unique_lock<std::mutex> lock(mtx);
        
        // Sleep until the queue is NOT empty
        cv.wait(lock, [this] { return !queue.empty(); });

        std::vector<uint8_t> packet = std::move(queue.front());
        queue.pop();
        return packet;
    }
