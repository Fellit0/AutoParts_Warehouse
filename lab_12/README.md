# Учёт автозапчастей на складе

Консольное приложение на C++17 для учёта запчастей, поставщиков и движения товара (приход/продажа) с хранением данных в PostgreSQL.

## Зависимости

- компилятор C++17 (g++ 9+), CMake 3.15+
- PostgreSQL 14+
- libpqxx 7.x и pkg-config

Ubuntu / WSL:

```bash
sudo apt install g++ cmake pkg-config libpqxx-dev postgresql
```

## Настройка базы данных

Запустить PostgreSQL и создать базу:

```bash
sudo service postgresql start
sudo -u postgres psql -c "ALTER USER postgres PASSWORD 'ваш_пароль';"
sudo -u postgres createdb auto_parts_db
```

Применить SQL-скрипт инициализации (создаёт таблицы `suppliers`, `parts`, `operations`):

```bash
psql -h localhost -U postgres -d auto_parts_db -f init.sql
```

## Настройка подключения

Скопировать пример конфигурации и вписать свой пароль:

```bash
cp database.ini.example database.ini
```

`database.ini` не хранится в git. Вместо файла (или поверх него) можно задать переменные окружения
`DB_HOST`, `DB_PORT`, `DB_NAME`, `DB_USER`, `DB_PASSWORD`.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

## Запуск

Из папки проекта (там, где лежит `database.ini`):

```bash
./build/autoparts
```

Путь к другому файлу настроек можно передать аргументом: `./build/autoparts /path/to/database.ini`.

## Структура

```
domain/      — сущности: Part, Supplier, StockOperation
repository/  — интерфейсы доступа к данным и их реализации на PostgreSQL (libpqxx)
service/     — бизнес-логика: PartService, SupplierService, StockOperationService
cli/         — консольное меню (без SQL, работает только через сервисы)
config/      — чтение настроек подключения
main.cpp     — создание объектов и передача зависимостей
init.sql     — схема базы данных
```