#include "course_ops.h"
#include "filehandler.h"
#include "student_ops.h"
#include <iostream>
using namespace std; //c

EnrollResult enrollStudent(string roll, string courseCode, string semester) {
    vector<string> student = findRow("students.txt", 0, roll);
    if (student.empty() || student[4] != "active")
        return STUDENT_NOT_ACTIVE;

    vector<string> course = findRow("courses.txt", 0, courseCode);
    if (course.empty())
        return COURSE_NOT_FOUND;

    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    int enrolledCount = 0;
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][2] == courseCode && enrollments[i][5] == "active")
            enrolledCount++;
    }
    int totalSeats = stoi(course[4]);
    if (enrolledCount >= totalSeats)
        return NO_SEATS;

    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == roll && 
            enrollments[i][2] == courseCode && 
            enrollments[i][3] == semester && 
            enrollments[i][5] == "active") {
            return ALREADY_ENROLLED;
        }
    }

    int currentCredits = getCreditLoad(roll, semester);
    int newCourseCredits = stoi(course[2]);
    if (currentCredits + newCourseCredits > 21)
        return CREDIT_OVERLOAD;

    if (!checkPrerequisite(roll, courseCode))
        return PREREQ_NOT_MET;

    string newID = "E" + to_string(enrollments.size() + 1);
    vector<string> newRow = {newID, roll, courseCode, semester, "15-02-2024", "active"};
    appendTXT("enrollments.txt", newRow);
    
    return SUCCESS;
}

void dropCourse(string roll, string courseCode, string semester) {
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    bool found = false;
    int index = -1;

    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == roll && enrollments[i][2] == courseCode && 
            enrollments[i][3] == semester && enrollments[i][5] == "active") {
            index = i;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Error: No active enrollment found!" << endl;
        return;
    }

    char confirm;
    cout << "Are you sure you want to drop " << courseCode << "? (y/n): ";
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        enrollments[index][5] = "dropped";
        vector<string> header = {"enrollment_id", "roll_no", "course_code", "semester", "enrollment_date", "status"};
        writeTXT("enrollments.txt", header, enrollments);
        cout << "Course dropped successfully!" << endl;
    } else {
        cout << "Drop operation cancelled." << endl;
    }
}

int getCreditLoad(string roll, string semester) {
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    vector<vector<string>> courses = readTXT("courses.txt");
    int totalCredits = 0;

    // enrollments.txt layout: enrollment_id(0), roll_no(1), course_code(2), semester(3), enrollment_date(4), status(5)
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == roll && 
            enrollments[i][3] == semester && 
            enrollments[i][5] == "active") {
            
            for (int j = 0; j < courses.size(); j++) {
                if (courses[j][0] == enrollments[i][2]) {
                    totalCredits += stoi(courses[j][2]);
                }
            }
        }
    }
    return totalCredits;
}

bool checkPrerequisite(string roll, string courseCode) {
    vector<string> course = findRow("courses.txt", 0, courseCode);
    if (course.empty()) return false;

    string prereq = course[6]; 
    
    if (prereq == "NONE") return true; 

    vector<vector<string>> grades = readTXT("grades.txt");
    
    if (grades.empty()) return false;

    for (int i = 0; i < grades.size(); i++) {
        if (grades[i][0] == roll && 
            grades[i][1] == prereq && 
            grades[i][6] != "F") { 
            return true;
        }
    }
    return false;
}

vector<vector<string>> listEnrolledStudents(string courseCode) {
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    vector<vector<string>> result;

    // course_code is index 2, status is index 5 in enrollments.txt
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][2] == courseCode && 
            enrollments[i][5] == "active") {
            result.push_back(enrollments[i]);
        }
    }
    return result;
}

void printStudentCourses(string roll) {
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    cout << "\n--- Active Courses for " << roll << " ---" << endl;
    bool found = false;
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == roll && enrollments[i][5] == "active") {
            cout << "Course: " << enrollments[i][2] << " | Semester: " << enrollments[i][3] << endl;
            found = true;
        }
    }
    if (!found) cout << "No active courses found for this student." << endl;
    cout << "------------------------------------------" << endl;
}