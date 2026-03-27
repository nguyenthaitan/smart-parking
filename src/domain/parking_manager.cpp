#include "parking_manager.hpp"

ParkingManager::ParkingManager()
    : total_allocations_(0), total_rejections_(0),
      motorcycle_rejections_(0), car_rejections_(0) {
}

void ParkingManager::initializeFromSpaces(const std::vector<ParkingSpace>& all_spaces) {
    all_spaces_ = all_spaces;

    // Rebuild priority queue with all available spaces
    while (!available_spaces_.empty()) {
        available_spaces_.pop();
    }

    for (size_t i = 0; i < all_spaces_.size(); ++i) {
        if (!all_spaces_[i].isOccupied()) {
            available_spaces_.push({all_spaces_[i], static_cast<int>(i)});
        }
    }
}

AllocationResult ParkingManager::findAndAllocateSpace(VehicleType type, 
                                                      const std::string& vehicle_id,
                                                      std::chrono::system_clock::time_point timestamp) {
    AllocationResult result{false, 0, ' ', 0, 0, ""};

    // Find nearest available space of the correct type
    while (!available_spaces_.empty()) {
        auto top = available_spaces_.top();
        available_spaces_.pop();

        ParkingSpace* space = &all_spaces_[top.space_index];

        // Check if space is still available and matches type
        if (!space->isOccupied() && space->canAccommodate(type)) {
            space->occupy(vehicle_id, timestamp);
            result.success = true;
            result.floor = space->getFloor();
            result.zone = space->getZone();
            result.space_number = space->getSpaceNumber();
            result.distance_to_exit = space->getDistanceToExit();
            total_allocations_++;
            return result;
        }
    }

    // No space found
    std::string vehicle_type_str = type == VehicleType::MOTORCYCLE ? "MOTORCYCLE" : "CAR";
    result.error_message = "No available space for " + vehicle_type_str;
    total_rejections_++;
    if (type == VehicleType::MOTORCYCLE) {
        motorcycle_rejections_++;
    } else {
        car_rejections_++;
    }

    return result;
}

void ParkingManager::releaseSpace(int floor, char zone, int space_number) {
    ParkingSpace* space = findSpaceByLocation(floor, zone, space_number);
    if (space) {
        space->release();
        // Find the index of this space
        for (size_t i = 0; i < all_spaces_.size(); ++i) {
            if (all_spaces_[i] == *space) {
                available_spaces_.push({*space, static_cast<int>(i)});
                break;
            }
        }
    }
}

int ParkingManager::getTotalSpaces() const {
    return all_spaces_.size();
}

int ParkingManager::getAvailableSpaces() const {
    int count = 0;
    for (const auto& space : all_spaces_) {
        if (!space.isOccupied()) {
            count++;
        }
    }
    return count;
}

int ParkingManager::getOccupiedSpaces() const {
    return getTotalSpaces() - getAvailableSpaces();
}

double ParkingManager::getOccupancyRate() const {
    if (getTotalSpaces() == 0) return 0.0;
    return (static_cast<double>(getOccupiedSpaces()) / getTotalSpaces()) * 100.0;
}

int ParkingManager::getAvailableSpacesForType(VehicleType type) const {
    int count = 0;
    for (const auto& space : all_spaces_) {
        if (space.getVehicleType() == type && !space.isOccupied()) {
            count++;
        }
    }
    return count;
}

int ParkingManager::getOccupancyForFloor(int floor) const {
    int count = 0;
    for (const auto& space : all_spaces_) {
        if (space.getFloor() == floor && space.isOccupied()) {
            count++;
        }
    }
    return count;
}

int ParkingManager::getTotalSpacesForFloor(int floor) const {
    int count = 0;
    for (const auto& space : all_spaces_) {
        if (space.getFloor() == floor) {
            count++;
        }
    }
    return count;
}

int ParkingManager::getTotalAllocations() const {
    return total_allocations_;
}

int ParkingManager::getTotalRejections() const {
    return total_rejections_;
}

int ParkingManager::getRejectionCountForType(VehicleType type) const {
    return type == VehicleType::MOTORCYCLE ? motorcycle_rejections_ : car_rejections_;
}

ParkingSpace* ParkingManager::findSpaceByLocation(int floor, char zone, int space_number) {
    for (auto& space : all_spaces_) {
        if (space.getFloor() == floor && space.getZone() == zone &&
            space.getSpaceNumber() == space_number) {
            return &space;
        }
    }
    return nullptr;
}
