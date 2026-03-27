#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <chrono>
#include "../enums.hpp"

struct SimulationEvent {
    int timestamp_seconds;
    std::string vehicle_id;
    EventType event_type;
    VehicleType vehicle_type;
    int duration_minutes;
};

struct SimulationStatistics {
    int total_arrivals;
    int total_departures;
    int total_allocations;
    int total_rejections;
    int motorcycle_rejections;
    int car_rejections;
    int current_parked_count;
    double current_occupancy_rate;
    double average_parking_duration;
    int peak_occupancy;
};

struct ApplicationConfig {
    std::string parking_config_file;
    std::string simulation_events_file;
    std::string output_file;
    bool verbose;
};

#endif // CONFIG_HPP
