#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>
#include "Product.h"

class Inventory {
private:
    std::vector<Product> products;
    std::vector<Transaction> transactions;
    std::string filename = "inventory.dat";
    std::string txFilename = "transactions.dat";
    const int lowStockThreshold = 5;
    const size_t maxTransactions = 10;

    int findProductIndexById(int id) const;
    void addTransaction(int productId, const std::string& productName, int quantity, double priceEach);
    std::string getCurrentTimestamp() const;

public:
    void addProduct();
    void updateStock();
    void sellProduct();
    void searchProduct() const;
    void displayAll() const;
    void showLowStock() const;
    void calculateTotalValue() const;
    void saveToFile() const;
    void loadFromFile();
    void generateRandomProducts();   // For testing

    // Additional Features
    void deleteProduct();
    void showRestockSuggestions() const;
    void showTransactionHistory() const;
};

#endif // INVENTORY_H
