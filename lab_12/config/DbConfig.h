#pragma once
#include <string>

class DbConfig {
private:
    std::string host_ = "localhost";
    std::string port_ = "5432";
    std::string dbname_;
    std::string user_;
    std::string password_;

    void set(const std::string& key, const std::string& value);
    void applyEnvironment();
    void validate() const;
    static std::string quote(const std::string& value);

public:
    static DbConfig load(const std::string& path);
    std::string connectionString() const;
};