#pragma once
#include "domain/Part.h"
#include <vector>
#include <optional>

class IPartRepository {
public:
    virtual ~IPartRepository() = default;
    virtual void save(const Part& part) = 0;
    virtual void update(const Part& part) = 0;
    virtual void remove(int id) = 0;
    virtual std::optional<Part> findById(int id) = 0;
    virtual std::optional<Part> findByArticle(const std::string& article) = 0;
    virtual std::vector<Part> getAll() = 0;
    virtual double getTotalInventoryValue() = 0;
};