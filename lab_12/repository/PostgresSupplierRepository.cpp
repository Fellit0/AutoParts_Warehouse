#include "PostgresSupplierRepository.h"
#include <stdexcept>

PostgresSupplierRepository::PostgresSupplierRepository(const std::string& conn_str)
    : connection_string_(conn_str) {}

Supplier PostgresSupplierRepository::fromRow(const pqxx::row& row) {
    return Supplier(row[0].as<int>(),
                    row[1].as<std::string>(),
                    row[2].is_null() ? "" : row[2].as<std::string>(),
                    row[3].is_null() ? "" : row[3].as<std::string>());
}

int PostgresSupplierRepository::save(const Supplier& supplier) {
    try {
        pqxx::connection conn(connection_string_);
        pqxx::work tx(conn);
        auto result = tx.exec_params(
            "INSERT INTO suppliers (name, phone, email) VALUES ($1, $2, $3) RETURNING id",
            supplier.getName(), supplier.getPhone(), supplier.getEmail());
        tx.commit();
        return result[0][0].as<int>();
    } catch (const pqxx::broken_connection& e) {
        throw std::runtime_error(std::string("Нет подключения к БД: ") + e.what());
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error(std::string("Ошибка сохранения поставщика: ") + e.what());
    }
}

void PostgresSupplierRepository::update(const Supplier& supplier) {
    try {
        pqxx::connection conn(connection_string_);
        pqxx::work tx(conn);
        tx.exec_params("UPDATE suppliers SET name = $1, phone = $2, email = $3 WHERE id = $4",
                       supplier.getName(), supplier.getPhone(), supplier.getEmail(), supplier.getId());
        tx.commit();
    } catch (const pqxx::broken_connection& e) {
        throw std::runtime_error(std::string("Нет подключения к БД: ") + e.what());
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error(std::string("Ошибка обновления поставщика: ") + e.what());
    }
}

void PostgresSupplierRepository::remove(int id) {
    try {
        pqxx::connection conn(connection_string_);
        pqxx::work tx(conn);
        tx.exec_params("DELETE FROM suppliers WHERE id = $1", id);
        tx.commit();
    } catch (const pqxx::broken_connection& e) {
        throw std::runtime_error(std::string("Нет подключения к БД: ") + e.what());
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error(std::string("Ошибка удаления поставщика: ") + e.what());
    }
}

std::optional<Supplier> PostgresSupplierRepository::findById(int id) {
    try {
        pqxx::connection conn(connection_string_);
        pqxx::work tx(conn);
        auto result = tx.exec_params("SELECT id, name, phone, email FROM suppliers WHERE id = $1", id);
        tx.commit();
        if (result.empty()) return std::nullopt;
        return fromRow(result[0]);
    } catch (const pqxx::broken_connection& e) {
        throw std::runtime_error(std::string("Нет подключения к БД: ") + e.what());
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error(std::string("Ошибка поиска поставщика: ") + e.what());
    }
}

std::vector<Supplier> PostgresSupplierRepository::getAll() {
    try {
        pqxx::connection conn(connection_string_);
        pqxx::work tx(conn);
        auto result = tx.exec("SELECT id, name, phone, email FROM suppliers ORDER BY id");
        tx.commit();
        std::vector<Supplier> suppliers;
        for (const auto& row : result) suppliers.push_back(fromRow(row));
        return suppliers;
    } catch (const pqxx::broken_connection& e) {
        throw std::runtime_error(std::string("Нет подключения к БД: ") + e.what());
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error(std::string("Ошибка чтения поставщиков: ") + e.what());
    }
}