#include "config/DbConfig.h"
#include "repository/PostgresPartRepository.h"
#include "repository/PostgresSupplierRepository.h"
#include "repository/PostgresOperationRepository.h"
#include "service/PartService.h"
#include "service/SupplierService.h"
#include "service/StockOperationService.h"
#include "cli/ConsoleMenu.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    try {
        std::string config_path = argc > 1 ? argv[1] : "database.ini";
        std::string conn = DbConfig::load(config_path).connectionString();

        auto part_repo = std::make_shared<PostgresPartRepository>(conn);
        auto supplier_repo = std::make_shared<PostgresSupplierRepository>(conn);
        auto operation_repo = std::make_shared<PostgresOperationRepository>(conn);
        auto part_service = std::make_shared<PartService>(part_repo);
        SupplierService supplier_service(supplier_repo);
        StockOperationService operation_service(operation_repo, supplier_repo, part_service);

        try {
            supplier_service.listSuppliers();
        } catch (const std::exception& e) {
            std::cerr << "Не удалось подключиться к базе данных.\n" << e.what() << "\n";
            return 1;
        }

        SupplierMenu supplier_menu(supplier_service);
        OperationMenu operation_menu(operation_service);
        ConsoleMenu menu(*part_service, operation_service, supplier_menu, operation_menu);
        menu.run();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    return 0;
}