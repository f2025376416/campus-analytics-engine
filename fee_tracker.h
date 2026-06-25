#ifndef FEE_TRACKER_H
#define FEE_TRACKER_H

#include <string>
#include <vector>

using namespace std;

bool validateDate(string date);
int countTotalDays(int d, int m, int y);
int daysBetween(string date1, string date2);
double computeLateFine(string dueDate, string paidDate, double amountDue);
void recordPayment(string roll, string semester, string date, double amount);
void generateReceipt(string roll, string semester);
vector<vector<string>> getDefaulters(string currentDate);

#endif