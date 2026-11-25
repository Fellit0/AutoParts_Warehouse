#pragma once
#include "IPartRepository.h"
#include <pqxx/pqxx>
#include <string>

class PostgresPartRepository : public IPartRepository {
private:
    std::string connection_string_;
public:
    explicit PostgresPartRepository(const std::string& conn_str);
    void save(const Part& part) override;
    void update(const Part& part) override;
    void remove(int id) override;
    std::optional<Part> findById(int id) override;
    std::optional<Part> findByArticle(const std::string& article) override;
    std::vector<Part> getAll() override;
    double getTotalInventoryValue() override;
};