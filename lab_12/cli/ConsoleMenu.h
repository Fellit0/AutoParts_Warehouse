#pragma once
#include "service/PartService.h"
#include "service/StockOperationService.h"
#include <iostream>
#include <limits>
#include <string>
#include <optional>

class ConsoleMenu {
private:
    PartService& part_service_;
    StockOperationService& operation_service_;

    void clearInput();
    void handleAddPart();
    void handleSellPart();
    void handleListParts();

    int readInt(const std::string& prompt);
    double readDouble(const std::string& prompt);
    std::string readString(const std::string& prompt);

public:
    ConsoleMenu(PartService& part_service, StockOperationService& operation_service);
    void run();
};