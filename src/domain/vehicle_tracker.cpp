#include "vehicle_tracker.hpp"

VehicleTracker::VehicleTracker() {
}

void VehicleTracker::registerVehicle(const Vehicle& vehicle) {
    parked_vehicles_[vehicle.getVehicleId()] = vehicle;
}

Vehicle* VehicleTracker::findVehicle(const std::string& vehicle_id) {
    auto it = parked_vehicles_.find(vehicle_id);
    if (it != parked_vehicles_.end()) {
        return &it->second;
    }
    return nullptr;
}

void VehicleTracker::removeVehicle(const std::string& vehicle_id) {
    auto it = parked_vehicles_.find(vehicle_id);
    if (it != parked_vehicles_.end()) {
        departed_vehicles_.push_back(it->second);
        parked_vehicles_.erase(it);
    }
}

bool VehicleTracker::isVehicleParked(const std::string& vehicle_id) const {
    return parked_vehicles_.find(vehicle_id) != parked_vehicles_.end();
}

int VehicleTracker::getTotalParkedVehicles() const {
    return parked_vehicles_.size();
}

int VehicleTracker::getParkedVehiclesOfType(VehicleType type) const {
    int count = 0;
    for (const auto& pair : parked_vehicles_) {
        if (pair.second.getVehicleType() == type) {
            count++;
        }
    }
    return count;
}

int VehicleTracker::getTotalDepartedVehicles() const {
    return departed_vehicles_.size();
}

double VehicleTracker::getAverageParkingDuration() const {
    if (departed_vehicles_.empty()) return 0.0;
    
    int total_duration = 0;
    for (const auto& vehicle : departed_vehicles_) {
        total_duration += vehicle.getActualDurationMinutes();
    }
    return static_cast<double>(total_duration) / departed_vehicles_.size();
}

int VehicleTracker::getTotalVehiclesProcessed() const {
    return parked_vehicles_.size() + departed_vehicles_.size();
}

std::vector<Vehicle> VehicleTracker::getAllDepartedVehicles() const {
    return departed_vehicles_;
}
