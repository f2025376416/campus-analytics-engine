#include "reports.h"
#include "filehandler.h"
#include "student_ops.h"
#include "attendance.h"
#include "fee_tracker.h"
#include "grades.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

void printMeritList() {
    vector<vector<string>> students = listActiveStudents();
    int n = students.size();
    
    // Bubble sort by CGPA (Descending) - Index 3 is CGPA [cite: 48, 78]
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (stod(students[j][3]) < stod(students[j+1][3])) {
                vector<string> temp = students[j];
                students[j] = students[j+1];
                students[j+1] = temp;
            }
        }
    }

    cout << "\n============= MERIT LIST =============" << endl;
    cout << left << setw(10) << "Rank" << setw(15) << "Roll" << setw(20) << "Name" << "CGPA" << endl;
    cout << "--------------------------------------" << endl;
    for (int i = 0; i < n; i++) {
        cout << left << setw(10) << (i + 1) 
             << setw(15) << students[i][0] 
             << setw(20) << students[i][1] 
             << students[i][3] << endl;
    }
    cout << "======================================\n" << endl;
}

void printAttendanceDefaulters() {
    vector<vector<string>> courses = readTXT("courses.txt");
    cout << "\n======= ATTENDANCE DEFAULTERS (< 75%) =======" << endl;
    cout << left << setw(15) << "Roll" << setw(20) << "Name" << setw(15) << "Course" << "Percentage" << endl;
    cout << "---------------------------------------------" << endl;

    bool foundAny = false;
    for (int i = 0; i < courses.size(); i++) {
        string courseCode = courses[i][0];
        vector<vector<string>> defaulters = getShortageList(courseCode); // Gets roll, course, pct [cite: 78]
        
        for (int j = 0; j < defaulters.size(); j++) {
            string roll = defaulters[j][0];
            vector<string> student = searchByRoll(roll);
            string name = student.empty() ? "Unknown" : student[1];
            
            cout << left << setw(15) << roll 
                 << setw(20) << name 
                 << setw(15) << courseCode 
                 << defaulters[j][2] << "%" << endl;
            foundAny = true;
        }
    }
    if (!foundAny) cout << "No attendance defaulters found!" << endl;
    cout << "=============================================\n" << endl;
}

void printFeeDefaulters(string currentDate) {
    vector<vector<string>> defaulters = getDefaulters(currentDate); // Calls from M6 [cite: 78]
    
    cout << "\n============= FEE DEFAULTERS =============" << endl;
    cout << left << setw(15) << "Roll" << setw(12) << "Semester" << setw(15) << "Overdue ($)" << "Weeks Late" << endl;
    cout << "------------------------------------------" << endl;

    for (int i = 0; i < defaulters.size(); i++) {
        string roll = defaulters[i][0];
        string semester = defaulters[i][1];
        string dueDate = defaulters[i][4];
        double outstanding = stod(defaulters[i].back()); // Outstanding amount added in M6
        int weeksLate = daysBetween(dueDate, currentDate) / 7; [cite: 72]
        
        if (weeksLate < 0) weeksLate = 0;

        cout << left << setw(15) << roll 
             << setw(12) << semester 
             << setw(15) << fixed << setprecision(2) << outstanding 
             << weeksLate << endl;
    }
    cout << "==========================================\n" << endl;
}

void printSemesterResult(string roll, string semester) {
    vector<vector<string>> grades = readTXT("grades.txt");
    vector<string> student = searchByRoll(roll);
    
    if (student.empty()) {
        cout << "Student not found!" << endl;
        return;
    }

    double gpa = computeGPA(roll, semester);

    cout << "\n+---------------------------------------------------+" << endl;
    cout << "|                 SEMESTER RESULT                   |" << endl;
    cout << "+---------------------------------------------------+" << endl;
    cout << "| Name: " << left << setw(43) << student[1] << " |" << endl;
    cout << "| Roll: " << left << setw(20) << roll << " Semester: " << setw(11) << semester << " |" << endl;
    cout << "| Semester GPA: " << left << setw(35) << fixed << setprecision(2) << gpa << " |" << endl;
    cout << "+---------------------------------------------------+" << endl;
    cout << "| " << left << setw(12) << "Course" << setw(10) << "Grade" << setw(25) << "Attendance (%)" << " |" << endl;
    cout << "+---------------------------------------------------+" << endl;

    bool hasCourses = false;
    for (int i = 0; i < grades.size(); i++) {
        if (grades[i][0] == roll && grades[i][2] == semester) {
            string courseCode = grades[i][1];
            string grade = grades[i][6];
            double attPct = getAttendancePct(roll, courseCode);
            
            cout << "| " << left << setw(12) << courseCode 
                 << setw(10) << grade 
                 << setw(25) << fixed << setprecision(2) << attPct << " |" << endl;
            hasCourses = true;
        }
    }
    
    if (!hasCourses) cout << "| No grades found for this semester.                |" << endl;
    cout << "+---------------------------------------------------+\n" << endl;
}

void printDepartmentSummary() {
    vector<vector<string>> students = readTXT("students.txt");
    
    // Parallel arrays for tracking [cite: 78]
    vector<string> depts;
    vector<int> counts;
    vector<double> cgpaSums;

    for (int i = 0; i < students.size(); i++) {
        if (students[i][4] != "active") continue; // only active
        
        string dept = students[i][2];
        double cgpa = stod(students[i][3]);
        
        bool found = false;
        for (int j = 0; j < depts.size(); j++) {
            if (depts[j] == dept) {
                counts[j]++;
                cgpaSums[j] += cgpa;
                found = true;
                break;
            }
        }
        if (!found) {
            depts.push_back(dept);
            counts.push_back(1);
            cgpaSums.push_back(cgpa);
        }
    }

    cout << "\n============== DEPARTMENT SUMMARY ==============" << endl;
    cout << left << setw(15) << "Department" << setw(15) << "Total Students" << "Avg CGPA" << endl;
    cout << "------------------------------------------------" << endl;
    
    for (int i = 0; i < depts.size(); i++) {
        double avg = cgpaSums[i] / counts[i];
        cout << left << setw(15) << depts[i] 
             << setw(15) << counts[i] 
             << fixed << setprecision(2) << avg << endl;
    }
    cout << "================================================\n" << endl;
}

// cout redirection magic [cite: 78]
void exportReportToFile(int reportType, string arg1, string arg2) {
    ofstream file("exported_report.txt");
    if (!file.is_open()) {
        cout << "Failed to create report file!" << endl;
        return;
    }

    // Save old cout buffer and redirect to file
    streambuf* coutbuf = cout.rdbuf(); 
    cout.rdbuf(file.rdbuf()); 

    if (reportType == 1) printMeritList();
    else if (reportType == 2) printAttendanceDefaulters();
    else if (reportType == 3) printFeeDefaulters(arg1);
    else if (reportType == 4) printSemesterResult(arg1, arg2);
    else if (reportType == 5) printDepartmentSummary();

    // Restore standard cout
    cout.rdbuf(coutbuf); 
    
    cout << "Report successfully exported to exported_report.txt" << endl;
}