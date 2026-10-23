#ifndef PACKETPAYLOAD_HPP
#define PACKETPAYLOAD_HPP

#include <cstdint>
#include "../packetHeader.hpp"

#pragma pack(push, 1)

// ============================================================================
// PACKET ID 0: MOTION DATA
// ============================================================================
struct CarMotionData {
    float m_worldPositionX;      // World space X position
    float m_worldPositionY;      // World space Y position
    float m_worldPositionZ;      // World space Z position
    float m_worldVelocityX;      // Velocity in world space X
    float m_worldVelocityY;      // Velocity in world space Y
    float m_worldVelocityZ;      // Velocity in world space Z
    int16_t m_worldForwardX;     // World space forward X direction (normalised)
    int16_t m_worldForwardY;     // World space forward Y direction (normalised)
    int16_t m_worldForwardZ;     // World space forward Z direction (normalised)
    int16_t m_worldRightX;       // World space right X direction (normalised)
    int16_t m_worldRightY;       // World space right Y direction (normalised)
    int16_t m_worldRightZ;       // World space right Z direction (normalised)
    float m_gForceLateral;       // Lateral G-Force component
    float m_gForceLongitudinal;  // Longitudinal G-Force component
    float m_gForceVertical;      // Vertical G-Force component
    float m_yaw;                 // Yaw angle in radians
    float m_pitch;               // Pitch angle in radians
    float m_roll;                // Roll angle in radians
};

struct PacketMotionData {
    PacketHeader    m_header;
    CarMotionData   m_carMotionData[22]; // Data for all 22 cars
    
    // Extra player-only motion data
    float           m_suspensionPosition[4];     // RL, RR, FL, FR
    float           m_suspensionVelocity[4];     // RL, RR, FL, FR
    float           m_suspensionAcceleration[4]; // RL, RR, FL, FR
    float           m_wheelSpeed[4];             // Speed of each wheel
    float           m_wheelSlip[4];              // Slip ratio for each wheel
    float           m_localVelocityX;            // Velocity in local space
    float           m_localVelocityY;            // Velocity in local space
    float           m_localVelocityZ;            // Velocity in local space
    float           m_angularVelocityX;          // Angular velocity x component
    float           m_angularVelocityY;          // Angular velocity y component
    float           m_angularVelocityZ;          // Angular velocity z component
    float           m_angularAccelerationX;      // Angular acceleration x component
    float           m_angularAccelerationY;      // Angular acceleration y component
    float           m_angularAccelerationZ;      // Angular acceleration z component
    float           m_frontWheelsAngle;          // Current front wheels angle in radians
};

// ============================================================================
// PACKET ID 1: SESSION DATA
// ============================================================================
struct MarshalZone {
    float m_zoneStart; // Fraction (0..1) of way through lap where zone starts
    int8_t m_zoneFlag; // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow, 4 = red
};

struct WeatherForecastSample {
    uint8_t m_sessionType;            // 0 = unknown, 1 = P1, 2 = P2, 3 = P3, 4 = Short P, 5 = Q1, etc.
    uint8_t m_timeOffset;             // Time in minutes the forecast is for
    uint8_t m_weather;                // 0 = clear, 1 = light cloud, 2 = overcast, 3 = light rain, 4 = heavy rain, 5 = storm
    int8_t  m_trackTemperature;       // Track temp in degrees Celsius
    int8_t  m_airTemperature;         // Air temp in degrees Celsius
};

struct PacketSessionData {
    PacketHeader          m_header;
    uint8_t               m_weather;              // Weather status
    int8_t                m_trackTemperature;     // Track temp in degrees Celsius
    int8_t                m_airTemperature;       // Air temp in degrees Celsius
    uint8_t               m_totalLaps;            // Total laps in race
    uint16_t              m_trackLength;          // Track length in metres
    uint8_t               m_sessionType;          // Session type ID
    int8_t                m_trackId;              // Track ID (-1 for unknown, 0-21)
    uint8_t               m_formula;              // 0 = F1 Modern, 1 = F1 Classic, 2 = F2, 3 = F1 Generic
    uint16_t              m_sessionTimeLeft;      // Time left in session in seconds
    uint16_t              m_sessionDuration;      // Total duration in seconds
    uint8_t               m_pitSpeedLimit;        // Pit speed limit in kph
    uint8_t               m_gamePaused;           // Whether the game is paused
    uint8_t               m_isSpectator;          // Whether player is spectating
    uint8_t               m_spectatorCarIndex;    // Index of car being spectated
    uint8_t               m_sliProNativeSupport;  // SLI Pro support
    uint8_t               m_numMarshalZones;      // Number of marshal zones
    MarshalZone           m_marshalZones[21];     // List of marshal zones
    uint8_t               m_safetyCarStatus;      // 0 = no SC, 1 = full SC, 2 = VSC, 3 = formation lap
    uint8_t               m_networkGame;          // 0 = offline, 1 = online
    uint8_t               m_numWeatherForecastSamples; // Number of forecast samples
    WeatherForecastSample m_weatherForecastSamples[20];
};

// ============================================================================
// PACKET ID 2: LAP DATA
// ============================================================================
struct LapData {
    float    m_lastLapTime;           // Last lap time in seconds
    float    m_currentLapTime;        // Current time around lap in seconds
    uint16_t m_sector1TimeInMS;       // Sector 1 time in milliseconds
    uint16_t m_sector2TimeInMS;       // Sector 2 time in milliseconds
    float    m_bestLapTime;           // Best lap time of session in seconds
    uint8_t  m_bestLapNum;            // Lap number best time was achieved on
    uint16_t m_bestLapSector1TimeInMS;// Best Sector 1 time in milliseconds
    uint16_t m_bestLapSector2TimeInMS;// Best Sector 2 time in milliseconds
    uint16_t m_bestLapSector3TimeInMS;// Best Sector 3 time in milliseconds
    uint16_t m_bestOverallSector1TimeInMS; // Best overall sector 1 time
    uint8_t  m_bestOverallSector1LapNum;   // Lap number best overall sector 1 was set
    uint16_t m_bestOverallSector2TimeInMS; // Best overall sector 2 time
    uint8_t  m_bestOverallSector2LapNum;   // Lap number best overall sector 2 was set
    uint16_t m_bestOverallSector3TimeInMS; // Best overall sector 3 time
    uint8_t  m_bestOverallSector3LapNum;   // Lap number best overall sector 3 was set
    float    m_lapDistance;           // Distance vehicle is around current lap in metres
    float    m_totalDistance;         // Total distance travelled in session in metres
    float    m_safetyCarDelta;        // Delta time for SC in seconds
    uint8_t  m_carPosition;           // Car race position
    uint8_t  m_currentLapNum;         // Current lap number
    uint8_t  m_pitStatus;             // 0 = none, 1 = pitting, 2 = in pit area
    uint8_t  m_sector;                // 0 = sector1, 1 = sector2, 2 = sector3
    uint8_t  m_currentLapInvalid;     // Current lap invalid (0 = valid, 1 = invalid)
    uint8_t  m_penalties;             // Accumulated time penalties in seconds
    uint8_t  m_gridPosition;          // Grid position car started the race in
    uint8_t  m_driverStatus;          // 0 = in garage, 1 = flying lap, 2 = in lap, 3 = out lap, 4 = on track
    uint8_t  m_resultStatus;          // 0 = invalid, 1 = inactive, 2 = active, 3 = finished, 4 = DNF, 5 = DSQ, 6 = retired
};

struct PacketLapData {
    PacketHeader m_header;
    LapData      m_lapData[22];       // Lap data for all 22 cars
};

// ============================================================================
// PACKET ID 3: EVENT DATA
// ============================================================================
union EventDataDetails {
    struct {
        uint8_t vehicleIdx;
        float lapTime;
    } FastestLap;

    struct {
        uint8_t vehicleIdx;
    } Retirement;

    struct {
        uint8_t vehicleIdx;
    } TeamMateInPits;

    struct {
        uint8_t vehicleIdx;
    } ChequeredFlag;

    struct {
        uint8_t vehicleIdx;
        float speed;
    } SpeedTrap;

    struct {
        uint8_t penaltyType;          // Penalty type enum
        uint8_t infringementType;     // Infringement enum
        uint8_t vehicleIdx;           // Vehicle index
        uint8_t otherVehicleIdx;      // Other vehicle index
        uint8_t time;                 // Time gain / penalty value
        uint8_t lapNum;               // Lap number
        uint8_t placesGained;         // Places gained
    } Penalty;

    struct {
        uint8_t vehicleIdx;           // Vehicle index
        float speed;                  // Speed in kph
    } Flashback;

    struct {
        uint32_t buttonStatus;        // Bit map showing buttons pressed
    } Buttons;
};

struct PacketEventData {
    PacketHeader     m_header;
    uint8_t          m_eventStringCode[4]; // Event code (e.g. "FTLP", "SSTA")
    EventDataDetails m_eventDetails;       // Union details matching eventStringCode
};

// ============================================================================
// PACKET ID 4: PARTICIPANTS DATA
// ============================================================================
struct ParticipantData {
    uint8_t m_aiControlled;           // Whether the car is AI (1) or Human (0)
    uint8_t m_driverId;               // Driver ID
    uint8_t m_teamId;                 // Team ID
    uint8_t m_raceNumber;             // Race number
    uint8_t m_nationality;            // Nationality ID
    char    m_name[48];               // Name in UTF-8 format (null-terminated)
    uint8_t m_yourTelemetry;          // 0 = restricted, 1 = public
};

struct PacketParticipantsData {
    PacketHeader    m_header;
    uint8_t         m_numActiveCars;  // Number of active cars in data
    ParticipantData m_participants[22];
};

// ============================================================================
// PACKET ID 5: CAR SETUPS DATA
// ============================================================================
struct CarSetupData {
    uint8_t m_frontWing;              // Front wing aero
    uint8_t m_rearWing;               // Rear wing aero
    uint8_t m_onThrottle;             // Differential adjustment on throttle (%)
    uint8_t m_offThrottle;            // Differential adjustment off throttle (%)
    float   m_frontCamber;            // Front camber angle
    float   m_rearCamber;             // Rear camber angle
    float   m_frontToe;               // Front toe angle
    float   m_rearToe;                // Rear toe angle
    uint8_t m_frontSuspension;        // Front suspension
    uint8_t m_rearSuspension;         // Rear suspension
    uint8_t m_frontAntiRollBar;       // Front anti-roll bar
    uint8_t m_rearAntiRollBar;        // Rear anti-roll bar
    uint8_t m_frontSuspensionHeight;  // Front ride height
    uint8_t m_rearSuspensionHeight;   // Rear ride height
    uint8_t m_brakePressure;          // Brake pressure (%)
    uint8_t m_brakeBias;              // Front brake bias (%)
    float   m_rearTyrePressure;       // Rear tyre pressure (PSI)
    float   m_frontTyrePressure;      // Front tyre pressure (PSI)
    uint8_t m_ballast;                // Ballast
    float   m_fuelLoad;               // Fuel load
};

struct PacketCarSetupData {
    PacketHeader m_header;
    CarSetupData m_carSetups[22];
};

// ============================================================================
// PACKET ID 6: CAR TELEMETRY DATA
// ============================================================================
struct CarTelemetryData {
    uint16_t m_speed;                         // Speed in kilometers per hour
    float    m_throttle;                      // Amount of throttle applied (0.0 to 1.0)
    float    m_steer;                         // Steering (-1.0 full left to 1.0 full right)
    float    m_brake;                         // Amount of brake applied (0.0 to 1.0)
    uint8_t  m_clutch;                        // Amount of clutch applied (0 to 100)
    int8_t   m_gear;                          // Gear selected (1-8, N=0, R=-1)
    uint16_t m_engineRPM;                     // Engine RPM
    uint8_t  m_drs;                           // 0 = off, 1 = on
    uint8_t  m_revLightsPercent;              // Rev lights indicator (%)
    uint16_t m_revLightsBitValue;             // Rev lights (bit mapping)
    uint16_t m_brakesTemperature[4];          // Brakes temperature (Celsius) [RL, RR, FL, FR]
    uint8_t  m_tyresSurfaceTemperature[4];    // Tyres surface temperature (Celsius)
    uint8_t  m_tyresInnerTemperature[4];      // Tyres inner temperature (Celsius)
    uint16_t m_engineTemperature;             // Engine temperature (Celsius)
    float    m_tyresPressure[4];              // Tyres pressure (PSI)
    uint8_t  m_surfaceType[4];                // Driving surface type
};

struct PacketCarTelemetryData {
    PacketHeader     m_header;
    CarTelemetryData m_carTelemetryData[22];
    uint32_t         m_buttonStatus;          // Bit map showing buttons pressed
    uint8_t          m_mfdPanelIndex;         // MFD panel index
    uint8_t          m_mfdPanelIndexSecondaryPlayer;
    int8_t           m_suggestedGear;         // Suggested gear (1-8, 0 if none)
};

// ============================================================================
// PACKET ID 7: CAR STATUS DATA (CORRECTED)
// ============================================================================
struct CarStatusData {
    uint8_t  m_tractionControl;         // 0 = off, 1 = medium, 2 = full
    uint8_t  m_antiLockBrakes;          // 0 = off, 1 = on
    uint8_t  m_fuelMix;                 // 0 = lean, 1 = standard, 2 = rich, 3 = max
    uint8_t  m_frontBrakeBias;          // Front brake bias (%)
    uint8_t  m_pitLimiterStatus;        // Pit limiter status (0 = off, 1 = on)
    float    m_fuelInTank;              // Current fuel mass (FLOAT)
    float    m_fuelCapacity;            // Fuel capacity (FLOAT)
    float    m_fuelRemainingLaps;       // Fuel remaining in terms of laps (FLOAT)
    uint16_t m_maxRPM;                  // Cars max RPM, point of rev limiter
    uint16_t m_idleRPM;                 // Cars idle RPM
    uint8_t  m_maxGears;                // Maximum number of gears
    uint8_t  m_drsAllowed;              // 0 = not allowed, 1 = allowed, -1 = unknown
    uint16_t m_drsActivationDistance;  // 0 = DRS not available, non-zero = distance in metres
    uint8_t  m_tyresWear[4];            // Tyre wear percentage
    uint8_t  m_actualTyreCompound;      // Actual tyre compound
    uint8_t  m_visualTyreCompound;      // Visual tyre compound
    uint8_t  m_tyresAgeLaps;            // Age of tyres in laps
    uint8_t  m_tyresDamage[4];          // Tyre damage percentage
    uint8_t  m_frontLeftWingDamage;     // Front left wing damage (%)
    uint8_t  m_frontRightWingDamage;    // Front right wing damage (%)
    uint8_t  m_rearWingDamage;          // Rear wing damage (%)
    uint8_t  m_drsFault;                // DRS fault (0 = OK, 1 = fault)
    uint8_t  m_engineDamage;            // Engine damage (%)
    uint8_t  m_gearBoxDamage;           // Gearbox damage (%)
    int8_t   m_vehicleFiaFlags;         // -1 = invalid, 0 = none, 1 = green, 2 = blue, 3 = yellow, 4 = red
    float    m_ersStoreEnergy;          // ERS energy store in Joules
    uint8_t  m_ersDeployMode;           // ERS deploy mode (0 = none, 1 = medium, 2 = overtake, 3 = hotlap)
    float    m_ersHarvestedThisLapMGUK; // ERS energy harvested this lap by MGU-K
    float    m_ersHarvestedThisLapMGUH; // ERS energy harvested this lap by MGU-H
    float    m_ersDeployedThisLap;      // ERS energy deployed this lap
};

struct PacketCarStatusData {
    PacketHeader  m_header;
    CarStatusData m_carStatusData[22];
};

#pragma pack(pop)
#endif