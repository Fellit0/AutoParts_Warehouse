#include "SupplierMenu.h"
#include "ConsoleInput.h"
#include <iostream>

SupplierMenu::SupplierMenu(SupplierService& supplier_service)
    : supplier_service_(supplier_service) {}

void SupplierMenu::run() {
    while (true) {
        std::cout << "\n ПОСТАВЩИКИ \n";
        std::cout << "1. Показать всех поставщиков\n";
        std::cout << "2. Добавить поставщика\n";
        std::cout << "3. Переименовать поставщика\n";
        std::cout << "4. Изменить контакты\n";
        std::cout << "5. Удалить поставщика\n";
        std::cout << "0. Назад\n";

        int choice = ConsoleInput::readInt("Выберите действие: ");
        try {
            switch (choice) {
                case 1: handleList(); break;
                case 2: handleAdd(); break;
                case 3: handleRename(); break;
                case 4: handleUpdateContacts(); break;
                case 5: handleRemove(); break;
                case 0: return;
                default: std::cout << "Неверный пункт меню.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "\n[ОШИБКА] " << e.what() << "\n";
        }
    }
}

void SupplierMenu::handleList() {
    auto suppliers = supplier_service_.listSuppliers();
    if (suppliers.empty()) {
        std::cout << "Поставщиков нет.\n";
        return;
    }
    std::cout << "\n СПИСОК ПОСТАВЩИКОВ \n";
    for (const auto& s : suppliers) {
        std::cout << "ID: " << s.getId() << " | " << s.getName()
                  << " | Тел.: " << (s.getPhone().empty() ? "-" : s.getPhone())
                  << " | Email: " << (s.getEmail().empty() ? "-" : s.getEmail()) << "\n";
    }
}

void SupplierMenu::handleAdd() {
    std::string name = ConsoleInput::readString("Название: ");
    std::string phone = ConsoleInput::readOptionalString("Телефон (Enter — пропустить): ");
    std::string email = ConsoleInput::readOptionalString("Email (Enter — пропустить): ");
    int id = supplier_service_.addSupplier(name, phone, email);
    std::cout << "Поставщик добавлен, ID: " << id << "\n";
}

void SupplierMenu::handleRename() {
    int id = ConsoleInput::readInt("ID поставщика: ");
    std::string name = ConsoleInput::readString("Новое название: ");
    supplier_service_.renameSupplier(id, name);
    std::cout << "Поставщик переименован.\n";
}

void SupplierMenu::handleUpdateContacts() {
    int id = ConsoleInput::readInt("ID поставщика: ");
    std::string phone = ConsoleInput::readOptionalString("Новый телефон (Enter — пусто): ");
    std::string email = ConsoleInput::readOptionalString("Новый email (Enter — пусто): ");
    supplier_service_.updateContacts(id, phone, email);
    std::cout << "Контакты обновлены.\n";
}

void SupplierMenu::handleRemove() {
    int id = ConsoleInput::readInt("ID поставщика: ");
    supplier_service_.removeSupplier(id);
    std::cout << "Поставщик удалён. У его запчастей и операций поставщик теперь не указан.\n";
}