#include "PostgresOperationRepository.h"
#include <stdexcept>

namespace {
const char* SELECT_COLUMNS =
    "SELECT id, part_id, supplier_id, type, quantity, to_char(op_date, 'YYYY-MM-DD HH24:MI:SS') "
    "FROM operations ";
}

PostgresOperationRepository::PostgresOperationRepository(const std::string& conn_str)
    : connection_string_(conn_str) {}

StockOperation PostgresOperationRepository::fromRow(const pqxx::row& row) {
    return StockOperation(row[0].as<int>(),
                          row[1].as<int>(),
                          row[2].is_null() ? 0 : row[2].as<int>(),
                          operationTypeFromString(row[3].as<std::string>()),
                          row[4].as<int>(),
                          row[5].as<std::string>());
}

std::vector<StockOperation> PostgresOperationRepository::toVector(const pqxx::result& result) {
    std::vector<StockOperation> ops;
    for (const auto& row : result) ops.push_back(fromRow(row));
    return ops;
}

int PostgresOperationRepository::save(const StockOperation& op) {
    try {
        pqxx::connection conn(connection_string_);
        pqxx::work tx(conn);
        // supplier_id = 0 означает "не указан" -> пишем NULL
        std::optional<int> supplier;
        if (op.getSupplierId() > 0) supplier = op.getSupplierId();
        auto result = tx.exec_params(
            "INSERT INTO operations (part_id, supplier_id, type, quantity) VALUES ($1, $2, $3, $4) RETURNING id",
            op.getPartId(), supplier, toString(op.getType()), op.getQuantity());
        tx.commit();
        return result[0][0].as<int>();
    } catch (const pqxx::broken_connection& e) {
        throw std::runtime_error(std::string("Нет подключения к БД: ") + e.what());
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error(std::string("Ошибка сохранения операции: ") + e.what());
    }
}

std::vector<StockOperation> PostgresOperationRepository::getAll() {
    try {
        pqxx::connection conn(connection_string_);
        pqxx::work tx(conn);
        auto result = tx.exec(std::string(SELECT_COLUMNS) + "ORDER BY op_date, id");
        tx.commit();
        return toVector(result);
    } catch (const pqxx::broken_connection& e) {
        throw std::runtime_error(std::string("Нет подключения к БД: ") + e.what());
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error(std::string("Ошибка чтения операций: ") + e.what());
    }
}

std::vector<StockOperation> PostgresOperationRepository::findByPart(int part_id) {
    try {
        pqxx::connection conn(connection_string_);
        pqxx::work tx(conn);
        auto result = tx.exec_params(std::string(SELECT_COLUMNS) + "WHERE part_id = $1 ORDER BY op_date, id",
                                     part_id);
        tx.commit();
        return toVector(result);
    } catch (const pqxx::broken_connection& e) {
        throw std::runtime_error(std::string("Нет подключения к БД: ") + e.what());
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error(std::string("Ошибка чтения операций по детали: ") + e.what());
    }
}

std::vector<StockOperation> PostgresOperationRepository::findByPeriod(const std::string& from, const std::string& to) {
    try {
        pqxx::connection conn(connection_string_);
        pqxx::work tx(conn);
        auto result = tx.exec_params(
            std::string(SELECT_COLUMNS) +
            "WHERE op_date >= $1::date AND op_date < $2::date + 1 ORDER BY op_date, id",
            from, to);
        tx.commit();
        return toVector(result);
    } catch (const pqxx::broken_connection& e) {
        throw std::runtime_error(std::string("Нет подключения к БД: ") + e.what());
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error(std::string("Ошибка чтения операций за период: ") + e.what());
    }
}