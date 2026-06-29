#include <iostream>
#include "Inventory.h"
#include "Console.h"

void displayMenu() {
    Console::clear();
    Console::setColor(Console::BOLD_CYAN);
    std::cout << "======================================================================\n";
    std::cout << "               INVENTORY MANAGEMENT SYSTEM (C++)\n";
    std::cout << "======================================================================\n";
    Console::setColor(Console::RESET);

    std::cout << " 1.  Add New Product\n";
    std::cout << " 2.  Update Stock Level\n";
    std::cout << " 3.  Sell Product (Deduct Stock)\n";
    std::cout << " 4.  Search Product (ID/Name)\n";
    std::cout << " 5.  Display All Products\n";
    std::cout << " 6.  Show Low Stock Items\n";
    std::cout << " 7.  Calculate Total Inventory Value\n";
    std::cout << " 8.  Generate Random Products (Demo)\n";
    std::cout << " 9.  Save to File\n";
    std::cout << " 10. Load from File\n";
    
    Console::setColor(Console::CYAN);
    std::cout << "---------------------- Intermediate Features -------------------------\n";
    Console::setColor(Console::RESET);
    std::cout << " 11. Delete Product\n";
    std::cout << " 12. View Restock Suggestions\n";
    std::cout << " 13. View Transaction History (Last 10 Sales)\n";
    
    Console::printDivider();
    Console::setColor(Console::BOLD_RED);
    std::cout << " 0.  Exit System\n";
    Console::setColor(Console::RESET);
    Console::printDivider();
}

int main() {
    Inventory inventory;
    
    // Auto-load inventory on startup
    std::cout << "Starting system... attempting to load database...\n";
    inventory.loadFromFile();
    
    int choice = -1;
    while (choice != 0) {
        displayMenu();
        choice = Console::readInt("Enter your choice (0-13): ", 0, 13);
        
        switch (choice) {
            case 1:
                inventory.addProduct();
                break;
            case 2:
                inventory.updateStock();
                break;
            case 3:
                inventory.sellProduct();
                break;
            case 4:
                inventory.searchProduct();
                break;
            case 5:
                inventory.displayAll();
                break;
            case 6:
                inventory.showLowStock();
                break;
            case 7:
                inventory.calculateTotalValue();
                break;
            case 8:
                inventory.generateRandomProducts();
                break;
            case 9:
                inventory.saveToFile();
                break;
            case 10:
                inventory.loadFromFile();
                break;
            case 11:
                inventory.deleteProduct();
                break;
            case 12:
                inventory.showRestockSuggestions();
                break;
            case 13:
                inventory.showTransactionHistory();
                break;
            case 0:
                Console::setColor(Console::BOLD_GREEN);
                std::cout << "\nThank you for using Inventory Management System. Goodbye!\n";
                Console::setColor(Console::RESET);
                break;
            default:
                break;
        }
    }
    
    return 0;
}
