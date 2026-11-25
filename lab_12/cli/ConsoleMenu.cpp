#include "ConsoleMenu.h"
#include "ConsoleInput.h"
#include <iostream>
#include <optional>

ConsoleMenu::ConsoleMenu(PartService& part_service, StockOperationService& operation_service,
                         SupplierMenu& supplier_menu, OperationMenu& operation_menu)
    : part_service_(part_service), operation_service_(operation_service),
      supplier_menu_(supplier_menu), operation_menu_(operation_menu) {}

void ConsoleMenu::run() {
    bool running = true;
    while (running) {
        std::cout << "\n СИСТЕМА УЧЕТА АВТОЗАПЧАСТЕЙ \n";
        std::cout << "1. Показать все запчасти\n";
        std::cout << "2. Добавить запчасть\n";
        std::cout << "3. Продать запчасть\n";
        std::cout << "4. Общая стоимость склада\n";
        std::cout << "5. Поставщики\n";
        std::cout << "6. Приход и история операций\n";
        std::cout << "0. Выход\n";

        int choice = ConsoleInput::readInt("Выберите действие: ");

        try {
            switch (choice) {
                case 1: handleListParts(); break;
                case 2: handleAddPart(); break;
                case 3: handleSellPart(); break;
                case 4:
                    std::cout << "Общая стоимость склада: "
                              << part_service_.calculateTotalWarehouseValue() << " тг\n";
                    break;
                case 5: supplier_menu_.run(); break;
                case 6: operation_menu_.run(); break;
                case 0: running = false; break;
                default: std::cout << "Неверный пункт меню.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "\n[ОШИБКА] " << e.what() << "\n";
        }
    }
}

void ConsoleMenu::handleAddPart() {
    std::string article = ConsoleInput::readString("Артикул: ");
    std::string name = ConsoleInput::readString("Название: ");
    double price = ConsoleInput::readDouble("Цена: ");
    int qty = ConsoleInput::readInt("Кол-во: ");
    int sup_id = ConsoleInput::readInt("ID Поставщика (0 если нет): ");

    std::optional<int> sup_opt = (sup_id == 0) ? std::nullopt : std::optional<int>(sup_id);
    part_service_.addNewPart(article, name, price, qty, sup_opt);
    std::cout << "Запчасть успешно добавлена\n";
}

void ConsoleMenu::handleSellPart() {
    int id = ConsoleInput::readInt("ID детали: ");
    int qty = ConsoleInput::readInt("Сколько продать: ");

    operation_service_.sell(id, qty);

    std::cout << "Успешно продано!\n";
}

void ConsoleMenu::handleListParts() {
    auto parts = part_service_.listAllParts();
    if (parts.empty()) {
        std::cout << "Склад пуст.\n";
        return;
    }
    std::cout << "\n СПИСОК ЗАПЧАСТЕЙ \n";
    for (const auto& p : parts) {
        std::cout << "ID: " << p.getId() << " | Артикул: " << p.getArticle()
                  << " | " << p.getName() << " | Кол-во: " << p.getQuantity()
                  << " | Цена: " << p.getPrice()
                  << " | Поставщик: " << (p.getSupplierId() ? std::to_string(*p.getSupplierId()) : "-") << "\n";
    }
}