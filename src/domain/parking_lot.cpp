#include "parking_lot.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>

ParkingLot::ParkingLot()
    : total_arrivals_(0), total_departures_(0), peak_occupancy_(0) {
}

void ParkingLot::loadConfiguration(const std::vector<ParkingSpace>& spaces) {
    parking_manager_.initializeFromSpaces(spaces);
}

AllocationResult ParkingLot::arrivedVehicle(const std::string& vehicle_id, VehicleType type,
                                           int expected_duration_minutes,
                                           std::chrono::system_clock::time_point timestamp) {
    total_arrivals_++;

    // Create and allocate vehicle
    Vehicle vehicle(vehicle_id, type, expected_duration_minutes, timestamp);

    AllocationResult result = parking_manager_.findAndAllocateSpace(type, vehicle_id, timestamp);

    if (result.success) {
        vehicle.assignSpace(result.floor, result.zone, result.space_number,
                           result.distance_to_exit);
        vehicle_tracker_.registerVehicle(vehicle);

        // Update peak occupancy
        int current_parked = getCurrentParkedCount();
        peak_occupancy_ = std::max(peak_occupancy_, current_parked);
    }

    return result;
}

void ParkingLot::departedVehicle(const std::string& vehicle_id,
                                std::chrono::system_clock::time_point timestamp) {
    total_departures_++;

    Vehicle* vehicle = vehicle_tracker_.findVehicle(vehicle_id);
    if (vehicle) {
        vehicle->completeParking(timestamp);

        // Release space from parking manager
        parking_manager_.releaseSpace(vehicle->getAllocatedFloor(),
                                     vehicle->getAllocatedZone(),
                                     vehicle->getAllocatedSpaceNumber());

        // Move vehicle to departed list
        vehicle_tracker_.removeVehicle(vehicle_id);
    }
}

int ParkingLot::getTotalSpaces() const {
    return parking_manager_.getTotalSpaces();
}

int ParkingLot::getAvailableSpaces() const {
    return parking_manager_.getAvailableSpaces();
}

int ParkingLot::getCurrentParkedCount() const {
    return vehicle_tracker_.getTotalParkedVehicles();
}

double ParkingLot::getCurrentOccupancyRate() const {
    return parking_manager_.getOccupancyRate();
}

SimulationStatistics ParkingLot::getStatistics() const {
    SimulationStatistics stats;
    stats.total_arrivals = total_arrivals_;
    stats.total_departures = total_departures_;
    stats.total_allocations = parking_manager_.getTotalAllocations();
    stats.total_rejections = parking_manager_.getTotalRejections();
    stats.motorcycle_rejections = parking_manager_.getRejectionCountForType(VehicleType::MOTORCYCLE);
    stats.car_rejections = parking_manager_.getRejectionCountForType(VehicleType::CAR);
    stats.current_parked_count = getCurrentParkedCount();
    stats.current_occupancy_rate = getCurrentOccupancyRate();
    stats.average_parking_duration = vehicle_tracker_.getAverageParkingDuration();
    stats.peak_occupancy = peak_occupancy_;
    return stats;
}

std::string ParkingLot::getStatusReport() const {
    std::ostringstream oss;
    oss << "=== Parking Lot Status ===" << std::endl;
    oss << "Total Spaces: " << getTotalSpaces() << std::endl;
    oss << "Available Spaces: " << getAvailableSpaces() << std::endl;
    oss << "Currently Parked: " << getCurrentParkedCount() << std::endl;
    oss << "Occupancy Rate: " << std::fixed << std::setprecision(2)
        << getCurrentOccupancyRate() << "%" << std::endl;
    oss << "Peak Occupancy: " << peak_occupancy_ << std::endl;
    return oss.str();
}

std::string ParkingLot::getDetailedReport() const {
    std::ostringstream oss;
    auto stats = getStatistics();

    oss << "=== Detailed Simulation Report ===" << std::endl;
    oss << "Total Arrivals: " << stats.total_arrivals << std::endl;
    oss << "Total Departures: " << stats.total_departures << std::endl;
    oss << "Total Allocations: " << stats.total_allocations << std::endl;
    oss << "Total Rejections: " << stats.total_rejections << std::endl;
    oss << "  - Motorcycle Rejections: " << stats.motorcycle_rejections << std::endl;
    oss << "  - Car Rejections: " << stats.car_rejections << std::endl;
    oss << "Current Parked Vehicles: " << stats.current_parked_count << std::endl;
    oss << "Current Occupancy Rate: " << std::fixed << std::setprecision(2)
        << stats.current_occupancy_rate << "%" << std::endl;
    oss << "Peak Occupancy: " << stats.peak_occupancy << " vehicles" << std::endl;
    oss << "Average Parking Duration: " << std::fixed << std::setprecision(2)
        << stats.average_parking_duration << " minutes" << std::endl;
    oss << "Rejection Rate: " << std::fixed << std::setprecision(2)
        << (stats.total_arrivals > 0 ? (100.0 * stats.total_rejections / stats.total_arrivals) : 0.0)
        << "%" << std::endl;

    return oss.str();
}

int ParkingLot::getTotalAllocations() const {
    return parking_manager_.getTotalAllocations();
}

int ParkingLot::getTotalRejections() const {
    return parking_manager_.getTotalRejections();
}
