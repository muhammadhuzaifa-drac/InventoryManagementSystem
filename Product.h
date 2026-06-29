#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

struct Product {
    int id;
    std::string name;
    double price;
    int quantity;
    std::string category;
};

struct Transaction {
    std::string timestamp;
    int productId;
    std::string productName;
    int quantity;
    double priceEach;
    double totalAmount;
};

#endif // PRODUCT_H
