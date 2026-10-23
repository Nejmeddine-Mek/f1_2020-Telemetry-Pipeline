#include "../include/writer.hpp"
#include <iostream>
#include <iomanip>

// Helper to validate and print standard telemetry output
void Writer::write(const PacketMotionData& packet) {
    uint8_t pIdx = packet.m_header.m_playerCarIndex;
    const auto& motion = packet.m_carMotionData[pIdx];

    std::cout << "[Writer::Motion] Frame: " << packet.m_header.m_frameIdentifier
              << " | Pos (" << motion.m_worldPositionX << ", " 
              << motion.m_worldPositionY << ", " 
              << motion.m_worldPositionZ << ")"
              << " | G-Lat: " << motion.m_gForceLateral << "G\n";

    if (m_motionFile.is_open()) {
        m_motionFile << packet.m_header.m_frameIdentifier << ","
                     << packet.m_header.m_sessionTime << ","
                     << motion.m_worldPositionX << ","
                     << motion.m_worldPositionY << ","
                     << motion.m_worldPositionZ << ","
                     << motion.m_gForceLateral << ","
                     << motion.m_gForceLongitudinal << "\n";
        m_motionFile.flush();
    }
}

void Writer::write(const PacketSessionData& packet) {
    std::cout << "[Writer::Session] Track ID: " << static_cast<int>(packet.m_trackId)
              << " | Laps: " << static_cast<int>(packet.m_totalLaps)
              << " | Weather: " << static_cast<int>(packet.m_weather)
              << " | Track Temp: " << static_cast<int>(packet.m_trackTemperature) << "C\n";
}

void Writer::write(const PacketLapData& packet) {
    uint8_t pIdx = packet.m_header.m_playerCarIndex;
    const auto& lap = packet.m_lapData[pIdx];

    std::cout << "[Writer::Lap] Lap: " << static_cast<int>(lap.m_currentLapNum)
              << " | Pos: P" << static_cast<int>(lap.m_carPosition)
              << " | Current Time: " << lap.m_currentLapTime << "s"
              << " | Last Lap: " << lap.m_lastLapTime << "s\n";
}

void Writer::write(const PacketEventData& packet) {
    std::string eventCode(reinterpret_cast<const char*>(packet.m_eventStringCode), 4);
    std::cout << "[Writer::Event] Triggered: " << eventCode << "\n";
}

void Writer::write(const PacketParticipantsData& packet) {
    uint8_t pIdx = packet.m_header.m_playerCarIndex;
    const auto& player = packet.m_participants[pIdx];

    std::cout << "[Writer::Participants] Active Cars: " << static_cast<int>(packet.m_numActiveCars)
              << " | Player: " << player.m_name 
              << " (#" << static_cast<int>(player.m_raceNumber) << ")\n";
}

void Writer::write(const PacketCarSetupData& packet) {
    uint8_t pIdx = packet.m_header.m_playerCarIndex;
    const auto& setup = packet.m_carSetups[pIdx];

    std::cout << "[Writer::Setup] Wings (F/R): " << static_cast<int>(setup.m_frontWing) 
              << "/" << static_cast<int>(setup.m_rearWing)
              << " | Brake Bias: " << static_cast<int>(setup.m_brakeBias) << "%\n";
}

void Writer::write(const PacketCarTelemetryData& packet) {
    uint8_t pIdx = packet.m_header.m_playerCarIndex;
    const auto& telemetry = packet.m_carTelemetryData[pIdx];

    std::cout << "[Writer::Telemetry] Speed: " << telemetry.m_speed << " km/h"
              << " | RPM: " << telemetry.m_engineRPM
              << " | Gear: " << static_cast<int>(telemetry.m_gear)
              << " | Throttle: " << static_cast<int>(telemetry.m_throttle * 100.0f) << "%"
              << " | Brake: " << static_cast<int>(telemetry.m_brake * 100.0f) << "%\n";

    if (m_telemetryFile.is_open()) {
        m_telemetryFile << packet.m_header.m_frameIdentifier << ","
                        << packet.m_header.m_sessionTime << ","
                        << telemetry.m_speed << ","
                        << telemetry.m_engineRPM << ","
                        << static_cast<int>(telemetry.m_gear) << ","
                        << telemetry.m_throttle << ","
                        << telemetry.m_brake << "\n";
        m_telemetryFile.flush();
    }
}

void Writer::write(const PacketCarStatusData& packet) {
    uint8_t pIdx = packet.m_header.m_playerCarIndex;
    const auto& status = packet.m_carStatusData[pIdx];

    std::cout << "[Writer::Status] Fuel: " << status.m_fuelInTank << "L ("
              << status.m_fuelRemainingLaps << " laps left)"
              << " | ERS Mode: " << static_cast<int>(status.m_ersDeployMode)
              << " | Front Wings Damage (L/R): " << static_cast<int>(status.m_frontLeftWingDamage) 
              << "% / " << static_cast<int>(status.m_frontRightWingDamage) << "%\n";
}

}