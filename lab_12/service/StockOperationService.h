#pragma once
#include "repository/IOperationRepository.h"
#include "repository/ISupplierRepository.h"
#include "service/IStockService.h"
#include <memory>
#include <map>

struct StockReport {
    int total_in = 0;
    int total_out = 0;
    std::map<int, int> balance_by_part;  
    std::vector<StockOperation> operations;
};

class StockOperationService {
private:
    std::shared_ptr<IOperationRepository> operations_;
    std::shared_ptr<ISupplierRepository> suppliers_;
    std::shared_ptr<IStockService> stock_;

public:
    StockOperationService(std::shared_ptr<IOperationRepository> operations,
                          std::shared_ptr<ISupplierRepository> suppliers,
                          std::shared_ptr<IStockService> stock);

    int receiveFromSupplier(int part_id, int supplier_id, int amount);
    int sell(int part_id, int amount);

    std::vector<StockOperation> history();
    std::vector<StockOperation> historyForPart(int part_id);
    StockReport reportForPeriod(const std::string& from, const std::string& to);
};