#ifndef PARKING_MANAGER_HPP
#define PARKING_MANAGER_HPP

#include <vector>
#include <queue>
#include <memory>
#include "../data/parking_space.hpp"
#include "../enums.hpp"

struct AllocationResult {
    bool success;
    int floor;
    char zone;
    int space_number;
    int distance_to_exit;
    std::string error_message;
};

class ParkingManager {
public:
    ParkingManager();

    // Initialization
    void initializeFromSpaces(const std::vector<ParkingSpace>& all_spaces);

    // Space allocation and management
    AllocationResult findAndAllocateSpace(VehicleType type, const std::string& vehicle_id,
                                         std::chrono::system_clock::time_point timestamp);
    void releaseSpace(int floor, char zone, int space_number);

    // Query methods
    int getTotalSpaces() const;
    int getAvailableSpaces() const;
    int getOccupiedSpaces() const;
    double getOccupancyRate() const;
    int getAvailableSpacesForType(VehicleType type) const;
    int getOccupancyForFloor(int floor) const;
    int getTotalSpacesForFloor(int floor) const;

    // Statistics
    int getTotalAllocations() const;
    int getTotalRejections() const;
    int getRejectionCountForType(VehicleType type) const;

private:
    struct ParkingSpaceWithId {
        ParkingSpace space;
        int space_index;
        
        bool operator>(const ParkingSpaceWithId& other) const {
            return space > other.space;
        }
    };

    std::vector<ParkingSpace> all_spaces_;
    std::priority_queue<ParkingSpaceWithId, std::vector<ParkingSpaceWithId>,
                       std::greater<ParkingSpaceWithId>> available_spaces_;

    int total_allocations_;
    int total_rejections_;
    int motorcycle_rejections_;
    int car_rejections_;

    ParkingSpace* findSpaceByLocation(int floor, char zone, int space_number);
};

#endif // PARKING_MANAGER_HPP
