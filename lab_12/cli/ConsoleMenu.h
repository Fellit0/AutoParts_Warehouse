#pragma once
#include "service/PartService.h"
#include "service/StockOperationService.h"
#include "cli/SupplierMenu.h"
#include "cli/OperationMenu.h"

class ConsoleMenu {
private:
    PartService& part_service_;
    StockOperationService& operation_service_;
    SupplierMenu& supplier_menu_;
    OperationMenu& operation_menu_;

    void handleAddPart();
    void handleSellPart();
    void handleListParts();

public:
    ConsoleMenu(PartService& part_service, StockOperationService& operation_service,
                SupplierMenu& supplier_menu, OperationMenu& operation_menu);
    void run();
};