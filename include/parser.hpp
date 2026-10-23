#ifndef PARSER_HPP
#define PARSER_HPP
#include <atomic>
#include "./packetQueue.hpp"
#include "./taskQueue.hpp"
#include "./packetHeader.hpp"
#include "./payload/packetPayload.hpp"
#include "./writer.hpp"

void parse(PacketQueue& packetQueue, TaskQueue& taskQueue,Writer& writer, std::atomic<bool>& isRunning);

#endif