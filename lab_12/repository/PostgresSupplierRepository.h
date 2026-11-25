#pragma once
#include "repository/ISupplierRepository.h"
#include <pqxx/pqxx>
#include <string>

class PostgresSupplierRepository : public ISupplierRepository {
private:
    std::string connection_string_;
    static Supplier fromRow(const pqxx::row& row);

public:
    explicit PostgresSupplierRepository(const std::string& conn_str);

    int save(const Supplier& supplier) override;
    void update(const Supplier& supplier) override;
    void remove(int id) override;
    std::optional<Supplier> findById(int id) override;
    std::vector<Supplier> getAll() override;
};