#pragma once
#include "domain/StockOperation.h"
#include <vector>
#include <string>

class IOperationRepository {
public:
    virtual ~IOperationRepository() = default;
    virtual int save(const StockOperation& op) = 0;  
    virtual std::vector<StockOperation> getAll() = 0;
    virtual std::vector<StockOperation> findByPart(int part_id) = 0;
    virtual std::vector<StockOperation> findByPeriod(const std::string& from, const std::string& to) = 0;
};