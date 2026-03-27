#ifndef VEHICLE_TRACKER_HPP
#define VEHICLE_TRACKER_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include "../data/vehicle.hpp"

class VehicleTracker {
public:
    VehicleTracker();

    // Vehicle registration and removal
    void registerVehicle(const Vehicle& vehicle);
    Vehicle* findVehicle(const std::string& vehicle_id);
    void removeVehicle(const std::string& vehicle_id);

    // Query methods
    bool isVehicleParked(const std::string& vehicle_id) const;
    int getTotalParkedVehicles() const;
    int getParkedVehiclesOfType(VehicleType type) const;
    int getTotalDepartedVehicles() const;

    // Statistics
    double getAverageParkingDuration() const;
    int getTotalVehiclesProcessed() const;
    std::vector<Vehicle> getAllDepartedVehicles() const;

private:
    std::unordered_map<std::string, Vehicle> parked_vehicles_;
    std::vector<Vehicle> departed_vehicles_;
};

#endif // VEHICLE_TRACKER_HPP
