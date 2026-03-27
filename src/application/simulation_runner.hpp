#ifndef SIMULATION_RUNNER_HPP
#define SIMULATION_RUNNER_HPP

#include <vector>
#include <string>
#include "../domain/parking_lot.hpp"
#include "../integration/dataset_loader.hpp"
#include "../application/config.hpp"

class SimulationRunner {
public:
    SimulationRunner();

    // Initialization
    bool initializeSimulation(const ApplicationConfig& config);

    // Execution
    bool runSimulation();

    // Results
    SimulationStatistics getStatistics() const;
    std::string getDetailedReport() const;

private:
    ParkingLot parking_lot_;
    DatasetLoader loader_;
    std::vector<SimulationEvent> events_;
    ApplicationConfig config_;
    bool initialized_;

    void logEvent(int event_count, const SimulationEvent& event, const AllocationResult& result);
};

#endif // SIMULATION_RUNNER_HPP
