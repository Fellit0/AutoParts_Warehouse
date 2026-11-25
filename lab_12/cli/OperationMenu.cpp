#include "OperationMenu.h"
#include "ConsoleInput.h"
#include <iostream>

OperationMenu::OperationMenu(StockOperationService& operation_service)
    : operation_service_(operation_service) {}

void OperationMenu::run() {
    while (true) {
        std::cout << "\n ДВИЖЕНИЕ ТОВАРА \n";
        std::cout << "1. Приход от поставщика\n";
        std::cout << "2. История всех операций\n";
        std::cout << "3. История по запчасти\n";
        std::cout << "4. Отчёт за период\n";
        std::cout << "0. Назад\n";

        int choice = ConsoleInput::readInt("Выберите действие: ");
        try {
            switch (choice) {
                case 1: handleReceive(); break;
                case 2: handleHistory(); break;
                case 3: handlePartHistory(); break;
                case 4: handleReport(); break;
                case 0: return;
                default: std::cout << "Неверный пункт меню.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "\n[ОШИБКА] " << e.what() << "\n";
        }
    }
}

void OperationMenu::printOperations(const std::vector<StockOperation>& ops) {
    if (ops.empty()) {
        std::cout << "Операций нет.\n";
        return;
    }
    for (const auto& op : ops) {
        std::cout << "#" << op.getId() << " | " << op.getDate()
                  << " | " << (op.getType() == OperationType::IN ? "Приход" : "Продажа")
                  << " | Запчасть ID: " << op.getPartId()
                  << " | Кол-во: " << op.getQuantity();
        if (op.getSupplierId() > 0) std::cout << " | Поставщик ID: " << op.getSupplierId();
        std::cout << "\n";
    }
}

void OperationMenu::handleReceive() {
    int part_id = ConsoleInput::readInt("ID запчасти: ");
    int supplier_id = ConsoleInput::readInt("ID поставщика: ");
    int amount = ConsoleInput::readInt("Количество: ");
    operation_service_.receiveFromSupplier(part_id, supplier_id, amount);
    std::cout << "Приход оформлен.\n";
}

void OperationMenu::handleHistory() {
    std::cout << "\n ИСТОРИЯ ОПЕРАЦИЙ \n";
    printOperations(operation_service_.history());
}

void OperationMenu::handlePartHistory() {
    int part_id = ConsoleInput::readInt("ID запчасти: ");
    std::cout << "\n ИСТОРИЯ ПО ЗАПЧАСТИ " << part_id << " \n";
    printOperations(operation_service_.historyForPart(part_id));
}

void OperationMenu::handleReport() {
    std::string from = ConsoleInput::readDate("С (ГГГГ-ММ-ДД): ");
    std::string to = ConsoleInput::readDate("По (ГГГГ-ММ-ДД): ");
    StockReport report = operation_service_.reportForPeriod(from, to);

    std::cout << "\n ОТЧЁТ С " << from << " ПО " << to << " \n";
    printOperations(report.operations);
    std::cout << "Всего приход: " << report.total_in << " шт.\n";
    std::cout << "Всего продано: " << report.total_out << " шт.\n";
    if (!report.balance_by_part.empty()) {
        std::cout << "Изменение остатка по запчастям:\n";
        for (const auto& [part_id, delta] : report.balance_by_part)
            std::cout << "  ID " << part_id << ": " << (delta > 0 ? "+" : "") << delta << "\n";
    }
}