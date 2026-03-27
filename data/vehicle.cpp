#include "vehicle.hpp"
#include <sstream>

Vehicle::Vehicle()
    : vehicle_id_(""), vehicle_type_(VehicleType::CAR),
      expected_duration_minutes_(0), arrival_time_(std::chrono::system_clock::now()),
      departure_time_(std::chrono::system_clock::now()),
      allocated_floor_(0), allocated_zone_(' '), allocated_space_number_(0),
      distance_to_exit_(0), is_departed_(false) {
}

Vehicle::Vehicle(const std::string& vehicle_id, VehicleType type,
                 int expected_duration_minutes, std::chrono::system_clock::time_point arrival_time)
    : vehicle_id_(vehicle_id), vehicle_type_(type),
      expected_duration_minutes_(expected_duration_minutes),
      arrival_time_(arrival_time), departure_time_(std::chrono::system_clock::now()),
      allocated_floor_(0), allocated_zone_(' '), allocated_space_number_(0),
      distance_to_exit_(0), is_departed_(false) {
}

std::string Vehicle::getVehicleId() const {
    return vehicle_id_;
}

VehicleType Vehicle::getVehicleType() const {
    return vehicle_type_;
}

int Vehicle::getExpectedDurationMinutes() const {
    return expected_duration_minutes_;
}

std::chrono::system_clock::time_point Vehicle::getArrivalTime() const {
    return arrival_time_;
}

std::chrono::system_clock::time_point Vehicle::getDepartureTime() const {
    return departure_time_;
}

int Vehicle::getAllocatedFloor() const {
    return allocated_floor_;
}

char Vehicle::getAllocatedZone() const {
    return allocated_zone_;
}

int Vehicle::getAllocatedSpaceNumber() const {
    return allocated_space_number_;
}

int Vehicle::getDistanceToExit() const {
    return distance_to_exit_;
}

void Vehicle::assignSpace(int floor, char zone, int space_number, int distance_to_exit) {
    allocated_floor_ = floor;
    allocated_zone_ = zone;
    allocated_space_number_ = space_number;
    distance_to_exit_ = distance_to_exit;
}

void Vehicle::completeParking(std::chrono::system_clock::time_point departure_time) {
    departure_time_ = departure_time;
    is_departed_ = true;
}

int Vehicle::getActualDurationMinutes() const {
    if (!is_departed_) return 0;
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(
        departure_time_ - arrival_time_);
    return static_cast<int>(duration.count());
}

int Vehicle::getDurationDifference() const {
    return getActualDurationMinutes() - expected_duration_minutes_;
}

std::string Vehicle::toString() const {
    std::ostringstream oss;
    oss << "Vehicle " << vehicle_id_
        << " (Type: " << (vehicle_type_ == VehicleType::MOTORCYCLE ? "MOTORCYCLE" : "CAR")
        << ", Floor " << allocated_floor_ << " Zone " << allocated_zone_ 
        << " Space " << allocated_space_number_;
    if (is_departed_) {
        oss << ", Duration: " << getActualDurationMinutes() << " min)";
    } else {
        oss << ", Status: PARKED)";
    }
    return oss.str();
}
