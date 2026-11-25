#include "ConsoleMenu.h"

ConsoleMenu::ConsoleMenu(PartService& part_service, StockOperationService& operation_service) 
    : part_service_(part_service), operation_service_(operation_service) {}

void ConsoleMenu::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int ConsoleMenu::readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            clearInput();
            std::cout << "Ошибка: введите целое число.\n";
        }
    }
}

double ConsoleMenu::readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            clearInput();
            std::cout << "Ошибка: введите число (например, 150.50).\n";
        }
    }
}

std::string ConsoleMenu::readString(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin >> std::ws, value);
    return value;
}

void ConsoleMenu::run() {
    bool running = true;
    while (running) {
        std::cout << "\n СИСТЕМА УЧЕТА АВТОЗАПЧАСТЕЙ \n";
        std::cout << "1. Показать все запчасти\n";
        std::cout << "2. Добавить запчасть\n";
        std::cout << "3. Продать запчасть\n";
        std::cout << "4. Общая стоимость склада\n";
        std::cout << "0. Выход\n";

        int choice = readInt("Выберите действие: ");

        try {
            switch (choice) {
                case 1: handleListParts(); break;
                case 2: handleAddPart(); break;
                case 3: handleSellPart(); break;
                case 4: 
                    std::cout << "Общая стоимость склада: " 
                              << part_service_.calculateTotalWarehouseValue() << " руб.\n";
                    break;
                case 0: running = false; break;
                default: std::cout << "Неверный пункт меню.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "\n[ОШИБКА] " << e.what() << "\n";
        }
    }
}

void ConsoleMenu::handleAddPart() {
    std::string article = readString("Артикул: ");
    std::string name = readString("Название: ");
    double price = readDouble("Цена: ");
    int qty = readInt("Кол-во: ");
    int sup_id = readInt("ID Поставщика (0 если нет): ");
    
    std::optional<int> sup_opt = (sup_id == 0) ? std::nullopt : std::optional<int>(sup_id);
    part_service_.addNewPart(article, name, price, qty, sup_opt);
    std::cout << "Запчасть успешно добавлена\n";
}

void ConsoleMenu::handleSellPart() {
    int id = readInt("ID детали: ");
    int qty = readInt("Сколько продать: ");
    
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
                  << " | Цена: " << p.getPrice() << "\n";
    }
}