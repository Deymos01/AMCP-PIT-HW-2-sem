#include <iostream>
#include "order.h"

std::ostream &operator<<(std::ostream &out, Order &data) {
    out << data.getID() << ", ";

    out << data.getDate() << " " << data.getTime() << ", ";
    out << data.getTicker() << ", " << (data.getType() ? "buy" : "sell") << ", ";
    out << data.getQuantity() << ", ";
    out << std::fixed;
    out.precision(2);
    out << data.getPrice();
    return out;
}

std::ostream &operator<<(std::ostream &out, const Date &data) {
    out << data.getYear() << ".";
    out << ((data.getMonth() < 10) ? "0" : "") << data.getMonth() << ".";
    out << ((data.getDay() < 10) ? "0" : "") << data.getDay();
    return out;
}

std::ostream &operator<<(std::ostream &out, const Time &data) {
    out << ((data.getHours() < 10) ? "0" : "") << data.getHours() << ":";
    out << ((data.getMinutes() < 10) ? "0" : "") << data.getMinutes() << ":";
    out << ((data.getSeconds() < 10) ? "0" : "") << data.getSeconds();
    return out;
}