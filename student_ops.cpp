#include "student_ops.h"
#include "filehandler.h"
#include <iostream>
#include <cstdlib>
#ifdef _WIN32
#include <conio.h>
#endif
using namespace std;

bool validateRollFormat(string roll) {
    if (roll.length() != 11) return false;
    if (roll.substr(0, 5) != "BSAI-") return false;
    if (!isdigit(roll[5]) || !isdigit(roll[6])) return false;
    if (roll[7] != '-') return false;
    if (!isdigit(roll[8]) || !isdigit(roll[9]) || !isdigit(roll[10])) return false;
    return true;
}

bool addStudent(string roll, string name, string dept, string cgpa) {
    if (!validateRollFormat(roll)) {
        cout << "Invalid roll format! Use BSAI-YY-XXX" << endl;
        return false;
    }
    if (rowExists("students.txt", 0, roll)) {
        cout << "Student already exists!" << endl;
        return false;
    }
    for (int i = 0; i < name.length(); i++) {
        if (isdigit(name[i])) {
            cout << "Name cannot contain digits!" << endl;
            return false;
        }
    }
    float cgpaVal = stof(cgpa);
    if (cgpaVal < 0.0 || cgpaVal > 4.0) {
        cout << "CGPA must be between 0.0 and 4.0!" << endl;
        return false;
    }
    vector<string> row = {roll, name, dept, cgpa, "active"};
    appendTXT("students.txt", row);
    cout << "Student added successfully!" << endl;
    return true;
}

vector<string> searchByRoll(string roll) {
    vector<string> student = findRow("students.txt", 0, roll);
    if (student.empty()) {
        cout << "Student not found!" << endl;
        return student;
    }
    cout << "\n--- Student Details ---" << endl;
    cout << "Roll:   " << student[0] << "\nName:   " << student[1] << "\nStatus: " << student[4] << endl;
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    cout << "\nActive Courses:" << endl;
    bool found = false;
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == roll && enrollments[i][5] == "active") {
            cout << "- " << enrollments[i][2] << " (Sem: " << enrollments[i][3] << ")" << endl;
            found = true;
        }
    }
    if (!found) cout << "No active courses found." << endl;
    cout << "-----------------------" << endl;
    return student;
}

vector<vector<string>> searchByName(string name) {
    vector<vector<string>> data = readTXT("students.txt");
    vector<vector<string>> results;
    for (int i = 0; i < data.size(); i++) {
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
    vector<string> student = findRow("students.txt", 0, roll);
    if (student.empty()) {
        cout << "Student not found!" << endl;
        return;
    }
    if (student[4] == "active") {
        updateStudent(roll, 4, "inactive");
        cout << "Student account has been DEACTIVATED (Soft-Deleted)!\n";
    } else {
        updateStudent(roll, 4, "active");
        cout << "Student account has been REACTIVATED successfully!\n";
    }
}

vector<vector<string>> listActiveStudents() {
    vector<vector<string>> data = readTXT("students.txt");
    vector<vector<string>> active;
    for (int i = 0; i < data.size(); i++) {
        if (data[i][4] == "active") {
            active.push_back(data[i]);
        }
    }
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

// Bonus: Search-as-you-type. Reads one character per loop iteration and
// re-filters/reprints the matching student list after every keystroke.
// Prefix matching is done purely with substr() and length comparisons --
// no STL algorithms, no regex.
void searchAsYouType() {
    vector<vector<string>> data = readTXT("students.txt");
    string query = "";
    char ch;
    bool done = false;

    cout << "\n=== SEARCH AS YOU TYPE ===\n";
    cout << "Type a name. Backspace deletes a char. Enter finishes.\n\n";

    while (!done) {
#ifdef _WIN32
        ch = _getch();   // true single-keypress read, no Enter needed
#else
        ch = cin.get();  // portable fallback; console line-buffers until Enter,
                          // but the loop still processes and filters one
                          // character at a time exactly as required
#endif

        if (ch == '\r' || ch == '\n') {
            done = true;
            break;
        } else if (ch == 8 || ch == 127) { // backspace / delete
            if (query.length() > 0) {
                query = query.substr(0, query.length() - 1);
            }
        } else {
            query += ch;
        }

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        cout << "=== SEARCH AS YOU TYPE ===\n";
        cout << "Query: " << query << "_\n\n";
        cout << left;
        cout.width(15); cout << "Roll";
        cout.width(20); cout << "Name";
        cout << "Status" << endl;
        cout << "------------------------------------------\n";

        bool any = false;
        for (int i = 0; i < (int)data.size(); i++) {
            string name = data[i][1];

            // Prefix match: only valid if query isn't longer than the name
            if (query.length() <= name.length()) {
                string prefix = name.substr(0, query.length());
                if (prefix == query) {
                    cout.width(15); cout << data[i][0];
                    cout.width(20); cout << data[i][1];
                    cout << data[i][4] << endl;
                    any = true;
                }
            }
        }
        if (!any) cout << "(no matches)\n";
    }

    cout << "\nSearch finished. Final query: \"" << query << "\"\n";
}