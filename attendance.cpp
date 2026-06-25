#include "attendance.h"
#include "filehandler.h"
#include "course_ops.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Backup vector - undo ke liye
vector<vector<string>> attendanceBackup;

void markAttendance(string courseCode, string date, string semester) {

    // Backup pehle save karo (undo ke liye)
    attendanceBackup = readTXT("attendance_log.txt");

    // Is course ke enrolled students lo
    vector<vector<string>> enrolled = listEnrolledStudents(courseCode);

    if (enrolled.empty()) {
        cout << "No students enrolled in this course!" << endl;
        return;
    }

    // Har student ka status lo
    for (int i = 0; i < enrolled.size(); i++) {
        string roll = enrolled[i][0];
        string status = "";

        // Jab tak valid input na aaye
        while (status != "P" && status != "A" && status != "L") {
            cout << "Student " << roll << " - Enter P/A/L: ";
            cin >> status;
            // Capital karo
            if (status == "p") status = "P";
            if (status == "a") status = "A";
            if (status == "l") status = "L";
        }

        // File mein save karo
        vector<string> row = {roll, courseCode, date, status};
        appendTXT("attendance_log.txt", row);
    }

    cout << "Attendance marked successfully!" << endl;
}

double getAttendancePct(string roll, string courseCode) {

    vector<vector<string>> data = readTXT("attendance_log.txt");

    int total = 0;
    double present = 0;

    for (int i = 0; i < data.size(); i++) {
        if (data[i][0] == roll && data[i][1] == courseCode) {
            total++;
            if (data[i][3] == "P")      present += 1.0;
            else if (data[i][3] == "L") present += 0.5; // Late = half present
        }
    }

    if (total == 0) return 0.0;
    return (present / total) * 100.0;
}

vector<vector<string>> getShortageList(string courseCode) {

    // Is course ke sab enrolled students lo
    vector<vector<string>> enrolled = listEnrolledStudents(courseCode);
    vector<vector<string>> shortage;

    for (int i = 0; i < enrolled.size(); i++) {
        string roll = enrolled[i][0];
        double pct = getAttendancePct(roll, courseCode);

        if (pct < 75.0) {
            // Roll + course + percentage save karo
            vector<string> row = {roll, courseCode, to_string(pct)};
            shortage.push_back(row);
        }
    }

    return shortage;
}

bool undoLastSession(string courseCode, string date) {

    // Backup exist karta hai?
    if (attendanceBackup.empty()) {
        cout << "No backup available to undo!" << endl;
        return false;
    }

    // Backup se restore karo
    vector<string> header = {"roll", "course_code", "date", "status"};
    writeTXT("attendance_log.txt", header, attendanceBackup);

    cout << "Last session undone successfully!" << endl;
    return true;
}

void printDailySheet(string courseCode, string date) {

    vector<vector<string>> data = readTXT("attendance_log.txt");

    cout << "\n===== Daily Attendance Sheet =====" << endl;
    cout << "Course: " << courseCode << " | Date: " << date << endl;
    cout << "----------------------------------" << endl;
    cout << setw(15) << "Roll" 
         << setw(10) << "Status" << endl;
    cout << "----------------------------------" << endl;

    bool found = false;
    for (int i = 0; i < data.size(); i++) {
        if (data[i][1] == courseCode && data[i][2] == date) {
            cout << setw(15) << data[i][0] 
                 << setw(10) << data[i][3] << endl;
            found = true;
        }
    }

    if (!found) cout << "No attendance found for this date!" << endl;
    cout << "==================================\n" << endl;
}#include "attendance.h"
#include "filehandler.h"
#include "course_ops.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Backup vector - undo ke liye
vector<vector<string>> attendanceBackup;

void markAttendance(string courseCode, string date, string semester) {

    // Backup pehle save karo (undo ke liye)
    attendanceBackup = readTXT("attendance_log.txt");

    // Is course ke enrolled students lo
    vector<vector<string>> enrolled = listEnrolledStudents(courseCode);

    if (enrolled.empty()) {
        cout << "No students enrolled in this course!" << endl;
        return;
    }

    // Har student ka status lo
    for (int i = 0; i < enrolled.size(); i++) {
        string roll = enrolled[i][0];
        string status = "";

        // Jab tak valid input na aaye
        while (status != "P" && status != "A" && status != "L") {
            cout << "Student " << roll << " - Enter P/A/L: ";
            cin >> status;
            // Capital karo
            if (status == "p") status = "P";
            if (status == "a") status = "A";
            if (status == "l") status = "L";
        }

        // File mein save karo
        vector<string> row = {roll, courseCode, date, status};
        appendTXT("attendance_log.txt", row);
    }

    cout << "Attendance marked successfully!" << endl;
}

double getAttendancePct(string roll, string courseCode) {

    vector<vector<string>> data = readTXT("attendance_log.txt");

    int total = 0;
    double present = 0;

    for (int i = 0; i < data.size(); i++) {
        if (data[i][0] == roll && data[i][1] == courseCode) {
            total++;
            if (data[i][3] == "P")      present += 1.0;
            else if (data[i][3] == "L") present += 0.5; // Late = half present
        }
    }

    if (total == 0) return 0.0;
    return (present / total) * 100.0;
}

vector<vector<string>> getShortageList(string courseCode) {

    // Is course ke sab enrolled students lo
    vector<vector<string>> enrolled = listEnrolledStudents(courseCode);
    vector<vector<string>> shortage;

    for (int i = 0; i < enrolled.size(); i++) {
        string roll = enrolled[i][0];
        double pct = getAttendancePct(roll, courseCode);

        if (pct < 75.0) {
            // Roll + course + percentage save karo
            vector<string> row = {roll, courseCode, to_string(pct)};
            shortage.push_back(row);
        }
    }

    return shortage;
}

bool undoLastSession(string courseCode, string date) {

    // Backup exist karta hai?
    if (attendanceBackup.empty()) {
        cout << "No backup available to undo!" << endl;
        return false;
    }

    // Backup se restore karo
    vector<string> header = {"roll", "course_code", "date", "status"};
    writeTXT("attendance_log.txt", header, attendanceBackup);

    cout << "Last session undone successfully!" << endl;
    return true;
}

void printDailySheet(string courseCode, string date) {

    vector<vector<string>> data = readTXT("attendance_log.txt");

    cout << "\n===== Daily Attendance Sheet =====" << endl;
    cout << "Course: " << courseCode << " | Date: " << date << endl;
    cout << "----------------------------------" << endl;
    cout << setw(15) << "Roll" 
         << setw(10) << "Status" << endl;
    cout << "----------------------------------" << endl;

    bool found = false;
    for (int i = 0; i < data.size(); i++) {
        if (data[i][1] == courseCode && data[i][2] == date) {
            cout << setw(15) << data[i][0] 
                 << setw(10) << data[i][3] << endl;
            found = true;
        }
    }

    if (!found) cout << "No attendance found for this date!" << endl;
    cout << "==================================\n" << endl;
}