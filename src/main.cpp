#include "application/simulation_runner.hpp"
#include "application/config.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS]\n"
             << "Options:\n"
             << "  --help                Show this help message\n"
             << "  --config <file>       Parking lot configuration file\n"
             << "  --events <file>       Simulation events file\n"
             << "  --output <file>       Output report file\n"
             << "  --verbose             Print detailed logs\n";
}

int main(int argc, char* argv[]) {
    ApplicationConfig config{
        "data/parking_lot_config.csv",
        "data/simulation_events.csv",
        "data/simulation_report.txt",
        false
    };

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--config" && i + 1 < argc) {
            config.parking_config_file = argv[++i];
        } else if (arg == "--events" && i + 1 < argc) {
            config.simulation_events_file = argv[++i];
        } else if (arg == "--output" && i + 1 < argc) {
            config.output_file = argv[++i];
        } else if (arg == "--verbose") {
            config.verbose = true;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }

    // Initialize and run simulation
    SimulationRunner runner;

    std::cout << "Initializing simulation...\n";
    if (!runner.initializeSimulation(config)) {
        std::cerr << "Failed to initialize simulation" << std::endl;
        return 1;
    }

    std::cout << "Running simulation...\n";
    if (!runner.runSimulation()) {
        std::cerr << "Failed to run simulation" << std::endl;
        return 1;
    }

    // Display results
    std::cout << runner.getDetailedReport() << std::endl;

    // Save results to file if output specified
    if (!config.output_file.empty()) {
        std::ofstream output(config.output_file);
        output << runner.getDetailedReport();
        output.close();
        std::cout << "\nReport saved to: " << config.output_file << std::endl;
    }

    return 0;
}

