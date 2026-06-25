#include "course_ops.h"
#include "filehandler.h"
#include "student_ops.h"
#include <iostream>
using namespace std;

EnrollResult enrollStudent(string roll, string courseCode, string semester) {
    
    // 1. Student active hai?
    vector<string> student = searchByRoll(roll);
    if (student.empty() || student[4] != "active")
        return STUDENT_NOT_ACTIVE;

    // 2. Course exist karta hai?
    vector<string> course = findRow("courses.txt", 0, courseCode);
    if (course.empty())
        return COURSE_NOT_FOUND;

    // 3. Seats available hain?
    // Enrolled count karo
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    int enrolledCount = 0;
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == courseCode && enrollments[i][3] == "enrolled")
            enrolledCount++;
    }
    int totalSeats = stoi(course[3]); // seats column
    if (enrolledCount >= totalSeats)
        return NO_SEATS;

    // 4. Already enrolled hai?
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][0] == roll && 
            enrollments[i][1] == courseCode && 
            enrollments[i][3] == "enrolled")
            return ALREADY_ENROLLED;
    }

    // 5. Credit load 21 se zyada nahi hona chahiye
    if (getCreditLoad(roll, semester) + stoi(course[2]) > 21)
        return CREDIT_OVERLOAD;

    // 6. Prerequisite check
    if (!checkPrerequisite(roll, courseCode))
        return PREREQ_NOT_MET;

    // Sab theek hai - enroll karo
    vector<string> newRow = {roll, courseCode, semester, "enrolled"};
    appendTXT("enrollments.txt", newRow);
    return SUCCESS;
}

void dropCourse(string roll, string courseCode, string semester) {
    
    // Check karo koi attendance toh nahi hai
    vector<vector<string>> attendance = readTXT("attendance_log.txt");
    for (int i = 0; i < attendance.size(); i++) {
        if (attendance[i][0] == roll && attendance[i][1] == courseCode) {
            cout << "Cannot drop! Attendance already marked." << endl;
            return;
        }
    }

    // Status dropped kar do
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][0] == roll && 
            enrollments[i][1] == courseCode && 
            enrollments[i][2] == semester) {
            enrollments[i][3] = "dropped";
        }
    }

    vector<string> header = {"roll", "course_code", "semester", "status"};
    writeTXT("enrollments.txt", header, enrollments);
    cout << "Course dropped successfully!" << endl;
}

int getCreditLoad(string roll, string semester) {
    
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    vector<vector<string>> courses = readTXT("courses.txt");
    int totalCredits = 0;

    // Har enrolled course ke credits add karo
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][0] == roll && 
            enrollments[i][2] == semester && 
            enrollments[i][3] == "enrolled") {
            
            // Is course ke credits dhundo
            for (int j = 0; j < courses.size(); j++) {
                if (courses[j][0] == enrollments[i][1]) {
                    totalCredits += stoi(courses[j][2]);
                }
            }
        }
    }
    return totalCredits;
}

bool checkPrerequisite(string roll, string courseCode) {
    
    // Course ka prereq field dekho
    vector<string> course = findRow("courses.txt", 0, courseCode);
    if (course.empty()) return false;

    string prereq = course[4]; // prereq column
    if (prereq == "NONE") return true; // koi prereq nahi

    // Grades file mein check karo - student ne prereq pass ki?
    vector<vector<string>> grades = readTXT("grades.txt");
    for (int i = 0; i < grades.size(); i++) {
        if (grades[i][0] == roll && 
            grades[i][1] == prereq && 
            grades[i][6] != "F") { // grade column
            return true;
        }
    }
    return false;
}

vector<vector<string>> listEnrolledStudents(string courseCode) {
    
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    vector<vector<string>> result;

    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == courseCode && 
            enrollments[i][3] == "enrolled") {
            result.push_back(enrollments[i]);
        }
    }
    return result;
}