#pragma once
#include "repository/IOperationRepository.h"
#include <pqxx/pqxx>
#include <string>

class PostgresOperationRepository : public IOperationRepository {
private:
    std::string connection_string_;
    static StockOperation fromRow(const pqxx::row& row);
    static std::vector<StockOperation> toVector(const pqxx::result& result);

public:
    explicit PostgresOperationRepository(const std::string& conn_str);

    int save(const StockOperation& op) override;
    std::vector<StockOperation> getAll() override;
    std::vector<StockOperation> findByPart(int part_id) override;
    std::vector<StockOperation> findByPeriod(const std::string& from, const std::string& to) override;
};