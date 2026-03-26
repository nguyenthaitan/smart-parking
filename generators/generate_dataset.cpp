#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <cstdlib>

int main() {
    // ==================== CONFIG ====================
    const int NUM_FLOORS = 5;
    const int ZONES_PER_FLOOR = 8;
    const int SPACES_PER_ZONE = 12;
    const double INITIAL_OCCUPANCY_RATE = 0.5;
    const int NUM_VEHICLES = 100;
    const unsigned int RANDOM_SEED = 42;
    
    const std::string DATA_DIR = "../data";
    const std::string CONFIG_FILE = DATA_DIR + "/parking_lot_config.csv";
    
    // Create data directory if not exists
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
    
    std::mt19937 engine(RANDOM_SEED == 0 ? std::random_device{}() : RANDOM_SEED);
    std::uniform_int_distribution<> duration_dist(15, 300);
    
    config_file << "floor,zone,space_number,vehicle_type,distance_base,occupied_by,parked_duration_minutes\n";
    
    int space_counter = 0;
    int occupied_count = 0;
    int total_spaces = NUM_FLOORS * ZONES_PER_FLOOR * SPACES_PER_ZONE;
    int target_occupied = (int)(total_spaces * INITIAL_OCCUPANCY_RATE);
    
    for (int floor = 1; floor <= NUM_FLOORS; ++floor) {
        for (int zone = 0; zone < ZONES_PER_FLOOR; ++zone) {
            std::string zone_type = (zone % 2 == 0) ? "CAR" : "MOTORCYCLE";
            int distance_base = (floor * 10) + (zone * 2);
            
            for (int space = 1; space <= SPACES_PER_ZONE; ++space) {
                space_counter++;
                char zone_letter = 'A' + zone;
                
                bool is_occupied = (space_counter <= target_occupied);
                std::string occupied_by = "";
                int parked_duration = 0;
                
                if (is_occupied) {
                    occupied_count++;
                    int vehicle_num = ((occupied_count - 1) % NUM_VEHICLES) + 1;
                    occupied_by = "V" + std::string(3 - std::to_string(vehicle_num).length(), '0') + std::to_string(vehicle_num);
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
    
    // ==================== OUTPUT SUMMARY ====================
    std::cout << "\nDataset generated successfully!\n";
    std::cout << "File created: " << CONFIG_FILE << "\n";
    std::cout << "Total spaces: " << total_spaces << "\n";
    std::cout << "Occupied spaces: " << occupied_count << "\n";
    
    return 0;
}
