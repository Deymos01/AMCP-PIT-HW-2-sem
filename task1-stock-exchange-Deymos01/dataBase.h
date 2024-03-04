#ifndef TASK1_STOCK_EXCHANGE_DEYMOS01_DATABASE_H
#define TASK1_STOCK_EXCHANGE_DEYMOS01_DATABASE_H

#include <cstring>
#include <iostream>
#include <fstream>
#include "order.h"
#include "validator.h"
#include "stringUtil.h"

class Node {
public:
    Node *nextNode{};
    Node *previousNode{};
    Order data_;
};

class DataBase {
private:
    Node *begin;
    Node *end;
public:
    int quantityOfOrder = 0;

    DataBase() {
        begin = nullptr;
        end = nullptr;
    }

    Node *getBegin() const {
        return this->begin;
    }

    void setBegin(Node *val) {
        this->begin = val;
    }

    Node *getEnd() const {
        return this->end;
    }

    void setEnd(Node *val) {
        this->end = val;
    }

	~DataBase() {
		Node *curr = this->begin;
		while (curr != nullptr){
			this->begin = curr->nextNode;
			delete curr;
			curr = this->begin;
		}
	}
};

void push(DataBase &DB, const Order &order);

void calculateNewQuantity(Order &order1, Order &order2);

void closeCrossOrders(DataBase &DB, Order &newOrder);

void addOrder(DataBase &DB, int id);

inline bool compByDateAndTime(const Order &left, const Order &right);

inline bool ascendingPrice(const Order &a, const Order &b);

inline bool descendingPrice(const Order &a, const Order &b);

inline bool descendingOrderBook(const Order &a, const Order &b);

void makeSort(DataBase &DB, bool(*comparator)(const Order &, const Order &));

void printListOfOrders(DataBase &DB);

void deleteOrder(DataBase &DB, Node *curr);

void deleteOrderByID(DataBase &DB, const int &id);

void deleteEmptyOrder(DataBase &DB);

void saveDataBase(const DataBase &DB, const char filename[], int &lastIndex);

void loadDataBase(DataBase &DB, const char filename[], int &lastIndex);

void exportDataBase(DataBase &DB, const char filename[]);

void makeOrderBook(DataBase &DB, char ticker[]);

void printOrderBook(DataBase &DB, char ticker[]);

#endif //TASK1_STOCK_EXCHANGE_DEYMOS01_DATABASE_H
