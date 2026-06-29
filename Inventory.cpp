#include "Inventory.h"
#include "Console.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>
#include <ctime>
#include <sstream>
#include <cctype>

// Helper to check case-insensitive substring
static bool isSubstringCaseInsensitive(const std::string& str, const std::string& sub) {
    if (sub.empty()) return true;
    auto it = std::search(
        str.begin(), str.end(),
        sub.begin(), sub.end(),
        [](char ch1, char ch2) {
            return std::tolower(static_cast<unsigned char>(ch1)) == std::tolower(static_cast<unsigned char>(ch2));
        }
    );
    return it != str.end();
}

// Helper to format tables
static void printProductTableHeader() {
    Console::setColor(Console::BOLD_CYAN);
    std::cout << std::left 
              << std::setw(8)  << "ID"
              << std::setw(25) << "Name"
              << std::setw(15) << "Category"
              << std::setw(12) << "Price ($)"
              << std::setw(10) << "Quantity"
              << "Status" << "\n";
    std::cout << std::string(75, '-') << "\n";
    Console::setColor(Console::RESET);
}

static void printProductRow(const Product& p, int threshold) {
    std::cout << std::left 
              << std::setw(8)  << p.id
              << std::setw(25) << (p.name.length() > 23 ? p.name.substr(0, 21) + ".." : p.name)
              << std::setw(15) << (p.category.length() > 13 ? p.category.substr(0, 11) + ".." : p.category)
              << std::fixed << std::setprecision(2) << std::setw(12) << p.price
              << std::setw(10) << p.quantity;
    if (p.quantity <= threshold) {
        Console::setColor(Console::BOLD_RED);
        std::cout << "LOW STOCK (" << p.quantity << ")";
    } else {
        Console::setColor(Console::GREEN);
        std::cout << "In Stock";
    }
    Console::setColor(Console::RESET);
    std::cout << "\n";
}

int Inventory::findProductIndexById(int id) const {
    for (size_t i = 0; i < products.size(); ++i) {
        if (products[i].id == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

std::string Inventory::getCurrentTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    if (!localTime) return "N/A";
    
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localTime);
    return std::string(buf);
}

void Inventory::addTransaction(int productId, const std::string& productName, int quantity, double priceEach) {
    Transaction tx;
    tx.timestamp = getCurrentTimestamp();
    tx.productId = productId;
    tx.productName = productName;
    tx.quantity = quantity;
    tx.priceEach = priceEach;
    tx.totalAmount = quantity * priceEach;
    
    transactions.insert(transactions.begin(), tx);
    if (transactions.size() > maxTransactions) {
        transactions.resize(maxTransactions);
    }
}

void Inventory::addProduct() {
    Console::printHeader("Add New Product");
    
    int id = Console::readInt("Enter unique Product ID (Numeric): ", 1);
    if (findProductIndexById(id) != -1) {
        Console::setColor(Console::BOLD_RED);
        std::cout << "Error: A product with ID " << id << " already exists!\n";
        Console::setColor(Console::RESET);
        Console::pressEnterToContinue();
        return;
    }
    
    std::string name = Console::readString("Enter Product Name: ");
    std::string category = Console::readString("Enter Product Category: ");
    double price = Console::readDouble("Enter Product Price ($): ", 0.01);
    int quantity = Console::readInt("Enter Initial Quantity: ", 0);
    
    Product p = { id, name, price, quantity, category };
    products.push_back(p);
    
    Console::setColor(Console::BOLD_GREEN);
    std::cout << "\nSuccess: Product '" << name << "' added successfully!\n";
    Console::setColor(Console::RESET);
    Console::pressEnterToContinue();
}

void Inventory::updateStock() {
    Console::printHeader("Update Stock Quantity");
    if (products.empty()) {
        std::cout << "Inventory is empty.\n";
        Console::pressEnterToContinue();
        return;
    }
    
    int id = Console::readInt("Enter Product ID: ");
    int idx = findProductIndexById(id);
    if (idx == -1) {
        Console::setColor(Console::BOLD_RED);
        std::cout << "Error: Product ID " << id << " not found.\n";
        Console::setColor(Console::RESET);
        Console::pressEnterToContinue();
        return;
    }
    
    Product& p = products[idx];
    Console::setColor(Console::CYAN);
    std::cout << "Found Product: " << p.name << " (Current Stock: " << p.quantity << ")\n";
    Console::setColor(Console::RESET);
    
    std::cout << "1. Add to stock (Restock)\n";
    std::cout << "2. Deduct from stock\n";
    std::cout << "3. Set new stock level directly\n";
    int choice = Console::readInt("Choose option (1-3): ", 1, 3);
    
    if (choice == 1) {
        int amount = Console::readInt("Enter quantity to add: ", 1);
        p.quantity += amount;
        Console::setColor(Console::BOLD_GREEN);
        std::cout << "Stock updated. New stock: " << p.quantity << "\n";
    } else if (choice == 2) {
        int amount = Console::readInt("Enter quantity to deduct: ", 1, p.quantity);
        p.quantity -= amount;
        Console::setColor(Console::BOLD_GREEN);
        std::cout << "Stock updated. New stock: " << p.quantity << "\n";
    } else if (choice == 3) {
        int amount = Console::readInt("Enter new stock level: ", 0);
        p.quantity = amount;
        Console::setColor(Console::BOLD_GREEN);
        std::cout << "Stock level set. New stock: " << p.quantity << "\n";
    }
    
    Console::setColor(Console::RESET);
    Console::pressEnterToContinue();
}

void Inventory::sellProduct() {
    Console::printHeader("Sell Product");
    if (products.empty()) {
        std::cout << "Inventory is empty.\n";
        Console::pressEnterToContinue();
        return;
    }
    
    int id = Console::readInt("Enter Product ID: ");
    int idx = findProductIndexById(id);
    if (idx == -1) {
        Console::setColor(Console::BOLD_RED);
        std::cout << "Error: Product ID " << id << " not found.\n";
        Console::setColor(Console::RESET);
        Console::pressEnterToContinue();
        return;
    }
    
    Product& p = products[idx];
    Console::setColor(Console::CYAN);
    std::cout << "Selling: " << p.name << " | Stock Available: " << p.quantity << " | Price: $" << p.price << "\n";
    Console::setColor(Console::RESET);
    
    if (p.quantity == 0) {
        Console::setColor(Console::BOLD_RED);
        std::cout << "Error: This item is currently out of stock.\n";
        Console::setColor(Console::RESET);
        Console::pressEnterToContinue();
        return;
    }
    
    int sellQty = Console::readInt("Enter quantity to sell: ", 1, p.quantity);
    p.quantity -= sellQty;
    
    double total = sellQty * p.price;
    addTransaction(p.id, p.name, sellQty, p.price);
    
    Console::setColor(Console::BOLD_GREEN);
    std::cout << "\n================ SALE SUCCESSFUL ================\n";
    std::cout << "Product:      " << p.name << "\n";
    std::cout << "Qty Sold:     " << sellQty << "\n";
    std::cout << "Price/Unit:   $" << std::fixed << std::setprecision(2) << p.price << "\n";
    std::cout << "Total Amount: $" << total << "\n";
    std::cout << "Remaining:    " << p.quantity << " units\n";
    std::cout << "=================================================\n";
    Console::setColor(Console::RESET);
    
    Console::pressEnterToContinue();
}

void Inventory::searchProduct() const {
    Console::printHeader("Search Product");
    if (products.empty()) {
        std::cout << "Inventory is empty.\n";
        Console::pressEnterToContinue();
        return;
    }
    
    std::cout << "Search by:\n";
    std::cout << "1. Product ID\n";
    std::cout << "2. Product Name (partial match)\n";
    int choice = Console::readInt("Choose option (1-2): ", 1, 2);
    
    if (choice == 1) {
        int id = Console::readInt("Enter Product ID to search: ");
        int idx = findProductIndexById(id);
        if (idx == -1) {
            Console::setColor(Console::BOLD_RED);
            std::cout << "No product found with ID " << id << ".\n";
            Console::setColor(Console::RESET);
        } else {
            Console::printDivider();
            printProductTableHeader();
            printProductRow(products[idx], lowStockThreshold);
            Console::printDivider();
        }
    } else {
        std::string query = Console::readString("Enter search query: ");
        std::vector<Product> results;
        for (const auto& p : products) {
            if (isSubstringCaseInsensitive(p.name, query)) {
                results.push_back(p);
            }
        }
        
        if (results.empty()) {
            Console::setColor(Console::BOLD_RED);
            std::cout << "No products matching '" << query << "' found.\n";
            Console::setColor(Console::RESET);
        } else {
            Console::printDivider();
            printProductTableHeader();
            for (const auto& p : results) {
                printProductRow(p, lowStockThreshold);
            }
            Console::printDivider();
            std::cout << "Total matched items: " << results.size() << "\n";
        }
    }
    
    Console::pressEnterToContinue();
}

void Inventory::displayAll() const {
    Console::printHeader("Display Products List");
    if (products.empty()) {
        std::cout << "Inventory is empty.\n";
        Console::pressEnterToContinue();
        return;
    }
    
    std::cout << "Sorting Options:\n";
    std::cout << "1. Sort by ID (Ascending)\n";
    std::cout << "2. Sort by Name\n";
    std::cout << "3. Sort by Price (Low to High)\n";
    std::cout << "4. Sort by Price (High to Low)\n";
    std::cout << "5. Sort by Stock Quantity\n";
    std::cout << "6. Sort by Category\n";
    std::cout << "0. Default (Unsorted)\n";
    int choice = Console::readInt("Select sorting option (0-6): ", 0, 6);
    
    std::vector<Product> sortedList = products;
    if (choice == 1) {
        std::sort(sortedList.begin(), sortedList.end(), [](const Product& a, const Product& b) { return a.id < b.id; });
    } else if (choice == 2) {
        std::sort(sortedList.begin(), sortedList.end(), [](const Product& a, const Product& b) { return a.name < b.name; });
    } else if (choice == 3) {
        std::sort(sortedList.begin(), sortedList.end(), [](const Product& a, const Product& b) { return a.price < b.price; });
    } else if (choice == 4) {
        std::sort(sortedList.begin(), sortedList.end(), [](const Product& a, const Product& b) { return a.price > b.price; });
    } else if (choice == 5) {
        std::sort(sortedList.begin(), sortedList.end(), [](const Product& a, const Product& b) { return a.quantity < b.quantity; });
    } else if (choice == 6) {
        std::sort(sortedList.begin(), sortedList.end(), [](const Product& a, const Product& b) { return a.category < b.category; });
    }
    
    Console::clear();
    Console::printHeader("Inventory Listing");
    printProductTableHeader();
    for (const auto& p : sortedList) {
        printProductRow(p, lowStockThreshold);
    }
    Console::printDivider();
    std::cout << "Total distinct items: " << sortedList.size() << "\n";
    Console::pressEnterToContinue();
}

void Inventory::showLowStock() const {
    Console::printHeader("Low Stock Alert (<= " + std::to_string(lowStockThreshold) + " units)");
    
    std::vector<Product> lowStockItems;
    for (const auto& p : products) {
        if (p.quantity <= lowStockThreshold) {
            lowStockItems.push_back(p);
        }
    }
    
    if (lowStockItems.empty()) {
        Console::setColor(Console::BOLD_GREEN);
        std::cout << "All items are well-stocked. No low stock items detected!\n";
        Console::setColor(Console::RESET);
    } else {
        printProductTableHeader();
        for (const auto& p : lowStockItems) {
            printProductRow(p, lowStockThreshold);
        }
        Console::printDivider();
        std::cout << "Total low stock distinct items: " << lowStockItems.size() << "\n";
    }
    
    Console::pressEnterToContinue();
}

void Inventory::calculateTotalValue() const {
    Console::printHeader("Inventory Value Summary");
    if (products.empty()) {
        std::cout << "Inventory is empty.\n";
        Console::pressEnterToContinue();
        return;
    }
    
    double grandTotal = 0.0;
    int totalItems = 0;
    
    for (const auto& p : products) {
        grandTotal += (p.price * p.quantity);
        totalItems += p.quantity;
    }
    
    Console::setColor(Console::CYAN);
    std::cout << "Total Unique Products: " << products.size() << "\n";
    std::cout << "Total Quantity in Stock: " << totalItems << " units\n";
    std::cout << "-------------------------------------------\n";
    Console::setColor(Console::BOLD_GREEN);
    std::cout << "GRAND TOTAL INVENTORY VALUE: $" << std::fixed << std::setprecision(2) << grandTotal << "\n";
    Console::setColor(Console::RESET);
    
    Console::pressEnterToContinue();
}

void Inventory::saveToFile() const {
    Console::printHeader("Saving Data to File");
    
    // Save products
    std::ofstream outProd(filename);
    if (!outProd) {
        Console::setColor(Console::BOLD_RED);
        std::cout << "Error: Could not open file '" << filename << "' for writing.\n";
        Console::setColor(Console::RESET);
        Console::pressEnterToContinue();
        return;
    }
    
    outProd << products.size() << "\n";
    for (const auto& p : products) {
        outProd << p.id << "\n";
        outProd << p.name << "\n";
        outProd << p.category << "\n";
        outProd << p.price << "\n";
        outProd << p.quantity << "\n";
    }
    outProd.close();
    
    // Save transactions
    std::ofstream outTx(txFilename);
    if (outTx) {
        outTx << transactions.size() << "\n";
        for (const auto& tx : transactions) {
            outTx << tx.timestamp << "\n";
            outTx << tx.productId << "\n";
            outTx << tx.productName << "\n";
            outTx << tx.quantity << "\n";
            outTx << tx.priceEach << "\n";
            outTx << tx.totalAmount << "\n";
        }
        outTx.close();
    }
    
    Console::setColor(Console::BOLD_GREEN);
    std::cout << "Success: Data saved successfully to '" << filename << "' and '" << txFilename << "'.\n";
    Console::setColor(Console::RESET);
    Console::pressEnterToContinue();
}

void Inventory::loadFromFile() {
    Console::printHeader("Loading Data from File");
    
    // Clear current memory first
    products.clear();
    transactions.clear();
    
    // Load products
    std::ifstream inProd(filename);
    if (!inProd) {
        Console::setColor(Console::YELLOW);
        std::cout << "Warning: File '" << filename << "' not found. Starting with empty inventory.\n";
        Console::setColor(Console::RESET);
    } else {
        size_t size = 0;
        if (inProd >> size) {
            inProd.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            for (size_t i = 0; i < size; ++i) {
                Product p;
                if (!(inProd >> p.id)) break;
                inProd.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(inProd, p.name);
                std::getline(inProd, p.category);
                inProd >> p.price;
                inProd >> p.quantity;
                inProd.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                products.push_back(p);
            }
        }
        inProd.close();
        Console::setColor(Console::BOLD_GREEN);
        std::cout << "Loaded " << products.size() << " products.\n";
    }
    
    // Load transactions
    std::ifstream inTx(txFilename);
    if (inTx) {
        size_t size = 0;
        if (inTx >> size) {
            inTx.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            for (size_t i = 0; i < size; ++i) {
                Transaction tx;
                std::getline(inTx, tx.timestamp);
                inTx >> tx.productId;
                inTx.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(inTx, tx.productName);
                inTx >> tx.quantity;
                inTx >> tx.priceEach;
                inTx >> tx.totalAmount;
                inTx.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                transactions.push_back(tx);
            }
        }
        inTx.close();
        Console::setColor(Console::BOLD_GREEN);
        std::cout << "Loaded " << transactions.size() << " transactions.\n";
    }
    
    Console::setColor(Console::RESET);
    Console::pressEnterToContinue();
}

void Inventory::generateRandomProducts() {
    Console::printHeader("Generate Demo Products");
    
    struct Mock {
        std::string name;
        std::string category;
        double minPrice;
        double maxPrice;
    };
    
    std::vector<Mock> mocks = {
        {"MacBook Pro 16\"", "Electronics", 1999.00, 2499.00},
        {"iPhone 15 Pro", "Electronics", 999.00, 1199.00},
        {"Coffee Maker", "Appliances", 49.99, 149.99},
        {"Ergonomic Chair", "Furniture", 150.00, 399.00},
        {"Wireless Headphones", "Electronics", 80.00, 250.00},
        {"Standing Desk", "Furniture", 250.00, 600.00},
        {"Running Shoes", "Apparel", 60.00, 150.00},
        {"Stainless Steel Bottle", "Kitchen", 15.00, 35.00},
        {"Mechanical Keyboard", "Electronics", 50.00, 150.00},
        {"Yoga Mat", "Fitness", 19.99, 45.00}
    };
    
    // Seed using random device and engine
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> qtyDist(0, 15);  // Random quantities (some low stock)
    std::uniform_int_distribution<int> selectMocks(5, 8); // Choose 5 to 8 products to add
    
    int itemsToAdd = selectMocks(gen);
    int addedCount = 0;
    
    // Shuffle mocks to choose random ones
    std::shuffle(mocks.begin(), mocks.end(), gen);
    
    // Starting offset for unique IDs
    int startId = 101;
    while (findProductIndexById(startId) != -1) {
        startId++;
    }
    
    for (int i = 0; i < itemsToAdd; ++i) {
        const auto& m = mocks[i];
        std::uniform_real_distribution<double> priceDist(m.minPrice, m.maxPrice);
        
        Product p;
        p.id = startId++;
        p.name = m.name;
        p.category = m.category;
        p.price = priceDist(gen);
        p.quantity = qtyDist(gen);
        
        products.push_back(p);
        addedCount++;
    }
    
    Console::setColor(Console::BOLD_GREEN);
    std::cout << "Success: Generated and added " << addedCount << " random products for demo.\n";
    Console::setColor(Console::RESET);
    Console::pressEnterToContinue();
}

void Inventory::deleteProduct() {
    Console::printHeader("Delete Product");
    if (products.empty()) {
        std::cout << "Inventory is empty.\n";
        Console::pressEnterToContinue();
        return;
    }
    
    int id = Console::readInt("Enter Product ID to delete: ");
    int idx = findProductIndexById(id);
    if (idx == -1) {
        Console::setColor(Console::BOLD_RED);
        std::cout << "Error: Product ID " << id << " not found.\n";
        Console::setColor(Console::RESET);
        Console::pressEnterToContinue();
        return;
    }
    
    Product& p = products[idx];
    Console::setColor(Console::YELLOW);
    std::cout << "Are you sure you want to delete '" << p.name << "' (ID: " << p.id << ")? (y/n): ";
    Console::setColor(Console::BOLD_RED);
    
    std::string confirm;
    std::getline(std::cin, confirm);
    Console::setColor(Console::RESET);
    
    if (!confirm.empty() && (confirm[0] == 'y' || confirm[0] == 'Y')) {
        products.erase(products.begin() + idx);
        Console::setColor(Console::BOLD_GREEN);
        std::cout << "Success: Product successfully deleted.\n";
    } else {
        std::cout << "Deletion cancelled.\n";
    }
    
    Console::setColor(Console::RESET);
    Console::pressEnterToContinue();
}

void Inventory::showRestockSuggestions() const {
    Console::printHeader("Restock Suggestions");
    
    std::vector<Product> lowStockItems;
    for (const auto& p : products) {
        if (p.quantity <= lowStockThreshold) {
            lowStockItems.push_back(p);
        }
    }
    
    if (lowStockItems.empty()) {
        Console::setColor(Console::BOLD_GREEN);
        std::cout << "All items have healthy stock levels. No restock suggested at this time.\n";
        Console::setColor(Console::RESET);
        Console::pressEnterToContinue();
        return;
    }
    
    const int targetStockLevel = 25;
    
    Console::setColor(Console::BOLD_CYAN);
    std::cout << std::left 
              << std::setw(8)  << "ID"
              << std::setw(25) << "Name"
              << std::setw(15) << "Current Stock"
              << std::setw(15) << "Target Level"
              << "Suggested Restock Qty" << "\n";
    std::cout << std::string(75, '-') << "\n";
    Console::setColor(Console::RESET);
    
    double totalEstCost = 0.0;
    
    for (const auto& p : lowStockItems) {
        int suggest = targetStockLevel - p.quantity;
        double estCost = suggest * p.price;
        totalEstCost += estCost;
        
        std::cout << std::left
                  << std::setw(8)  << p.id
                  << std::setw(25) << (p.name.length() > 23 ? p.name.substr(0, 21) + ".." : p.name)
                  << std::setw(15) << p.quantity
                  << std::setw(15) << targetStockLevel;
        Console::setColor(Console::BOLD_YELLOW);
        std::cout << std::setw(22) << suggest;
        Console::setColor(Console::RESET);
        std::cout << "(Est. Cost: $" << std::fixed << std::setprecision(2) << estCost << ")\n";
    }
    
    Console::printDivider();
    Console::setColor(Console::CYAN);
    std::cout << "Total Estimated Cost to Restock: ";
    Console::setColor(Console::BOLD_GREEN);
    std::cout << "$" << totalEstCost << "\n";
    Console::setColor(Console::RESET);
    
    Console::pressEnterToContinue();
}

void Inventory::showTransactionHistory() const {
    Console::printHeader("Recent Transaction History (Last 10 Sales)");
    
    if (transactions.empty()) {
        std::cout << "No sales recorded yet.\n";
        Console::pressEnterToContinue();
        return;
    }
    
    Console::setColor(Console::BOLD_CYAN);
    std::cout << std::left
              << std::setw(22) << "Timestamp"
              << std::setw(8)  << "Prod ID"
              << std::setw(25) << "Product Name"
              << std::setw(8)  << "Qty"
              << std::setw(12) << "Price/Unit"
              << "Total Sale" << "\n";
    std::cout << std::string(85, '-') << "\n";
    Console::setColor(Console::RESET);
    
    double totalPeriodSales = 0.0;
    
    for (const auto& tx : transactions) {
        totalPeriodSales += tx.totalAmount;
        std::cout << std::left
                  << std::setw(22) << tx.timestamp
                  << std::setw(8)  << tx.productId
                  << std::setw(25) << (tx.productName.length() > 23 ? tx.productName.substr(0, 21) + ".." : tx.productName)
                  << std::setw(8)  << tx.quantity
                  << std::setw(12) << std::fixed << std::setprecision(2) << tx.priceEach
                  << "$" << tx.totalAmount << "\n";
    }
    
    Console::printDivider();
    Console::setColor(Console::CYAN);
    std::cout << "Total revenue of shown transactions: ";
    Console::setColor(Console::BOLD_GREEN);
    std::cout << "$" << totalPeriodSales << "\n";
    Console::setColor(Console::RESET);
    
    Console::pressEnterToContinue();
}
