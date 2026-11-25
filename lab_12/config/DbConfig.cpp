#include "DbConfig.h"
#include <cstdlib>
#include <fstream>
#include <stdexcept>

namespace {
std::string trim(const std::string& s) {
    const char* spaces = " \t\r\n";
    auto begin = s.find_first_not_of(spaces);
    if (begin == std::string::npos) return "";
    auto end = s.find_last_not_of(spaces);
    return s.substr(begin, end - begin + 1);
}
}

void DbConfig::set(const std::string& key, const std::string& value) {
    if (key == "host") host_ = value;
    else if (key == "port") port_ = value;
    else if (key == "dbname") dbname_ = value;
    else if (key == "user") user_ = value;
    else if (key == "password") password_ = value;
}

void DbConfig::applyEnvironment() {
    const std::pair<const char*, const char*> vars[] = {
        {"DB_HOST", "host"}, {"DB_PORT", "port"}, {"DB_NAME", "dbname"},
        {"DB_USER", "user"}, {"DB_PASSWORD", "password"}};
    for (const auto& [env, key] : vars) {
        if (const char* value = std::getenv(env)) set(key, value);
    }
}

void DbConfig::validate() const {
    if (dbname_.empty()) throw std::runtime_error("В настройках БД не указан dbname");
    if (user_.empty()) throw std::runtime_error("В настройках БД не указан user");
}

DbConfig DbConfig::load(const std::string& path) {
    DbConfig config;
    std::ifstream file(path);
    if (file) {
        std::string line;
        while (std::getline(file, line)) {
            line = trim(line);
            if (line.empty() || line[0] == '#' || line[0] == ';' || line[0] == '[') continue;
            auto eq = line.find('=');
            if (eq == std::string::npos) continue;
            config.set(trim(line.substr(0, eq)), trim(line.substr(eq + 1)));
        }
    } else if (!std::getenv("DB_NAME")) {
        throw std::runtime_error("Не найден файл настроек " + path +
                                 " (скопируйте database.ini.example в database.ini или задайте DB_* переменные)");
    }
    config.applyEnvironment();
    config.validate();
    return config;
}

std::string DbConfig::quote(const std::string& value) {
    std::string result = "'";
    for (char c : value) {
        if (c == '\'' || c == '\\') result += '\\';
        result += c;
    }
    return result + "'";
}

std::string DbConfig::connectionString() const {
    std::string result = "host=" + quote(host_) + " port=" + quote(port_) +
                         " dbname=" + quote(dbname_) + " user=" + quote(user_);
    if (!password_.empty()) result += " password=" + quote(password_);
    return result;
}