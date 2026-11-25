#pragma once
#include "service/StockOperationService.h"
#include <vector>

class OperationMenu {
private:
    StockOperationService& operation_service_;

    void handleReceive();
    void handleHistory();
    void handlePartHistory();
    void handleReport();
    static void printOperations(const std::vector<StockOperation>& ops);

public:
    explicit OperationMenu(StockOperationService& operation_service);
    void run();
};