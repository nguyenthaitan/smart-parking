#include "simulation_runner.hpp"
#include <iostream>
#include <chrono>
#include <algorithm>

SimulationRunner::SimulationRunner() : initialized_(false) {
}

bool SimulationRunner::initializeSimulation(const ApplicationConfig& config) {
    config_ = config;

    // Load parking lot configuration
    std::vector<ParkingSpace> spaces;
    if (!loader_.loadParkingLotConfig(config.parking_config_file, spaces)) {
        std::cerr << "Failed to load parking config: " << loader_.getLastError() << std::endl;
        return false;
    }

    parking_lot_.loadConfiguration(spaces);

    // Load simulation events
    if (!loader_.loadSimulationEvents(config.simulation_events_file, events_)) {
        std::cerr << "Failed to load simulation events: " << loader_.getLastError() << std::endl;
        return false;
    }

    // Sort events by timestamp
    std::sort(events_.begin(), events_.end(),
             [](const SimulationEvent& a, const SimulationEvent& b) {
                 return a.timestamp_seconds < b.timestamp_seconds;
             });

    initialized_ = true;
    if (config.verbose) {
        std::cout << "Simulation initialized with " << spaces.size() << " parking spaces and "
                 << events_.size() << " events." << std::endl;
    }

    return true;
}

bool SimulationRunner::runSimulation() {
    if (!initialized_) {
        std::cerr << "Simulation not initialized" << std::endl;
        return false;
    }

    if (config_.verbose) {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "PARKING LOT SIMULATION - DETAILED VIEW\n";
        std::cout << std::string(60, '=') << "\n\n";
    }

    int event_count = 0;
    for (const auto& event : events_) {
        event_count++;
        
        if (event.event_type == EventType::ARRIVAL) {
            auto result = parking_lot_.arrivedVehicle(
                event.vehicle_id,
                event.vehicle_type,
                event.duration_minutes,
                std::chrono::system_clock::now()
            );

            if (config_.verbose) {
                logEvent(event_count, event, result);
            }
        } else {
            parking_lot_.departedVehicle(
                event.vehicle_id,
                std::chrono::system_clock::now()
            );

            if (config_.verbose) {
                auto stats = parking_lot_.getStatistics();
                std::string type = (event.vehicle_type == VehicleType::MOTORCYCLE ? "MOTORCYCLE" : "CAR");
                std::cout << "[" << event_count << "] DEPARTURE V" << event.vehicle_id 
                         << " (" << type << ") [" << stats.current_parked_count << "/480]\n";
            }
        }
    }

    if (config_.verbose) {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "SIMULATION COMPLETE\n";
        std::cout << std::string(60, '=') << "\n\n";
    }

    return true;
}

SimulationStatistics SimulationRunner::getStatistics() const {
    return parking_lot_.getStatistics();
}

std::string SimulationRunner::getDetailedReport() const {
    return parking_lot_.getDetailedReport();
}

void SimulationRunner::logEvent(int event_count,
                               const SimulationEvent& event,
                               const AllocationResult& result) {
    auto stats = parking_lot_.getStatistics();
    std::string type = (event.vehicle_type == VehicleType::MOTORCYCLE ? "MOTORCYCLE" : "CAR");
    
    if (result.success) {
        std::cout << "[" << event_count << "] ARRIVAL V" << event.vehicle_id 
                 << " (" << type << ") => Floor " << result.floor 
                 << " Zone " << result.zone << " Space " << result.space_number 
                 << " [" << stats.current_parked_count << "/480]\n";
    } else {
        std::cout << "[" << event_count << "] ARRIVAL V" << event.vehicle_id 
                 << " (" << type << ") => REJECTED (" << result.error_message 
                 << ") [" << stats.current_parked_count << "/480]\n";
    }
}

