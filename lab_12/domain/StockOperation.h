#pragma once
#include <string>
#include <stdexcept>

enum class OperationType { IN, OUT };

inline std::string toString(OperationType type) {
    return type == OperationType::IN ? "IN" : "OUT";
}

inline OperationType operationTypeFromString(const std::string& s) {
    if (s == "IN") return OperationType::IN;
    if (s == "OUT") return OperationType::OUT;
    throw std::invalid_argument("Неизвестный тип операции: " + s);
}

class StockOperation {
private:
    int id_;
    int part_id_;
    int supplier_id_;  
    OperationType type_;
    int quantity_;
    std::string date_;  

public:
    StockOperation(int id, int part_id, int supplier_id, OperationType type, int quantity, std::string date = "")
        : id_(id), part_id_(part_id), supplier_id_(supplier_id), type_(type),
          quantity_(quantity), date_(std::move(date)) {
        if (quantity_ <= 0) throw std::invalid_argument("Количество в операции должно быть > 0");
    }

    int getId() const { return id_; }
    int getPartId() const { return part_id_; }
    int getSupplierId() const { return supplier_id_; }
    OperationType getType() const { return type_; }
    int getQuantity() const { return quantity_; }
    const std::string& getDate() const { return date_; }
};