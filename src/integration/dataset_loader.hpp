#ifndef DATASET_LOADER_HPP
#define DATASET_LOADER_HPP

#include <vector>
#include <string>
#include "../data/parking_space.hpp"
#include "../application/config.hpp"

class DatasetLoader {
public:
    DatasetLoader();

    // Loading methods
    bool loadParkingLotConfig(const std::string& filename,
                             std::vector<ParkingSpace>& spaces);
    bool loadSimulationEvents(const std::string& filename,
                             std::vector<SimulationEvent>& events);

    // Error reporting
    std::string getLastError() const;

private:
    std::string last_error_;

    // CSV parsing helpers
    std::vector<std::string> splitCsvLine(const std::string& line);
    bool validateConfigHeaders(const std::vector<std::string>& headers);
    bool validateEventHeaders(const std::vector<std::string>& headers);
};

#endif // DATASET_LOADER_HPP
