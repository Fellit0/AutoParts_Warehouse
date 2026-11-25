#pragma once
#include "service/SupplierService.h"

class SupplierMenu {
private:
    SupplierService& supplier_service_;

    void handleList();
    void handleAdd();
    void handleRename();
    void handleUpdateContacts();
    void handleRemove();

public:
    explicit SupplierMenu(SupplierService& supplier_service);
    void run();
};