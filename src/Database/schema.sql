CREATE TABLE users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    email TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL,
    first_name TEXT,
    last_name TEXT,
    user_type TEXT CHECK(user_type IN ('customer', 'seller', 'admin')) NOT NULL,
    deposit_account_balance REAL
);

CREATE TABLE addresses (
    address_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    street TEXT,
    unit TEXT,
    city TEXT,
    state TEXT,
    zip_code INTEGER,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);


CREATE TABLE payment_methods (
    payment_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    card_number TEXT,
    card_type TEXT CHECK(card_type IN ('Visa', 'MasterCard', 'American Express', 'Discover')),
    expiry_date TEXT,
    billing_address_id INTEGER,
    cardholder_name TEXT,
    default_payment_method INTEGER NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (billing_address_id) REFERENCES addresses(address_id)
);

CREATE TABLE products (
    product_id INTEGER PRIMARY KEY AUTOINCREMENT,
    seller_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    type TEXT NOT NULL,
    description TEXT,
    price REAL NOT NULL,
    inventory INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY (seller_id) REFERENCES users(user_id)
);


CREATE TABLE customer_orders (
    order_id INTEGER PRIMARY KEY AUTOINCREMENT,
    customer_id INTEGER NOT NULL,
    order_date TEXT NOT NULL,
    shipping_address_id INTEGER,
    payment_id INTEGER,
    total REAL NOT NULL,
    FOREIGN KEY (customer_id) REFERENCES users(user_id),
    FOREIGN KEY (shipping_address_id) REFERENCES addresses(address_id),
    FOREIGN KEY (payment_id) REFERENCES payment_methods(payment_id)
);

CREATE TABLE order_items (
    order_item_id INTEGER PRIMARY KEY AUTOINCREMENT,
    order_id INTEGER NOT NULL,
    product_id INTEGER NOT NULL,
    seller_ID INTEGER NOT NULL,
    quantity INTEGER NOT NULL,
    price_at_purchase REAL NOT NULL,
    FOREIGN KEY (order_id) REFERENCES customer_orders(order_id),
    FOREIGN KEY (product_id) REFERENCES products(product_id),
    FOREIGN KEY (seller_ID) REFERENCES users(user_id)
);

CREATE TABLE seller_orders (
    seller_id INTEGER NOT NULL,
    order_id INTEGER NOT NULL,
    order_date TEXT NOT NULL,
    total REAL NOT NULL,
    FOREIGN KEY (seller_id) REFERENCES users(user_id),
    FOREIGN KEY (order_id) REFERENCES customer_orders(order_id)
);

