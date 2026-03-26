#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "../enums.hpp"

struct SimulationEvent {
    int timestamp_seconds;
    std::string vehicle_id;
    EventType event_type;
    VehicleType vehicle_type;
    int duration_minutes;
};

struct ApplicationConfig {
    std::string parking_config_file;
    std::string simulation_events_file;
};

#endif // CONFIG_HPP