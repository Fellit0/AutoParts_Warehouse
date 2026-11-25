#pragma once
#include "service/PartService.h"
#include <iostream>
#include <limits>

class ConsoleMenu {
private:
    PartService& part_service_;

    void clearInput();
    void handleAddPart();
    void handleSellPart();
    void handleListParts();

public:
    ConsoleMenu(PartService& part_service);
    void run();
};