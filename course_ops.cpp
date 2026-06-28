#include "course_ops.h"
#include "filehandler.h"
#include "student_ops.h"
#include <iostream>
using namespace std;

EnrollResult enrollStudent(string roll, string courseCode, string semester) {
    // 1. Student active hai? (Using findRow instead of searchByRoll so it stays silent)
    vector<string> student = findRow("students.txt", 0, roll);
    if (student.empty() || student[4] != "active")
        return STUDENT_NOT_ACTIVE;

    // 2. Course exist karta hai?
    vector<string> course = findRow("courses.txt", 0, courseCode);
    if (course.empty())
        return COURSE_NOT_FOUND;

    // 3. Seats available hain?
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    int enrolledCount = 0;
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][2] == courseCode && enrollments[i][5] == "active") // index 2 is course, index 5 is status
            enrolledCount++;
    }
    int totalSeats = stoi(course[4]); // capacity column index is 4 in courses.txt
    if (enrolledCount >= totalSeats)
        return NO_SEATS;

    // 4. Already enrolled hai?
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == roll && 
            enrollments[i][2] == courseCode && 
            enrollments[i][3] == semester && 
            enrollments[i][5] == "active") {
            return ALREADY_ENROLLED;
        }
    }

    // 5. Credit Overload Check (Max 21)
    int currentCredits = getCreditLoad(roll, semester);
    int newCourseCredits = stoi(course[2]);
    if (currentCredits + newCourseCredits > 21)
        return CREDIT_OVERLOAD;

    // 6. Prerequisite Check
    if (!checkPrerequisite(roll, courseCode))
        return PREREQ_NOT_MET;

    // 7. Success - Append to file
    // Generate a simple ID
    string newID = "E" + to_string(enrollments.size() + 1);
    // Columns: enrollment_id, roll_no, course_code, semester, enrollment_date, status
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

    // Confirmation Prompt
    char confirm;
    cout << "Are you sure you want to drop " << courseCode << "? (y/n): ";
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        enrollments[index][5] = "dropped"; // Update status
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
    // 1. Course ka data nikaal
    vector<string> course = findRow("courses.txt", 0, courseCode);
    if (course.empty()) return false;

    // BUG FIX: Prerequisite column is at index 6!
    // (0:code, 1:name, 2:credits, 3:instructor, 4:capacity, 5:enrolled, 6:prereq)
    string prereq = course[6]; 
    
    // Agar course ka prereq NONE hai, toh seedha pass karo
    if (prereq == "NONE") return true; 

    // 2. Agar prereq hai, toh grades file mein check karo
    vector<vector<string>> grades = readTXT("grades.txt");
    
    // Agar grades file mein kuch nahi, toh definitely prereq pass nahi kiya
    if (grades.empty()) return false;

    // Grades array mein dhoondo (roll_no=0, course=1, grade=6)
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

    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == courseCode && 
            enrollments[i][3] == "enrolled") {
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
        // [1] = roll, [2] = course, [3] = sem, [5] = status
        if (enrollments[i][1] == roll && enrollments[i][5] == "active") {
            cout << "Course: " << enrollments[i][2] << " | Semester: " << enrollments[i][3] << endl;
            found = true;
        }
    }
    if (!found) cout << "No active courses found for this student." << endl;
    cout << "------------------------------------------" << endl;
} // just printing all courses