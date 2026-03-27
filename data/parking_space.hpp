#ifndef PARKING_SPACE_HPP
#define PARKING_SPACE_HPP

#include <string>
#include <chrono>
#include "../enums.hpp"

class ParkingSpace {
public:
    // Constructor
    ParkingSpace(int floor, char zone, int space_number, VehicleType type, 
                 int distance_base, int space_offset);

    // Accessors
    int getFloor() const;
    char getZone() const;
    int getSpaceNumber() const;
    VehicleType getVehicleType() const;
    int getDistanceToExit() const;
    bool isOccupied() const;
    std::string getOccupiedBy() const;
    std::chrono::system_clock::time_point getOccupationStartTime() const;

    // Mutators
    void occupy(const std::string& vehicle_id, std::chrono::system_clock::time_point start_time);
    void release();

    // Comparison operator for min-heap (distance-based)
    bool operator>(const ParkingSpace& other) const;
    bool operator<(const ParkingSpace& other) const;
    bool operator==(const ParkingSpace& other) const;

    // Type checking
    bool canAccommodate(VehicleType type) const;

    // Formatting
    std::string toString() const;

private:
    int floor_;
    char zone_;
    int space_number_;
    VehicleType vehicle_type_;
    int distance_to_exit_;
    bool is_occupied_;
    std::string occupied_by_;
    std::chrono::system_clock::time_point occupation_start_time_;
};

#endif // PARKING_SPACE_HPP
