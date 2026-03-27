#ifndef PARKING_LOT_HPP
#define PARKING_LOT_HPP

#include <vector>
#include <string>
#include "parking_manager.hpp"
#include "vehicle_tracker.hpp"
#include "../application/config.hpp"

class ParkingLot {
public:
    ParkingLot();

    // Configuration
    void loadConfiguration(const std::vector<ParkingSpace>& spaces);

    // Vehicle operations
    AllocationResult arrivedVehicle(const std::string& vehicle_id, VehicleType type,
                                    int expected_duration_minutes,
                                    std::chrono::system_clock::time_point timestamp);
    void departedVehicle(const std::string& vehicle_id,
                        std::chrono::system_clock::time_point timestamp);

    // Statistics and reporting
    int getTotalSpaces() const;
    int getAvailableSpaces() const;
    int getCurrentParkedCount() const;
    double getCurrentOccupancyRate() const;
    
    SimulationStatistics getStatistics() const;
    std::string getStatusReport() const;
    std::string getDetailedReport() const;

    int getTotalAllocations() const;
    int getTotalRejections() const;

private:
    ParkingManager parking_manager_;
    VehicleTracker vehicle_tracker_;

    int total_arrivals_;
    int total_departures_;
    int peak_occupancy_;
};

#endif // PARKING_LOT_HPP
