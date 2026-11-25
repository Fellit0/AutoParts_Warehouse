#include "StockOperationService.h"
#include <stdexcept>

StockOperationService::StockOperationService(std::shared_ptr<IOperationRepository> operations,
                                             std::shared_ptr<ISupplierRepository> suppliers,
                                             std::shared_ptr<IStockService> stock)
    : operations_(std::move(operations)), suppliers_(std::move(suppliers)), stock_(std::move(stock)) {}

int StockOperationService::receiveFromSupplier(int part_id, int supplier_id, int amount) {
    if (!suppliers_->findById(supplier_id))
        throw std::runtime_error("Поставщик с id " + std::to_string(supplier_id) + " не найден");

    StockOperation op(0, part_id, supplier_id, OperationType::IN, amount);  
    stock_->receivePart(part_id, amount);
    return operations_->save(op);
}

int StockOperationService::sell(int part_id, int amount) {
    StockOperation op(0, part_id, 0, OperationType::OUT, amount);
    stock_->sellPart(part_id, amount);  
    return operations_->save(op);
}

std::vector<StockOperation> StockOperationService::history() {
    return operations_->getAll();
}

std::vector<StockOperation> StockOperationService::historyForPart(int part_id) {
    return operations_->findByPart(part_id);
}

StockReport StockOperationService::reportForPeriod(const std::string& from, const std::string& to) {
    if (from > to) throw std::invalid_argument("Начало периода позже конца");

    StockReport report;
    report.operations = operations_->findByPeriod(from, to);
    for (const auto& op : report.operations) {
        if (op.getType() == OperationType::IN) {
            report.total_in += op.getQuantity();
            report.balance_by_part[op.getPartId()] += op.getQuantity();
        } else {
            report.total_out += op.getQuantity();
            report.balance_by_part[op.getPartId()] -= op.getQuantity();
        }
    }
    return report;
}