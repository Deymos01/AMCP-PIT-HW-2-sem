#include <iostream>
#include <cstring>
#include "dataBase.h"

int main() {
    DataBase DB = DataBase();
    char command[32];
    char filename[128];
    int id = 1;
    char ticker[16];
    char buffer[1024];

    while (true) {
        std::cout << "> ";
        memset(command, '\0', 32);
        std::cin >> command;

        if (strcmp(command, "load") == 0) {
            std::cin >> filename;
            loadDataBase(DB, filename, id);

        } else if (strcmp(command, "save") == 0) {
            std::cin >> filename;
            saveDataBase(DB, filename, id);

        } else if (strcmp(command, "add") == 0) {
            addOrder(DB, id++);

        } else if (strcmp(command, "delete") == 0) {
            int deleteId;
            std::cin >> deleteId;
            deleteOrderByID(DB, deleteId);

        } else if (strcmp(command, "clear") == 0) {
            deleteEmptyOrder(DB);

        } else if (strcmp(command, "print") == 0) {
            printListOfOrders(DB);

        } else if (strcmp(command, "dom") == 0) {
            memset(ticker, '\0', 16);
            std::cin >> ticker;
            printOrderBook(DB, ticker);

        } else if (strcmp(command, "export") == 0) {
            std::cin >> filename;
            exportDataBase(DB, filename);

        } else if (strcmp(command, "quit") == 0) {
            return 0;
        } else {
            std::cout << "Unknown command: " << command << std::endl;
            if (std::cin.peek() != '\n'){
                std::cin.getline(buffer, 1024);
            }
        }
    }
}