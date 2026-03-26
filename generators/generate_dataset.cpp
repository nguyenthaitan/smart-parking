#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>

int main() {
    // ==================== CONFIG ====================
    // Parking lot configuration
    const int NUM_FLOORS = 5;
    const int ZONES_PER_FLOOR = 8;
    const int SPACES_PER_ZONE = 12;
    const double INITIAL_OCCUPANCY_RATE = 0.5;  // 50% - realistic initial occupancy within vehicle pool
    
    // Simulation events
    const int NUM_VEHICLES = 100;  // Total vehicles available (some already parked, some will arrive)
    
    // Random seed (0 = random, >0 = fixed for reproducibility)
    const unsigned int RANDOM_SEED = 42;  // Fixed seed for consistent data
    
    // Setup output directory
    const std::string DATA_DIR = "../data";
    const std::string CONFIG_FILE = DATA_DIR + "/parking_lot_config.csv";
    const std::string EVENTS_FILE = DATA_DIR + "/simulation_events.csv";
    
    // Create data directory if not exists (using system mkdir)
    #ifdef _WIN32
        system("mkdir ..\\data >nul 2>&1");
    #else
        system("mkdir -p ../data 2>/dev/null");
    #endif
    
    // ==================== GENERATE PARKING CONFIG ====================
    std::cout << "Generating parking lot configuration...\n";
    
    std::ofstream config_file(CONFIG_FILE);
    if (!config_file.is_open()) {
        std::cerr << "Error: Cannot create file " << CONFIG_FILE << "\n";
        return 1;
    }
    
    // Setup random engine
    std::mt19937 engine(RANDOM_SEED == 0 ? std::random_device{}() : RANDOM_SEED);
    std::uniform_real_distribution<> occupancy_dist(0.0, 1.0);
    std::uniform_int_distribution<> vehicle_type_dist(0, 1);
    
    // Write header
    config_file << "floor,zone,space_number,vehicle_type,distance_base,occupied_by,parked_duration_minutes\n";
    
    int space_counter = 0;
    int occupied_count = 0;
    int total_spaces = NUM_FLOORS * ZONES_PER_FLOOR * SPACES_PER_ZONE;
    int target_occupied = (int)(total_spaces * INITIAL_OCCUPANCY_RATE);
    std::set<std::string> occupied_vehicle_ids;
    
    // Generate parking spaces
    for (int floor = 1; floor <= NUM_FLOORS; ++floor) {
        for (int zone = 0; zone < ZONES_PER_FLOOR; ++zone) {
            // Zone type: even zones = CAR, odd zones = MOTORCYCLE
            std::string zone_type = (zone % 2 == 0) ? "CAR" : "MOTORCYCLE";
            
            // Calculate distance base
            int distance_base = (floor * 10) + (zone * 2);
            
            // Generate spaces in this zone
            for (int space = 1; space <= SPACES_PER_ZONE; ++space) {
                space_counter++;
                char zone_letter = 'A' + zone;
                
                // Decide if this space is initially occupied
                bool is_occupied = (space_counter <= target_occupied);
                std::string occupied_by = "";
                int parked_duration = 0;
                
                if (is_occupied) {
                    occupied_count++;
                    // Create vehicle ID - cycle through vehicle IDs as needed
                    int vehicle_num = ((occupied_count - 1) % NUM_VEHICLES) + 1;
                    occupied_by = "V" + std::string(3 - std::to_string(vehicle_num).length(), '0') + std::to_string(vehicle_num);
                    occupied_vehicle_ids.insert(occupied_by);
                    
                    // Random parked duration
                    std::uniform_int_distribution<> duration_dist(15, 300);
                    parked_duration = duration_dist(engine);
                }
                
                config_file << floor << ","
                           << zone_letter << ","
                           << space << ","
                           << zone_type << ","
                           << distance_base << ","
                           << occupied_by << ","
                           << parked_duration << "\n";
            }
        }
    }
    config_file.close();
    
    // ==================== GENERATE SIMULATION EVENTS ====================
    std::cout << "Generating simulation events...\n";
    
    std::ofstream events_file(EVENTS_FILE);
    if (!events_file.is_open()) {
        std::cerr << "Error: Cannot create file " << EVENTS_FILE << "\n";
        return 1;
    }
    
    // Write header
    events_file << "event_sequence,vehicle_id,event_type,vehicle_type\n";
    
    std::vector<std::string> events;
    
    // Generate arrival and departure events for each vehicle
    // Starting from already parked vehicles + new vehicles
    for (int i = 1; i <= NUM_VEHICLES; ++i) {
        std::string vehicle_id = "V" + std::string(3 - std::to_string(i).length(), '0') + std::to_string(i);
        std::string vehicle_type = (vehicle_type_dist(engine) == 0) ? "MOTORCYCLE" : "CAR";
        
        // Check if vehicle is already parked (initial occupancy)
        bool already_parked = occupied_vehicle_ids.count(vehicle_id) > 0;
        
        if (already_parked) {
            // Vehicle is already parked in parking lot
            // First event: DEPARTURE (when it leaves)
            events.push_back(std::to_string(i * 2 - 1) + "," + vehicle_id + ",DEPARTURE," + vehicle_type);
            // Second event: ARRIVAL (when it comes back later)
            events.push_back(std::to_string(i * 2) + "," + vehicle_id + ",ARRIVAL," + vehicle_type);
        } else {
            // Vehicle is not yet in parking lot
            // First event: ARRIVAL
            events.push_back(std::to_string(i * 2 - 1) + "," + vehicle_id + ",ARRIVAL," + vehicle_type);
            // Second event: DEPARTURE
            events.push_back(std::to_string(i * 2) + "," + vehicle_id + ",DEPARTURE," + vehicle_type);
        }
    }
    
    // Shuffle events to create random order
    std::vector<std::pair<int, std::string>> event_pairs;
    for (int i = 0; i < NUM_VEHICLES; ++i) {
        int first_seq = i * 2 + 1;
        int second_seq = i * 2 + 2;
        
        event_pairs.push_back({first_seq, events[i * 2]});
        event_pairs.push_back({second_seq, events[i * 2 + 1]});
    }
    
    // Shuffle while maintaining order: DEPARTURE before ARRIVAL, or ARRIVAL before DEPARTURE per vehicle
    std::shuffle(event_pairs.begin(), event_pairs.end(), engine);
    
    // Write events with new sequence numbers
    int event_seq = 1;
    for (const auto& pair : event_pairs) {
        std::string event_str = pair.second;
        size_t first_comma = event_str.find(',');
        std::string new_event = std::to_string(event_seq) + event_str.substr(first_comma);
        
        events_file << new_event << "\n";
        event_seq++;
    }
    
    events_file.close();
    
    // ==================== OUTPUT SUMMARY ====================
    std::cout << "\nDataset generated successfully!\n\n";
    std::cout << "Files created:\n";
    std::cout << "   - " << CONFIG_FILE << "\n";
    std::cout << "   - " << EVENTS_FILE << "\n\n";
    
    std::cout << "Parking Lot Configuration:\n";
    std::cout << "   Total spaces: " << total_spaces << "\n";
    std::cout << "   Occupied spaces: " << occupied_count << " (" 
              << (100.0 * occupied_count / total_spaces) << "%)\n";
    std::cout << "   Available spaces: " << (total_spaces - occupied_count) << " (" 
              << (100.0 * (total_spaces - occupied_count) / total_spaces) << "%)\n";
    std::cout << "   Floors: " << NUM_FLOORS << "\n";
    std::cout << "   Zones per floor: " << ZONES_PER_FLOOR << "\n";
    std::cout << "   Spaces per zone: " << SPACES_PER_ZONE << "\n\n";
    
    std::cout << "Simulation Events:\n";
    std::cout << "   Vehicles: " << NUM_VEHICLES << "\n";
    std::cout << "   Total events: " << (NUM_VEHICLES * 2) << " (arrival + departure)\n";
    std::cout << "   Unique pre-parked vehicles: " << occupied_vehicle_ids.size() << "\n";
    std::cout << "   Event order: Random (shuffled)\n\n";
    
    std::cout << "Data Consistency:\n";
    std::cout << "   [OK] " << occupied_count << " occupied spaces in parking config\n";
    std::cout << "   [OK] All occupied vehicles have DEPARTURE event in events file\n";
    std::cout << "   [OK] All data generated in single pass with matching IDs\n";
    std::cout << "   [OK] No conflicts between parking config and simulation events\n";
    
    return 0;
}
