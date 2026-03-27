#include "dataset_loader.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

DatasetLoader::DatasetLoader() : last_error_("") {
}

bool DatasetLoader::loadParkingLotConfig(const std::string& filename,
                                        std::vector<ParkingSpace>& spaces) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        last_error_ = "Failed to open file: " + filename;
        return false;
    }

    std::string line;
    int line_number = 0;

    // Read and validate headers
    if (!std::getline(file, line)) {
        last_error_ = "File is empty";
        return false;
    }
    line_number++;

    auto headers = splitCsvLine(line);
    if (!validateConfigHeaders(headers)) {
        last_error_ = "Invalid CSV headers for parking config";
        return false;
    }

    // Parse data rows
    while (std::getline(file, line)) {
        line_number++;
        if (line.empty()) continue;

        auto fields = splitCsvLine(line);
        if (fields.size() < 5) {
            last_error_ = "Insufficient fields on line " + std::to_string(line_number);
            return false;
        }

        try {
            int floor = std::stoi(fields[0]);
            char zone = fields[1][0];
            int space_number = std::stoi(fields[2]);
            VehicleType type = (fields[3] == "MOTORCYCLE") ? VehicleType::MOTORCYCLE : VehicleType::CAR;
            int distance_base = std::stoi(fields[4]);
            int space_offset = 0;  // default offset
            
            // Create the parking space
            ParkingSpace space(floor, zone, space_number, type, distance_base, space_offset);
            
            // Check if space is initially occupied
            if (fields.size() >= 7 && !fields[5].empty()) {
                // Fields: floor, zone, space_number, vehicle_type, distance_base, occupied_by, parked_duration_minutes
                std::string occupied_by = fields[5];
                // Mark space as occupied
                space.occupy(occupied_by, std::chrono::system_clock::now());
            }
            
            spaces.emplace_back(space);
        } catch (const std::exception& e) {
            last_error_ = "Parse error on line " + std::to_string(line_number) + ": " + e.what();
            return false;
        }
    }

    file.close();
    return true;
}

bool DatasetLoader::loadSimulationEvents(const std::string& filename,
                                        std::vector<SimulationEvent>& events) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        last_error_ = "Failed to open file: " + filename;
        return false;
    }

    std::string line;
    int line_number = 0;

    // Read and validate headers
    if (!std::getline(file, line)) {
        last_error_ = "File is empty";
        return false;
    }
    line_number++;

    auto headers = splitCsvLine(line);
    if (!validateEventHeaders(headers)) {
        last_error_ = "Invalid CSV headers for simulation events";
        return false;
    }

    // Parse data rows
    while (std::getline(file, line)) {
        line_number++;
        if (line.empty()) continue;

        auto fields = splitCsvLine(line);
        if (fields.size() < 4) {
            last_error_ = "Insufficient fields on line " + std::to_string(line_number);
            return false;
        }

        try {
            SimulationEvent event;
            // Fields: event_sequence, vehicle_id, event_type, vehicle_type
            event.timestamp_seconds = std::stoi(fields[0]);  // sequence order
            event.vehicle_id = fields[1];
            event.event_type = (fields[2] == "ARRIVAL") ? EventType::ARRIVAL : EventType::DEPARTURE;
            event.vehicle_type = (fields[3] == "MOTORCYCLE") ? VehicleType::MOTORCYCLE : VehicleType::CAR;
            event.duration_minutes = 0;  // Not used

            events.push_back(event);
        } catch (const std::exception& e) {
            last_error_ = "Parse error on line " + std::to_string(line_number) + ": " + e.what();
            return false;
        }
    }

    file.close();
    return true;
}

std::string DatasetLoader::getLastError() const {
    return last_error_;
}

std::vector<std::string> DatasetLoader::splitCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;

    while (std::getline(ss, field, ',')) {
        // Trim whitespace
        field.erase(0, field.find_first_not_of(" \t\r\n"));
        field.erase(field.find_last_not_of(" \t\r\n") + 1);
        fields.push_back(field);
    }

    return fields;
}

bool DatasetLoader::validateConfigHeaders(const std::vector<std::string>& headers) {
    if (headers.size() < 5) return false;
    // New format: floor,zone,space_number,vehicle_type,distance_base[,occupied_by,parked_duration_minutes]
    return headers[0] == "floor" && headers[1] == "zone" && headers[2] == "space_number" &&
           headers[3] == "vehicle_type" && headers[4] == "distance_base";
}

bool DatasetLoader::validateEventHeaders(const std::vector<std::string>& headers) {
    if (headers.size() < 4) return false;
    // New format: event_sequence,vehicle_id,event_type,vehicle_type
    return headers[0] == "event_sequence" && headers[1] == "vehicle_id" && headers[2] == "event_type" &&
           headers[3] == "vehicle_type";
}
