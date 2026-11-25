#pragma once
#include "repository/IPartRepository.h"
#include <memory>

class PartService {
private:
    std::shared_ptr<IPartRepository> repository_;
public:
    explicit PartService(std::shared_ptr<IPartRepository> repository);

    void addNewPart(const std::string& article, const std::string& name, double price, int quantity, int supplier_id);
    void sellPart(int part_id, int amount);
    void receivePart(int part_id, int amount);
    double calculateTotalWarehouseValue();
    std::vector<Part> listAllParts();
};