#include "PostgresPartRepository.h"
#include <stdexcept>

PostgresPartRepository::PostgresPartRepository(const std::string& conn_str) : connection_string_(conn_str) {}

void PostgresPartRepository::save(const Part& part) {
    pqxx::connection conn(connection_string_);
    pqxx::work tx(conn);
    
    try {
        tx.exec_params("INSERT INTO parts (article, name, price, quantity, supplier_id) VALUES ($1, $2, $3, $4, $5)",
                       part.getArticle(), part.getName(), part.getPrice(), part.getQuantity(), part.getSupplierId());
    } catch (const pqxx::foreign_key_violation&) {
        throw std::runtime_error("Поставщик с id " + std::to_string(part.getSupplierId().value_or(0)) + " не найден");
    }
    tx.commit();
}

void PostgresPartRepository::update(const Part& part) {
    pqxx::connection conn(connection_string_);
    pqxx::work tx(conn);
    
    if (part.getSupplierId().has_value()) {
        tx.exec_params("UPDATE parts SET price = $1, quantity = $2, supplier_id = $3 WHERE id = $4",
                       part.getPrice(), part.getQuantity(), part.getSupplierId().value(), part.getId());
    } else {
        tx.exec_params("UPDATE parts SET price = $1, quantity = $2, supplier_id = NULL WHERE id = $3",
                       part.getPrice(), part.getQuantity(), part.getId());
    }
    tx.commit();
}

void PostgresPartRepository::remove(int id) {
    pqxx::connection conn(connection_string_);
    pqxx::work tx(conn);
    tx.exec_params("DELETE FROM parts WHERE id = $1", id);
    tx.commit();
}

std::optional<Part> PostgresPartRepository::findById(int id) {
    pqxx::connection conn(connection_string_);
    pqxx::work tx(conn);
    auto result = tx.exec_params("SELECT id, article, name, price, quantity, supplier_id FROM parts WHERE id = $1", id);
    tx.commit();
    
    if (result.empty()) return std::nullopt;
    auto row = result[0];
    
    std::optional<int> sup_id = std::nullopt;
    if (!row[5].is_null()) {
        sup_id = row[5].as<int>();
    }

    return Part(row[0].as<int>(), row[1].as<std::string>(), row[2].as<std::string>(), 
                row[3].as<double>(), row[4].as<int>(), sup_id);
}

std::optional<Part> PostgresPartRepository::findByArticle(const std::string& article) {
    pqxx::connection conn(connection_string_);
    pqxx::work tx(conn);
    auto result = tx.exec_params("SELECT id, article, name, price, quantity, supplier_id FROM parts WHERE article = $1", article);
    tx.commit();
    
    if (result.empty()) return std::nullopt;
    auto row = result[0];
    
    std::optional<int> sup_id = std::nullopt;
    if (!row[5].is_null()) {
        sup_id = row[5].as<int>();
    }

    return Part(row[0].as<int>(), row[1].as<std::string>(), row[2].as<std::string>(), 
                row[3].as<double>(), row[4].as<int>(), sup_id);
}

std::vector<Part> PostgresPartRepository::getAll() {
    pqxx::connection conn(connection_string_);
    pqxx::work tx(conn);
    auto result = tx.exec("SELECT id, article, name, price, quantity, supplier_id FROM parts ORDER BY id");
    tx.commit();
    
    std::vector<Part> parts;
    for (auto row : result) {
        std::optional<int> sup_id = std::nullopt;
        if (!row[5].is_null()) {
            sup_id = row[5].as<int>();
        }
        parts.emplace_back(row[0].as<int>(), row[1].as<std::string>(), row[2].as<std::string>(), 
                           row[3].as<double>(), row[4].as<int>(), sup_id);
    }
    return parts;
}

double PostgresPartRepository::getTotalInventoryValue() {
    pqxx::connection conn(connection_string_);
    pqxx::work tx(conn);
    auto result = tx.exec("SELECT COALESCE(SUM(price * quantity), 0) FROM parts");
    tx.commit();
    return result[0][0].as<double>();
}