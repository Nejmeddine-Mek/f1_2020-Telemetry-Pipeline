#include "../include/parser.hpp"

void parse(PacketQueue& packetQueue, TaskQueue& taskQueue,Writer& writer, std::atomic<bool>& isRunning){
    while(isRunning){
        std::vector<uint8_t> rawPacket = packetQueue.pop();

        if(rawPacket.empty()){
            break;
        }

        if(rawPacket.size() < sizeof(PacketHeader)){
            continue;
        }
        // 3. Reinterpret byte buffer as PacketHeader
        const auto* header = reinterpret_cast<const PacketHeader*>(rawPacket.data());

        if (header->m_packetFormat != 2020) {
            continue;
        }
        switch(header->m_packetId){
            case 0: { // Motion
                if (rawPacket.size() >= sizeof(PacketMotionData)) {
                    PacketMotionData data = *reinterpret_cast<const PacketMotionData*>(rawPacket.data());
                    taskQueue.push([&writer, data]() { writer.write(data); });
                }
                break;
            }
            case 1: { // Session
                if (rawPacket.size() >= sizeof(PacketSessionData)) {
                    PacketSessionData data = *reinterpret_cast<const PacketSessionData*>(rawPacket.data());
                    taskQueue.push([&writer, data]() { writer.write(data); });
                }
                break;
            }
            case 2: { // Lap Data
                if (rawPacket.size() >= sizeof(PacketLapData)) {
                    PacketLapData data = *reinterpret_cast<const PacketLapData*>(rawPacket.data());
                    taskQueue.push([&writer, data]() { writer.write(data); });
                }
                break;
            }
            case 3: { // Event Data
                if (rawPacket.size() >= sizeof(PacketEventData)) {
                    PacketEventData data = *reinterpret_cast<const PacketEventData*>(rawPacket.data());
                    taskQueue.push([&writer, data]() { writer.write(data); });
                }
                break;
            }
            case 4: { // Participants
                if (rawPacket.size() >= sizeof(PacketParticipantsData)) {
                    PacketParticipantsData data = *reinterpret_cast<const PacketParticipantsData*>(rawPacket.data());
                    taskQueue.push([&writer, data]() { writer.write(data); });
                }
                break;
            }
            case 5: { // Car Setups
                if (rawPacket.size() >= sizeof(PacketCarSetupData)) {
                    PacketCarSetupData data = *reinterpret_cast<const PacketCarSetupData*>(rawPacket.data());
                    taskQueue.push([&writer, data]() { writer.write(data); });
                }
                break;
            }
            case 6: { // Car Telemetry
                if (rawPacket.size() >= sizeof(PacketCarTelemetryData)) {
                    PacketCarTelemetryData data = *reinterpret_cast<const PacketCarTelemetryData*>(rawPacket.data());
                    taskQueue.push([&writer, data]() { writer.write(data); });
                }
                break;
            }
            case 7: { // Car Status
                if (rawPacket.size() >= sizeof(PacketCarStatusData)) {
                    PacketCarStatusData data = *reinterpret_cast<const PacketCarStatusData*>(rawPacket.data());
                    taskQueue.push([&writer, data]() { writer.write(data); });
                }
                break;
            }
            default:
                break;
        }

    }
}