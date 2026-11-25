#pragma once
#include "repository/IPartRepository.h"
#include "service/IStockService.h"
#include <memory>
#include <optional>

class PartService : public IStockService {
private:
    std::shared_ptr<IPartRepository> repository_;
public:
    explicit PartService(std::shared_ptr<IPartRepository> repository);

    void addNewPart(const std::string& article, const std::string& name, double price, int quantity, std::optional<int> supplier_id);
    void sellPart(int part_id, int amount) override;
    void receivePart(int part_id, int amount) override;
    double calculateTotalWarehouseValue();
    std::vector<Part> listAllParts();
};