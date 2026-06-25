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

void studentMenu() {
    int choice;
    while (true) {
        cout << "\n--- LEVEL 3: STUDENT OPERATIONS ---\n";
        cout << "1. Add Student\n2. Search by Roll\n3. Soft Delete\n0. Back\nChoice: ";
        cin >> choice;
        if (choice == 0) break;

        string roll, name, dept, cgpa;
        switch (choice) {
            case 1:
                cout << "Roll (BSAI-YY-XXX): "; cin >> roll;
                cout << "Name (One word): "; cin >> name;
                cout << "Dept: "; cin >> dept;
                cout << "CGPA: "; cin >> cgpa;
                addStudent(roll, name, dept, cgpa);
                break;
            case 2:
                cout << "Roll: "; cin >> roll;
                searchByRoll(roll);
                break;
            case 3:
                cout << "Roll to delete: "; cin >> roll;
                softDelete(roll);
                break;
            default: cout << "Invalid choice!\n";
        }
    }
}

void courseMenu() {
    int choice;
    while (true) {
        cout << "\n--- LEVEL 3: COURSE OPERATIONS ---\n";
        cout << "1. Enroll Student\n2. Drop Course\n0. Back\nChoice: ";
        cin >> choice;
        if (choice == 0) break;

        string roll, course, sem;
        switch (choice) {
            case 1:
                cout << "Roll: "; cin >> roll;
                cout << "Course Code: "; cin >> course;
                cout << "Semester: "; cin >> sem;
                enrollStudent(roll, course, sem);
                break;
            case 2:
                cout << "Roll: "; cin >> roll;
                cout << "Course Code: "; cin >> course;
                cout << "Semester: "; cin >> sem;
                dropCourse(roll, course, sem);
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