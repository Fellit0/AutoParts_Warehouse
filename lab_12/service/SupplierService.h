#pragma once
#include "repository/ISupplierRepository.h"
#include <memory>

class SupplierService {
private:
    std::shared_ptr<ISupplierRepository> repository_;
    Supplier getExisting(int id);

public:
    explicit SupplierService(std::shared_ptr<ISupplierRepository> repository);

    int addSupplier(const std::string& name, const std::string& phone, const std::string& email);
    void renameSupplier(int id, const std::string& new_name);
    void updateContacts(int id, const std::string& phone, const std::string& email);
    void removeSupplier(int id);
    Supplier getSupplier(int id);
    std::vector<Supplier> listSuppliers();
};