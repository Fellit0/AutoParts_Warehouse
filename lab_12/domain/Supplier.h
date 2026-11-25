#pragma once
#include <string>
#include <stdexcept>

class Supplier {
private:
    int id_;
    std::string name_;
    std::string phone_;
    std::string email_;

    static void validateName(const std::string& name) {
        if (name.empty()) throw std::invalid_argument("Название поставщика не может быть пустым");
    }

public:
    Supplier(int id, std::string name, std::string phone, std::string email)
        : id_(id), name_(std::move(name)), phone_(std::move(phone)), email_(std::move(email)) {
        validateName(name_);
    }

    int getId() const { return id_; }
    const std::string& getName() const { return name_; }
    const std::string& getPhone() const { return phone_; }
    const std::string& getEmail() const { return email_; }

    void rename(const std::string& new_name) {
        validateName(new_name);
        name_ = new_name;
    }

    void updateContacts(const std::string& phone, const std::string& email) {
        if (!email.empty() && email.find('@') == std::string::npos)
            throw std::invalid_argument("Некорректный email");
        phone_ = phone;
        email_ = email;
    }
};