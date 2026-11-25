#include "SupplierService.h"
#include <stdexcept>

SupplierService::SupplierService(std::shared_ptr<ISupplierRepository> repository)
    : repository_(std::move(repository)) {}

Supplier SupplierService::getExisting(int id) {
    auto supplier = repository_->findById(id);
    if (!supplier) throw std::runtime_error("Поставщик с id " + std::to_string(id) + " не найден");
    return *supplier;
}

int SupplierService::addSupplier(const std::string& name, const std::string& phone, const std::string& email) {
    Supplier supplier(0, name, "", "");
    supplier.updateContacts(phone, email);  
    return repository_->save(supplier);
}

void SupplierService::renameSupplier(int id, const std::string& new_name) {
    Supplier supplier = getExisting(id);
    supplier.rename(new_name);
    repository_->update(supplier);
}

void SupplierService::updateContacts(int id, const std::string& phone, const std::string& email) {
    Supplier supplier = getExisting(id);
    supplier.updateContacts(phone, email);
    repository_->update(supplier);
}

void SupplierService::removeSupplier(int id) {
    getExisting(id);
    repository_->remove(id);
}

Supplier SupplierService::getSupplier(int id) {
    return getExisting(id);
}

std::vector<Supplier> SupplierService::listSuppliers() {
    return repository_->getAll();
}