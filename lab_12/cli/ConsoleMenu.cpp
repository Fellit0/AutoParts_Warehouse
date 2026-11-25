#include "ConsoleMenu.h"

ConsoleMenu::ConsoleMenu(PartService& part_service) 
    : part_service_(part_service) {}

void ConsoleMenu::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
        std::cout << "Выберите действие: ";

        int choice;
        if (!(std::cin >> choice)) {
            clearInput();
            std::cout << "Ошибка ввода: Введите число.\n";
            continue;
        }

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
    std::string article, name; double price; int qty, sup_id;
    std::cout << "Артикул: "; std::cin >> article;
    std::cout << "Название: "; std::cin >> name;
    std::cout << "Цена: "; std::cin >> price;
    std::cout << "Кол-во: "; std::cin >> qty;
    std::cout << "ID Поставщика (0 если нет): "; std::cin >> sup_id;
    
    std::optional<int> sup_opt = (sup_id == 0) ? std::nullopt : std::optional<int>(sup_id);
    part_service_.addNewPart(article, name, price, qty, sup_opt);
    std::cout << "Запчасть успешно добавлена!\n";
}

void ConsoleMenu::handleSellPart() {
    int id, qty;
    std::cout << "ID запчасти: "; std::cin >> id;
    std::cout << "Сколько продать: "; std::cin >> qty;
    part_service_.sellPart(id, qty);
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