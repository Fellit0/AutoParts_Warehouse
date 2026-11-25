#pragma once
#include <string>
#include <stdexcept>

class Part {
private:
    int id_;
    std::string article_;
    std::string name_;
    double price_;
    int quantity_;
    int supplier_id_;

public:
    Part(int id, std::string article, std::string name, double price, int quantity, int supplier_id)
        : id_(id), article_(std::move(article)), name_(std::move(name)), 
          price_(price), quantity_(quantity), supplier_id_(supplier_id) {
        if (price_ < 0 || quantity_ < 0) {
            throw std::invalid_argument("Цена и количество не могут быть отрицательными.");
        }
    }
    int getId() const { return id_; }
    const std::string& getArticle() const { return article_; }
    const std::string& getName() const { return name_; }
    double getPrice() const { return price_; }
    int getQuantity() const { return quantity_; }
    int getSupplierId() const { return supplier_id_; }

    void increaseStock(int amount) {
        if (amount <= 0) throw std::invalid_argument("Количество для прихода должно быть > 0");
        quantity_ += amount;
    }

    void decreaseStock(int amount) {
        if (amount <= 0) throw std::invalid_argument("Количество для расхода должно быть > 0");
        if (quantity_ < amount) throw std::runtime_error("Недостаточно деталей на складе!");
        quantity_ -= amount;
    }

    void updatePrice(double new_price) {
        if (new_price < 0) throw std::invalid_argument("Цена не может быть отрицательной");
        price_ = new_price;
    }
};