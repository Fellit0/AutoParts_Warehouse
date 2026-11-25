#pragma once

class IStockService {
public:
    virtual ~IStockService() = default;
    virtual void receivePart(int part_id, int amount) = 0;
    virtual void sellPart(int part_id, int amount) = 0;
};