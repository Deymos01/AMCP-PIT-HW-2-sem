#ifndef TASK1_STOCK_EXCHANGE_DEYMOS01_ORDER_H
#define TASK1_STOCK_EXCHANGE_DEYMOS01_ORDER_H

#include <cstring>
#include <iostream>
#include "stringUtil.h"

class Date {
private:
	int day_ = 0;
	int month_ = 0;
	int year_ = 0;
public:
	void setDay(int day) {
		this->day_ = day;
	}

	int getDay() const {
		return this->day_;
	}

	void setMonth(int month) {
		this->month_ = month;
	}

	int getMonth() const {
		return this->month_;
	}

	void setYear(int year) {
		this->year_ = year;
	}

	int getYear() const {
		return this->year_;
	}

	int getJulianDate() const {
		int a = (14 - this->month_) / 12;
		int y = this->year_ + 4800 - a;
		int m = this->month_ + 12 * a - 3;
		return this->day_ + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
	}
};

class Time {
private:
	int hours_ = 0;
	int minutes_ = 0;
	int seconds_ = 0;
public:
	void setHours(int hours) {
		this->hours_ = hours;
	}

	int getHours() const {
		return this->hours_;
	}

	void setMinutes(int minutes) {
		this->minutes_ = minutes;
	}

	int getMinutes() const {
		return this->minutes_;
	}

	void setSeconds(int seconds) {
		this->seconds_ = seconds;
	}

	int getSeconds() const {
		return this->seconds_;
	}

	int getUnitTime() const {
		return this->hours_ * 60 * 60 + this->minutes_ * 60 + this->seconds_;
	}
};

class Order {
private:
	int idOfOrder;
	char ticker_[16];
	int quantityOfStock;
	double priceOfStock;
	Date date;
	Time time;
	bool typeOfOrder; //false - sell, true - buy
public:
	Order() {
		idOfOrder = 0;
		quantityOfStock = 0;
		priceOfStock = 0;
		typeOfOrder = false;
	}

	Order(int id) {
		idOfOrder = id;
		memset(ticker_, '\0', 16);
		quantityOfStock = 0;
		priceOfStock = 0;
		typeOfOrder = false;
	}

	int getID() const {
		return this->idOfOrder;
	}

	void setDate(int day, int month, int year) {
		this->date.setDay(day);
		this->date.setMonth(month);
		this->date.setYear(year);
	}

	Date getDate() const {
		return this->date;
	}

	void setTime(int hours, int minutes, int seconds) {
		this->time.setHours(hours);
		this->time.setMinutes(minutes);
		this->time.setSeconds(seconds);
	}

	Time getTime() const {
		return this->time;
	}

	void setTicker(char *ticker) {
		for (int i = 0; i < strlen(ticker); ++i){
			this->ticker_[i] = ticker[i];
		}
	}

	char *getTicker() {
		return this->ticker_;
	}

	void setType(char *type) {
		this->typeOfOrder = strcmp(type, "buy") == 0;
	}

	bool getType() const {
		return this->typeOfOrder;
	}

	void setQuantity(int quantity) {
		this->quantityOfStock = quantity;
	}

	int getQuantity() const {
		return this->quantityOfStock;
	}

	void setPrice(double price) {
		this->priceOfStock = price;
	}

	double getPrice() const {
		return this->priceOfStock;
	}
};

std::ostream &operator<<(std::ostream &out, Order &data);

std::ostream &operator<<(std::ostream &out, const Date &data);

std::ostream &operator<<(std::ostream &out, const Time &data);

#endif //TASK1_STOCK_EXCHANGE_DEYMOS01_ORDER_H
