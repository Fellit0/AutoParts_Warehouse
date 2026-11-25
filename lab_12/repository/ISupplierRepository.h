#pragma once
#include "domain/Supplier.h"
#include <vector>
#include <optional>

class ISupplierRepository {
public:
    virtual ~ISupplierRepository() = default;
    virtual int save(const Supplier& supplier) = 0;  
    virtual void update(const Supplier& supplier) = 0;
    virtual void remove(int id) = 0;
    virtual std::optional<Supplier> findById(int id) = 0;
    virtual std::vector<Supplier> getAll() = 0;
};