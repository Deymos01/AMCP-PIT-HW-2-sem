#ifndef TASK_1_VALIDATOR_H
#define TASK_1_VALIDATOR_H

#include <iostream>

bool writeDataTimeOrDate(const char *data, int &index, const int &indexUntil, int &val, const char &nextSymbol);
bool isFebruaryError(const int &day, const int &month, const int &year);
void checkInputDate(const char *date, int &day, int &month, int &year);
void checkInputTime(const char *time, int &hours, int &minutes, int &seconds);
bool checkInputTicker(char *ticker);
bool checkInputType(char *typeOrder);
bool checkInputQuantity(char *quantityStr, int &quantity);
bool checkInputPrice(char *priceStr, double &price);

#endif //TASK_1_VALIDATOR_H
