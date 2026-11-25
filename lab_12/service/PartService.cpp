#include "PartService.h"
#include <iostream>

PartService::PartService(std::shared_ptr<IPartRepository> repository) : repository_(std::move(repository)) {}

void PartService::addNewPart(const std::string& article, const std::string& name, double price, int quantity, int supplier_id) {
    if (repository_->findByArticle(article).has_value()) {
        throw std::runtime_error("Запчасть с таким артикулом уже существует");
    }
    Part new_part(0, article, name, price, quantity, supplier_id);
    repository_->save(new_part);
}

void PartService::sellPart(int part_id, int amount) {
    auto part_opt = repository_->findById(part_id);
    if (!part_opt.has_value()) {
        throw std::runtime_error("Запчасть не найдена");
    }
    
    Part part = part_opt.value();
    part.decreaseStock(amount);
    repository_->update(part); 
}

void PartService::receivePart(int part_id, int amount) {
    auto part_opt = repository_->findById(part_id);
    if (!part_opt.has_value()) throw std::runtime_error("Запчасть не найдена");
    
    Part part = part_opt.value();
    part.increaseStock(amount);
    repository_->update(part);
}

double PartService::calculateTotalWarehouseValue() {
    return repository_->getTotalInventoryValue();
}

std::vector<Part> PartService::listAllParts() {
    return repository_->getAll();
}