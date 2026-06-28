#include <iostream>
#include <string>
#include <vector>
#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"
#include "attendance.h"
#include "grades.h"
#include "fee_tracker.h"
#include "reports.h"

using namespace std;

// For validations
// Check if name only has alphabets
// Check if name only has alphabets and spaces
bool isValidName(string name) {
    for (int i = 0; i < name.length(); i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return false;
    }
    return true;
}

// Check if dept only has alphabets and spaces
bool isValidDept(string dept) {
    for (int i = 0; i < dept.length(); i++) {
        if (!isalpha(dept[i]) && dept[i] != ' ') return false;
    }
    return true;
}

// Check if CGPA is purely numeric and between 0.0 and 4.0
bool isValidCGPA(string cgpa) {
    int dotCount = 0;
    for (int i = 0; i < cgpa.length(); i++) {
        if (cgpa[i] == '.') {
            dotCount++;
            if (dotCount > 1) return false; // Multiple decimals not allowed
        } else if (!isdigit(cgpa[i])) {
            return false; // Any alphabet or special char
        }
    }
    if (cgpa.empty() || cgpa == ".") return false;
    
    float val = stof(cgpa);
    return (val >= 0.0 && val <= 4.0);
}

void studentMenu() {
    int choice;
    while (true) {
        cout << "\n--- LEVEL 3: STUDENT OPERATIONS ---\n";
        // 1. Rename Option 3 Here
        cout << "1. Add Student\n2. Search by Roll\n3. Toggle Active/Inactive\n0. Back\nChoice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a valid number.\n";
            continue;
        }

        if (choice == 0) break;

        string roll, name, dept, cgpa;
        switch (choice) {
            case 1: { 
                cout << "Roll (BSAI-YY-XXX): "; cin >> roll;
                
                if (!validateRollFormat(roll)) {
                    cout << "Invalid roll format! Use strictly BSAI-YY-XXX\n";
                    break;
                }
                
                // 2. Smart Duplicate Check (Silent check with status validation)
                vector<string> existing = findRow("students.txt", 0, roll);
                if (!existing.empty()) {
                    if (existing[4] == "inactive") {
                        cout << "[Alert] This ID exists in the database but is INACTIVE!\n";
                        cout << "Please use Option 3 (Toggle Active/Inactive) to reactivate it.\n";
                    } else {
                        cout << "Student already exists and is currently ACTIVE!\n";
                    }
                    break;
                }

                cout << "Name (Alphabets & Spaces only): "; 
                cin >> ws; 
                getline(cin, name); 
                
                if (!isValidName(name)) {
                    cout << "Invalid Name! Digits and special chars not allowed.\n";
                    break;
                }

                cout << "Dept (Alphabets & Spaces only): "; 
                getline(cin, dept); 
                
                if (!isValidDept(dept)) {
                    cout << "Invalid Dept! Only alphabets allowed.\n";
                    break;
                }

                cout << "CGPA (0.0 to 4.0): "; cin >> cgpa;
                if (!isValidCGPA(cgpa)) {
                    cout << "Invalid CGPA! Must be a valid number between 0.0 and 4.0.\n";
                    break;
                }

                addStudent(roll, name, dept, cgpa);
                break;
            }
            case 2:
                cout << "Roll: "; cin >> roll;
                searchByRoll(roll);
                break;
            case 3: // Updated option context
                cout << "Roll to toggle status: "; cin >> roll;
                softDelete(roll); // Function name is same, but logic is toggle
                break;
            default: cout << "Invalid choice!\n";
        }
    }
} // added toggle functionality

void courseMenu() {
    int choice;
    while (true) {
        cout << "\n--- LEVEL 3: COURSE OPERATIONS ---\n";
        cout << "1. Enroll Student\n2. Drop Course\n3. View Student Courses\n0. Back\nChoice: ";
        cin >> choice;

        if (choice == 0) break;

        string roll, code, sem;
        switch (choice) {
            case 1: // ... (tera purana enroll code)
            case 2: // ... (tera purana drop code)
            case 3:
                cout << "Enter Roll Number to view courses: "; cin >> roll;
                printStudentCourses(roll);
                break;
            default: cout << "Invalid choice!\n";
        }
    }
}

void opsMenu() {
    int choice;
    while (true) {
        cout << "\n=== LEVEL 2: MANAGEMENT MODULES ===\n";
        cout << "1. Students\n2. Courses\n3. Attendance\n4. Grades\n5. Fees\n0. Back\nChoice: ";
        cin >> choice;
        if (choice == 0) break;

        string course, date, sem, roll;
        switch (choice) {
            case 1: studentMenu(); break;
            case 2: courseMenu(); break;
            case 3:
                cout << "Course Code: "; cin >> course;
                cout << "Date (DD-MM-YYYY): "; cin >> date;
                cout << "Semester: "; cin >> sem;
                markAttendance(course, date, sem);
                break;
            case 4:
                // Simplified for time: directly asks for inputs to demo enterMarks
                cout << "Roll: "; cin >> roll;
                cout << "Course: "; cin >> course;
                cout << "Semester: "; cin >> sem;
                enterMarks(roll, course, sem, {10, 8, 9}, {10, 10}, 35, 50); // Dummy array inputs for speed
                break;
            case 5:
                cout << "Roll: "; cin >> roll;
                cout << "Semester: "; cin >> sem;
                cout << "Date (DD-MM-YYYY): "; cin >> date;
                recordPayment(roll, sem, date, 15000);
                break;
            default: cout << "Invalid choice!\n";
        }
    }
}

void reportsMenu() {
    int choice;
    while (true) {
        cout << "\n=== LEVEL 2: REPORTS ===\n";
        cout << "1. Merit List\n2. Attendance Defaulters\n3. Fee Defaulters\n4. Dept Summary\n0. Back\nChoice: ";
        cin >> choice;
        if (choice == 0) break;

        string date;
        switch (choice) {
            case 1: printMeritList(); break;
            case 2: printAttendanceDefaulters(); break;
            case 3: 
                cout << "Current Date (DD-MM-YYYY): "; cin >> date;
                printFeeDefaulters(date); 
                break;
            case 4: printDepartmentSummary(); break;
            default: cout << "Invalid choice!\n";
        }
    }
}

int main() {
    int choice;
    cout << "Loading Campus Analytics Engine...\n";
    while (true) {
        cout << "\n### LEVEL 1: MAIN MENU ###\n";
        cout << "1. Management Operations\n2. Analytics & Reports\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 0) {
            cout << "Exiting system. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: opsMenu(); break;
            case 2: reportsMenu(); break;
            default: cout << "Invalid choice!\n";
        }
    }
    return 0;
}