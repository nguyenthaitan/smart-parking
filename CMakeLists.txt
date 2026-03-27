# Data structure library
add_library(data_structures
    data/parking_space.cpp
    data/vehicle.cpp
)

# Domain services library
add_library(domain_services
    domain/parking_manager.cpp
    domain/vehicle_tracker.cpp
    domain/parking_lot.cpp
)
target_link_libraries(domain_services data_structures)

# Integration library
add_library(integration
    integration/dataset_loader.cpp
)
target_link_libraries(integration data_structures domain_services)

# Application library
add_library(application
    application/simulation_runner.cpp
)
target_link_libraries(application domain_services integration)

# Main executable
add_executable(ParkingSystem main.cpp)
target_link_libraries(ParkingSystem application domain_services integration data_structures)

# Set output directory
set_target_properties(ParkingSystem PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
