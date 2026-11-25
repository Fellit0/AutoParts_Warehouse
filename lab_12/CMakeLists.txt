cmake_minimum_required(VERSION 3.15)
project(AutoPartsWarehouse VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PostgreSQL REQUIRED)
find_package(libpqxx REQUIRED)

add_library(backend_core
    domain/Part.h
    repository/IPartRepository.h
    repository/PostgresPartRepository.cpp
    service/PartService.cpp
)

target_include_directories(backend_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(backend_core PUBLIC libpqxx::pqxx)