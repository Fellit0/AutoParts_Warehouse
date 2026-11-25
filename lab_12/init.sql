CREATE TABLE IF NOT EXISTS suppliers (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    phone VARCHAR(30),
    email VARCHAR(100)
);

CREATE TABLE IF NOT EXISTS parts (
    id SERIAL PRIMARY KEY,
    article VARCHAR(50) UNIQUE NOT NULL,
    name VARCHAR(100) NOT NULL,
    price NUMERIC(10, 2) NOT NULL CHECK (price >= 0),
    quantity INT NOT NULL DEFAULT 0 CHECK (quantity >= 0),
    supplier_id INT REFERENCES suppliers(id) ON DELETE SET NULL
);

CREATE TABLE IF NOT EXISTS operations (
    id SERIAL PRIMARY KEY,
    part_id INT NOT NULL REFERENCES parts(id) ON DELETE CASCADE,
    supplier_id INT REFERENCES suppliers(id) ON DELETE SET NULL,
    type VARCHAR(3) NOT NULL CHECK (type IN ('IN', 'OUT')),
    quantity INT NOT NULL CHECK (quantity > 0),
    op_date TIMESTAMP NOT NULL DEFAULT now()
);