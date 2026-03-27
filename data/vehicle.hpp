#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>
#include <chrono>
#include "../enums.hpp"

class Vehicle {
public:
    // Default constructor (required for unordered_map)
    Vehicle();

    // Constructor for new vehicle
    Vehicle(const std::string& vehicle_id, VehicleType type, 
            int expected_duration_minutes, std::chrono::system_clock::time_point arrival_time);

    // Accessors
    std::string getVehicleId() const;
    VehicleType getVehicleType() const;
    int getExpectedDurationMinutes() const;
    std::chrono::system_clock::time_point getArrivalTime() const;
    std::chrono::system_clock::time_point getDepartureTime() const;

    // Allocated space info
    int getAllocatedFloor() const;
    char getAllocatedZone() const;
    int getAllocatedSpaceNumber() const;
    int getDistanceToExit() const;

    // Mutators
    void assignSpace(int floor, char zone, int space_number, int distance_to_exit);
    void completeParking(std::chrono::system_clock::time_point departure_time);

    // Duration calculation
    int getActualDurationMinutes() const;
    int getDurationDifference() const;  // actual - expected

    // Formatting
    std::string toString() const;

private:
    std::string vehicle_id_;
    VehicleType vehicle_type_;
    int expected_duration_minutes_;
    std::chrono::system_clock::time_point arrival_time_;
    std::chrono::system_clock::time_point departure_time_;

    int allocated_floor_;
    char allocated_zone_;
    int allocated_space_number_;
    int distance_to_exit_;

    bool is_departed_;
};

#endif // VEHICLE_HPP
