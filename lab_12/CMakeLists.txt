cmake_minimum_required(VERSION 3.15)
project(AutoPartsWarehouse VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)

add_library(backend_core
    repository/PostgresPartRepository.cpp
    repository/PostgresSupplierRepository.cpp
    repository/PostgresOperationRepository.cpp
    service/PartService.cpp
    service/SupplierService.cpp
    service/StockOperationService.cpp
)

target_include_directories(backend_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(backend_core PUBLIC PkgConfig::PQXX)