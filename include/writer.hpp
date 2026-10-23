#ifndef WRITER_HPP
#define WRITER_HPP
#include "./payload/packetPayload.hpp"
#include <fstream>
#include <string>
#include <iostream>

class Writer {
private:
    std::ofstream m_motionFile;
    std::ofstream m_telemetryFile;
public:

    Writer(const std::string& outputFolder = "data/") {
        m_motionFile.open(outputFolder + "motion_log.csv", std::ios::out | std::ios::app);
        if (m_motionFile.is_open()) {
            // Write CSV header if file is empty
            m_motionFile.seekp(0, std::ios::end);
            if (m_motionFile.tellp() == 0) {
                m_motionFile << "Frame,SessionTime,WorldPosX,WorldPosY,WorldPosZ,GForceLat,GForceLong\n";
                m_motionFile.flush();
            }
        } else {
            std::cerr << "[Writer::Error] Failed to open " << outputFolder << "motion_log.csv\n";
        }

        m_telemetryFile.open(outputFolder + "telemetry_log.csv", std::ios::out | std::ios::app);
        if (m_telemetryFile.is_open()) {
            // Write CSV header if file is empty
            m_telemetryFile.seekp(0, std::ios::end);
            if (m_telemetryFile.tellp() == 0) {
                m_telemetryFile << "Frame,SessionTime,Speed,RPM,Gear,Throttle,Brake\n";
                m_telemetryFile.flush();
            }
        } else {
            std::cerr << "[Writer::Error] Failed to open " << outputFolder << "telemetry_log.csv\n";
        }
    }

    ~Writer() {
        if (m_motionFile.is_open()) {
            m_motionFile.flush();
            m_motionFile.close();
        }
        if (m_telemetryFile.is_open()) {
            m_telemetryFile.flush();
            m_telemetryFile.close();
        }
    }

    // The 8 root packet overloads
    void write(const PacketMotionData& packet);
    void write(const PacketSessionData& packet);
    void write(const PacketLapData& packet);
    void write(const PacketEventData& packet);
    void write(const PacketParticipantsData& packet);
    void write(const PacketCarSetupData& packet);
    void write(const PacketCarTelemetryData& packet);
    void write(const PacketCarStatusData& packet);

private:
    // Helper methods for writing specific car data if needed internally
    void writePlayerTelemetry(const CarTelemetryData& playerCar);
};
#endif

#endif