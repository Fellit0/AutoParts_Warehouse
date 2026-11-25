cmake_minimum_required(VERSION 3.15)
project(AutoPartsWarehouse VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)

# domain + repository + service + config
add_library(backend_core
    config/DbConfig.cpp
    repository/PostgresPartRepository.cpp
    repository/PostgresSupplierRepository.cpp
    repository/PostgresOperationRepository.cpp
    service/PartService.cpp
    service/SupplierService.cpp
    service/StockOperationService.cpp
)
target_include_directories(backend_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(backend_core PUBLIC PkgConfig::PQXX)

# консольное приложение
add_executable(autoparts
    main.cpp
    cli/ConsoleInput.cpp
    cli/ConsoleMenu.cpp
    cli/SupplierMenu.cpp
    cli/OperationMenu.cpp
)
target_link_libraries(autoparts PRIVATE backend_core)
