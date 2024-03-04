#include "dataBase.h"
#include "stringUtil.h"
#include "order.h"

void push(DataBase &DB, const Order &order) {
	Node *tmp = new Node{nullptr, nullptr, order};

	if (DB.getBegin() != nullptr) {
		DB.getEnd()->nextNode = tmp;
	} else {
		DB.setBegin(tmp);
	}
	tmp->previousNode = DB.getEnd();
	DB.setEnd(tmp);
	++DB.quantityOfOrder;
}

void calculateNewQuantity(Order &order1, Order &order2) {
	if (order1.getQuantity() >= order2.getQuantity()) {
		order1.setQuantity(order1.getQuantity() - order2.getQuantity());
		order2.setQuantity(0);
	} else {
		order2.setQuantity(order2.getQuantity() - order1.getQuantity());
		order1.setQuantity(0);
	}
}

void closeCrossOrders(DataBase &DB, Order &newOrder) {
	Node *curr = DB.getBegin();
	if (curr == nullptr) {
		return;
	}

	while (curr != nullptr) {
		if (newOrder.getQuantity() == 0) {
			return;
		}

		if (compareStrings(newOrder.getTicker(), curr->data_.getTicker(), true) &&
			curr->data_.getType() != newOrder.getType() &&
			curr->data_.getQuantity() != 0) {
			// Присутствует встречная заявка

			if (newOrder.getType() && (newOrder.getPrice() - curr->data_.getPrice() >= 0) ||
				!newOrder.getType() && (curr->data_.getPrice() - newOrder.getPrice() >= 0)) {
				calculateNewQuantity(newOrder, curr->data_);
			}
		}

		curr = curr->nextNode;
	}
}

void addOrder(DataBase &DB, int id) {
	char date[32];
	char time[32];
	char ticker[32];
	char type[32];
	char quantityStr[32];
	char priceStr[32];
	char buffer[1024];

	memset(date, '\0', 32);
	memset(time, '\0', 32);
	memset(ticker, '\0', 32);
	memset(type, '\0', 32);
	memset(quantityStr, '\0', 32);
	memset(priceStr, '\0', 32);

	int day = 0;
	int month = 0;
	int year = 0;

	int hours = 0;
	int minutes = 0;
	int seconds = 0;

	int quantity = 0;
	double price = 0;

	std::cin.ignore();
	std::cin.getline(date, 31, ' ');

	try {
		checkInputDate(date, day, month, year);
	} catch (int errorCode) {
		switch (errorCode) {
			case 0:
				std::cout << "Wrong date format: " << date << std::endl;
				break;
			case 1:
				std::cout << "Day value is invalid: " << day << std::endl;
				break;
			case 2:
				std::cout << "Month value is invalid: " << month << std::endl;
				break;
			default:
				std::cout << "Unexpected error" << std::endl;
				break;
		}
		std::cin.getline(buffer, 1024);
		return;
	}

	std::cin.getline(time, 31, ',');
	try {
		checkInputTime(time, hours, minutes, seconds);
	} catch (int errorCode) {
		switch (errorCode) {
			case 0:
				std::cout << "Wrong time format: " << time << std::endl;
				break;
			case 1:
				std::cout << "Time value is invalid: " << time << std::endl;
				break;
			default:
				std::cout << "Unexpected error" << std::endl;
				break;
		}
		std::cin.getline(buffer, 1024);
		return;
	}

	ignoreSpace();
	std::cin.getline(ticker, 31, ',');
	if (!checkInputTicker(ticker)) {
		std::cout << "Unknown command" << std::endl;
		std::cin.getline(buffer, 1024);
		return;
	}

	ignoreSpace();
	std::cin.getline(type, 31, ',');
	if (!checkInputType(type)) {
		std::cout << "Unknown command: " << type << std::endl;
		std::cin.getline(buffer, 1024);
		return;
	}

	ignoreSpace();
	std::cin.getline(quantityStr, 32, ',');
	if (!checkInputQuantity(quantityStr, quantity)) {
		std::cout << "Unknown command" << std::endl;
		std::cin.getline(buffer, 1024);
		return;
	}

	ignoreSpace();
	std::cin >> priceStr;
	if (!checkInputPrice(priceStr, price)) {
		std::cout << "Unknown command" << std::endl;
		std::cin.getline(buffer, 1024);
		return;
	}

	Order newOrder(id);
	newOrder.setDate(day, month, year);
	newOrder.setTime(hours, minutes, seconds);
	newOrder.setTicker(ticker);
	newOrder.setType(type);
	newOrder.setQuantity(quantity);
	newOrder.setPrice(price);

	if (!newOrder.getType()) {
		makeSort(DB, ascendingPrice);
	} else {
		makeSort(DB, descendingPrice);
	}

	closeCrossOrders(DB, newOrder);

	push(DB, newOrder);
}

// _________Comparators_____________

bool compByDateAndTime(const Order &left, const Order &right) {
	if (left.getDate().getJulianDate() == right.getDate().getJulianDate() &&
		left.getTime().getUnitTime() == right.getTime().getUnitTime()) {
		return left.getID() < right.getID();
	}

	return (left.getDate().getJulianDate() < right.getDate().getJulianDate() ||
			left.getTime().getUnitTime() < right.getTime().getUnitTime());
}

bool ascendingPrice(const Order &a, const Order &b) {
	return a.getPrice() > b.getPrice();
}

bool descendingPrice(const Order &a, const Order &b) {
	return a.getPrice() < b.getPrice();
}

bool descendingOrderBook(const Order &a, const Order &b) {
	if (a.getPrice() > b.getPrice()) {
		return true;
	}
	if (a.getPrice() < b.getPrice()) {
		return false;
	}
	if (a.getType() && !b.getType()) {
		return false;
	}
	if (!a.getType() && b.getType()) {
		return true;
	}
	if (!a.getType() && !b.getType()) {
		return !compByDateAndTime(a, b);
	}
	return compByDateAndTime(a, b);
}
//____________________________________

void makeSort(DataBase &DB, bool(*comparator)(const Order &, const Order &)) {
	Node *obj1 = DB.getBegin();
	Node *obj2;

	for (int i = 0; i < DB.quantityOfOrder - 1; ++i) {
		obj2 = obj1;
		for (int j = i + 1; j < DB.quantityOfOrder; ++j) {
			obj2 = obj2->nextNode;
			if (comparator(obj2->data_, obj1->data_)) {
				std::swap(obj2->data_, obj1->data_);
			}
		}
		obj1 = obj1->nextNode;
	}
}

void printListOfOrders(DataBase &DB) {
	Node *curr = DB.getBegin();

	if (curr == nullptr) {
		return;
	}

	makeSort(DB, compByDateAndTime);

	while (curr != nullptr) {
		std::cout << curr->data_ << std::endl;
		curr = curr->nextNode;
	}
}

void deleteOrder(DataBase &DB, Node *curr) {
	if (curr == DB.getBegin() && curr == DB.getEnd()) {
		DB.setBegin(nullptr);
		DB.setEnd(nullptr);
	} else if (curr == DB.getBegin()) {
		curr->nextNode->previousNode = nullptr;
		DB.setBegin(curr->nextNode);
	} else if (curr == DB.getEnd()) {
		curr->previousNode->nextNode = nullptr;
		DB.setEnd(curr->previousNode);
	} else {
		curr->previousNode->nextNode = curr->nextNode;
		curr->nextNode->previousNode = curr->previousNode;
	}
	--DB.quantityOfOrder;
	delete curr;
}

void deleteOrderByID(DataBase &DB, const int &id) {
	Node *curr = DB.getBegin();

	while ((curr != nullptr) && curr->data_.getID() != id) {
		curr = curr->nextNode;
	}

	if (curr == nullptr) {
		return;
	}

	deleteOrder(DB, curr);
}

void deleteEmptyOrder(DataBase &DB) {
	Node *curr = DB.getBegin();
	Node *tmp;
	while (curr != nullptr) {
		tmp = curr->nextNode;
		if (curr->data_.getQuantity() == 0) {
			deleteOrder(DB, curr);
		}
		curr = tmp;
	}
}

void saveDataBase(const DataBase &DB, const char filename[], int &lastIndex) {
	std::ofstream outFile(filename, std::ios_base::binary);
	if (!outFile.is_open()) {
		std::cout << "Unknown command" << std::endl;
		outFile.close();
		return;
	}

	outFile.write(reinterpret_cast<char *>(&lastIndex), sizeof(lastIndex));
	Node *curr = DB.getBegin();
	if (curr == nullptr) {
		outFile.close();
		return;
	}

	while (curr != nullptr) {
		outFile.write(reinterpret_cast<char *>(&curr->data_), sizeof(curr->data_));
		curr = curr->nextNode;
	}

	outFile.close();
}

void loadDataBase(DataBase &DB, const char filename[], int &lastIndex) {
	std::ifstream inFile(filename, std::ios_base::binary);
	if (!inFile.is_open()) {
		std::cout << "Unknown command" << std::endl;
		inFile.close();
		return;
	}

	DB = DataBase();
	inFile.read(reinterpret_cast<char *>(&lastIndex), sizeof(lastIndex));
	Order tmp;
	while (inFile.peek() != EOF) {
		inFile.read(reinterpret_cast<char *>(&tmp), sizeof(Order));
		push(DB, tmp);
	}
	inFile.close();
}

void exportDataBase(DataBase &DB, const char filename[]) {
	std::ofstream outFile(filename);
	if (!outFile.is_open()) {
		std::cout << "Unknown command" << std::endl;
		outFile.close();
		return;
	}
	Node *curr = DB.getBegin();
	if (curr == nullptr) {
		outFile.close();
		return;
	}
	makeSort(DB, compByDateAndTime);

	while (curr != nullptr) {

		outFile << curr->data_.getDate() << " " << curr->data_.getTime() << ", ";
		outFile << curr->data_.getTicker() << ", " << (curr->data_.getType() ? "buy" : "sell") << ", ";
		outFile << curr->data_.getQuantity() << ", ";
		outFile << std::fixed;
		outFile.precision(2);
		outFile << curr->data_.getPrice() << "\n";

		curr = curr->nextNode;
	}

	outFile.close();
}

void makeOrderBook(DataBase &DB, char ticker[]) {
	Node *curr = DB.getBegin();
	while (curr != nullptr) {
		if (compareStrings(ticker, curr->data_.getTicker(), true)) {
			std::cout << (curr->data_.getType() ? "buy" : "sell") << ", ";
			std::cout.precision(2);
			std::cout << std::fixed << curr->data_.getPrice() << ", ";
			std::cout << curr->data_.getQuantity() << ", ";
			std::cout << curr->data_.getDate() << " ";
			std::cout << curr->data_.getTime() << "\n";
		}
		curr = curr->nextNode;
	}
}

void printOrderBook(DataBase &DB, char ticker[]) {
	if (DB.getBegin() == nullptr) {
		return;
	}
	makeSort(DB, descendingOrderBook);
	makeOrderBook(DB, ticker);
}