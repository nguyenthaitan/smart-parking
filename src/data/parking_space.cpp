#include "parking_space.hpp"
#include <sstream>
#include <iomanip>

ParkingSpace::ParkingSpace(int floor, char zone, int space_number, VehicleType type,
                           int distance_base, int space_offset)
    : floor_(floor), zone_(zone), space_number_(space_number),
      vehicle_type_(type), is_occupied_(false), occupied_by_(""),
      occupation_start_time_(std::chrono::system_clock::now()) {
    // Distance formula: floor * 10 + zone_offset * 2 + space_offset
    // Zone A=0, B=1, C=2, ..., J=9
    // Note: distance_base is provided but we calculate it locally for consistency
    (void)distance_base;  // Mark as intentionally unused
    int zone_offset = zone - 'A';
    distance_to_exit_ = (floor * 10) + (zone_offset * 2) + space_offset;
}

int ParkingSpace::getFloor() const {
    return floor_;
}

char ParkingSpace::getZone() const {
    return zone_;
}

int ParkingSpace::getSpaceNumber() const {
    return space_number_;
}

VehicleType ParkingSpace::getVehicleType() const {
    return vehicle_type_;
}

int ParkingSpace::getDistanceToExit() const {
    return distance_to_exit_;
}

bool ParkingSpace::isOccupied() const {
    return is_occupied_;
}

std::string ParkingSpace::getOccupiedBy() const {
    return occupied_by_;
}

std::chrono::system_clock::time_point ParkingSpace::getOccupationStartTime() const {
    return occupation_start_time_;
}

void ParkingSpace::occupy(const std::string& vehicle_id, 
                         std::chrono::system_clock::time_point start_time) {
    is_occupied_ = true;
    occupied_by_ = vehicle_id;
    occupation_start_time_ = start_time;
}

void ParkingSpace::release() {
    is_occupied_ = false;
    occupied_by_ = "";
    occupation_start_time_ = std::chrono::system_clock::now();
}

bool ParkingSpace::operator>(const ParkingSpace& other) const {
    // For min-heap: closer distance is "smaller"
    return distance_to_exit_ > other.distance_to_exit_;
}

bool ParkingSpace::operator<(const ParkingSpace& other) const {
    return distance_to_exit_ < other.distance_to_exit_;
}

bool ParkingSpace::operator==(const ParkingSpace& other) const {
    return floor_ == other.floor_ &&
           zone_ == other.zone_ &&
           space_number_ == other.space_number_;
}

bool ParkingSpace::canAccommodate(VehicleType type) const {
    return vehicle_type_ == type && !is_occupied_;
}

std::string ParkingSpace::toString() const {
    std::ostringstream oss;
    oss << "Floor " << floor_ << " Zone " << zone_ << " Space " << space_number_
        << " (Type: " << (vehicle_type_ == VehicleType::MOTORCYCLE ? "MOTORCYCLE" : "CAR")
        << ", Distance: " << distance_to_exit_ << "m, Status: "
        << (is_occupied_ ? "OCCUPIED by " + occupied_by_ : "AVAILABLE") << ")";
    return oss.str();
}
