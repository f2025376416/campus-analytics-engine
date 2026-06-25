#include "student_ops.h"
#include "filehandler.h"
#include <iostream>
using namespace std;

// BSAI-YY-XXX format validate karta hai
bool validateRollFormat(string roll) {
    // Length check: BSAI-24-001 = 10 characters
    if (roll.length() != 11) return false;

    // "BSAI-" check
    if (roll.substr(0, 5) != "BSAI-") return false;

    // YY check (2 digits)
    if (!isdigit(roll[5]) || !isdigit(roll[6])) return false;

    // "-" check
    if (roll[7] != '-') return false;

    // XXX check (3 digits)
    if (!isdigit(roll[8]) || !isdigit(roll[9]) || !isdigit(roll[10])) return false;

    return true;
}

bool addStudent(string roll, string name, string dept, string cgpa) {
    // Roll format validate karo
    if (!validateRollFormat(roll)) {
        cout << "Invalid roll format! Use BSAI-YY-XXX" << endl;
        return false;
    }

    // Duplicate check
    if (rowExists("students.txt", 0, roll)) {
        cout << "Student already exists!" << endl;
        return false;
    }

    // Name mein digits nahi hone chahiye
    for (int i = 0; i < name.length(); i++) {
        if (isdigit(name[i])) {
            cout << "Name cannot contain digits!" << endl;
            return false;
        }
    }

    // CGPA range check 0.0 to 4.0
    float cgpaVal = stof(cgpa);
    if (cgpaVal < 0.0 || cgpaVal > 4.0) {
        cout << "CGPA must be between 0.0 and 4.0!" << endl;
        return false;
    }

    // File mein add karo
    vector<string> row = {roll, name, dept, cgpa, "active"};
    appendTXT("students.txt", row);
    cout << "Student added successfully!" << endl;
    return true;
}

vector<string> searchByRoll(string roll) {
    vector<string> student = findRow("students.txt", 0, roll);
    if (student.empty()) {
        cout << "Student not found!" << endl;
    } else {
        // We added this else block to actually print the data
        cout << "\n--- Student Details ---" << endl;
        cout << "Roll:   " << student[0] << endl;
        cout << "Name:   " << student[1] << endl;
        cout << "Dept:   " << student[2] << endl;
        cout << "CGPA:   " << student[3] << endl;
        cout << "Status: " << student[4] << endl;
        cout << "-----------------------" << endl;
    }
    return student;
} // added functionality so that if student exists, his info stays

vector<vector<string>> searchByName(string name) {
    vector<vector<string>> data = readTXT("students.txt");
    vector<vector<string>> results;

    for (int i = 0; i < data.size(); i++) {
        // Name mein substring dhundo
        if (data[i][1].find(name) != string::npos) {
            results.push_back(data[i]);
        }
    }

    if (results.empty()) {
        cout << "No students found with name: " << name << endl;
    }
    return results;
}

void updateStudent(string roll, int fieldIndex, string newValue) {
    vector<vector<string>> data = readTXT("students.txt");
    bool found = false;

    for (int i = 0; i < data.size(); i++) {
        if (data[i][0] == roll) {
            data[i][fieldIndex] = newValue;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Student not found!" << endl;
        return;
    }

    vector<string> header = {"roll", "name", "dept", "cgpa", "status"};
    writeTXT("students.txt", header, data);
    cout << "Student updated successfully!" << endl;
}

void softDelete(string roll) {
    // First, check if the student actually exists
    if (!rowExists("students.txt", 0, roll)) {
        cout << "Cannot delete. Student not found!" << endl;
        return; // Stop the function right here
    }
    
    // If they exist, update their status
    updateStudent(roll, 4, "inactive");
    
    // Note: We removed the "Student deactivated" cout because 
    // updateStudent already prints "Student updated successfully!" 
} // it doesn't directly delete, just deactivates their account

vector<vector<string>> listActiveStudents() {
    vector<vector<string>> data = readTXT("students.txt");
    vector<vector<string>> active;

    // Sirf active students lo
    for (int i = 0; i < data.size(); i++) {
        if (data[i][4] == "active") {
            active.push_back(data[i]);
        }
    }

    // Selection sort by roll number
    for (int i = 0; i < active.size() - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < active.size(); j++) {
            if (active[j][0] < active[minIdx][0]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            vector<string> temp = active[i];
            active[i] = active[minIdx];
            active[minIdx] = temp;
        }
    }

    return active;
}