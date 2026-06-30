#include "fee_tracker.h"
#include "filehandler.h"
#include <iostream>
#include <iomanip>
#include <cmath> // c

using namespace std;

bool validateDate(string date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return false;
    }
    return true;
}

int monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int countTotalDays(int d, int m, int y) {
    long int n = y * 365 + d;
    for (int i = 0; i < m - 1; i++) {
        n += monthDays[i];
    }
    int leaps = y / 4 - y / 100 + y / 400;
    if (m <= 2 && (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0))) {
        leaps--;
    }
    n += leaps;
    return n;
}

int daysBetween(string date1, string date2) {
    int d1 = stoi(date1.substr(0, 2)), m1 = stoi(date1.substr(3, 2)), y1 = stoi(date1.substr(6, 4));
    int d2 = stoi(date2.substr(0, 2)), m2 = stoi(date2.substr(3, 2)), y2 = stoi(date2.substr(6, 4));

    int total1 = countTotalDays(d1, m1, y1);
    int total2 = countTotalDays(d2, m2, y2);

    return total2 - total1; 
}

double computeLateFine(string dueDate, string paidDate, double amountDue) {
    if (paidDate == "NONE" || paidDate.empty()) return 0.0;
    
    int daysLate = daysBetween(dueDate, paidDate);
    if (daysLate <= 0) return 0.0;

    int weeks = daysLate / 7;
    return weeks * 0.02 * amountDue;
}

void recordPayment(string roll, string semester, string date, double amount) {
    if (!validateDate(date)) {
        cout << "Invalid date format! Use DD-MM-YYYY." << endl;
        return;
    }

    // fees.txt layout: fee_id(0), roll_no(1), semester(2), total_fee(3),
    // amount_paid(4), due_date(5), payment_date(6), payment_method(7), status(8)
    vector<vector<string>> fees = readTXT("fees.txt");
    bool found = false;

    for (int i = 0; i < fees.size(); i++) {
        if (fees[i][1] == roll && fees[i][2] == semester) {
            double paidSoFar = stod(fees[i][4]);
            fees[i][4] = to_string(paidSoFar + amount);
            fees[i][6] = date;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Fee record not found for this student and semester!" << endl;
        return;
    }

    vector<string> header = {"fee_id", "roll_no", "semester", "total_fee", "amount_paid", "due_date", "payment_date", "payment_method", "status"};
    writeTXT("fees.txt", header, fees);
    cout << "Payment recorded successfully!" << endl;
}

void generateReceipt(string roll, string semester) {
    vector<vector<string>> fees = readTXT("fees.txt");
    
    for (int i = 0; i < fees.size(); i++) {
        if (fees[i][1] == roll && fees[i][2] == semester) {
            double due = stod(fees[i][3]);
            double paid = stod(fees[i][4]);
            string dueDate = fees[i][5];
            string paidDate = fees[i][6];

            double fine = computeLateFine(dueDate, paidDate, due);
            double totalDue = due + fine;
            double balance = totalDue - paid;

            cout << "\n======================================" << endl;
            cout << setfill(' ') << setw(25) << "FEE RECEIPT" << endl;
            cout << setfill('-') << setw(38) << "-" << setfill(' ') << endl;
            cout << left << setw(20) << "Roll Number:" << roll << endl;
            cout << left << setw(20) << "Semester:" << semester << endl;
            cout << setfill('-') << setw(38) << "-" << setfill(' ') << endl;
            cout << left << setw(20) << "Tuition Fee:" << "$" << fixed << setprecision(2) << due << endl;
            cout << left << setw(20) << "Late Fine:" << "$" << fine << endl;
            cout << setfill('-') << setw(38) << "-" << setfill(' ') << endl;
            cout << left << setw(20) << "Total Due:" << "$" << totalDue << endl;
            cout << left << setw(20) << "Amount Paid:" << "$" << paid << endl;
            cout << left << setw(20) << "Remaining Balance:" << "$" << balance << endl;
            cout << "======================================\n" << endl;
            return;
        }
    }
    cout << "No fee record found to generate receipt!" << endl;
}

vector<vector<string>> getDefaulters(string currentDate) {
    vector<vector<string>> fees = readTXT("fees.txt");
    vector<vector<string>> defaulters;

    for (int i = 0; i < fees.size(); i++) {
        double due = stod(fees[i][3]);
        double paid = stod(fees[i][4]);
        string dueDate = fees[i][5];

        if ((due - paid) > 0 && daysBetween(dueDate, currentDate) > 0) {
            vector<string> record = fees[i];
            record.push_back(to_string(due - paid)); 
            defaulters.push_back(record);
        }
    }

    int n = defaulters.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            double amount1 = stod(defaulters[j].back());
            double amount2 = stod(defaulters[j+1].back());
            
            if (amount1 < amount2) {
                vector<string> temp = defaulters[j];
                defaulters[j] = defaulters[j+1];
                defaulters[j+1] = temp;
            }
        }
    }

    return defaulters;
}