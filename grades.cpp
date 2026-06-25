#include "grades.h"
#include "filehandler.h"
#include "attendance.h"
#include "student_ops.h"
#include <iostream>
#include <vector>

using namespace std;

// Bubble sort lagaya hai manual descending sorting ke liye kyunke STL <algorithm> mana hai
double bestThreeOfFive(vector<double> quizzes) {
    int n = quizzes.size();
    if (n == 0) return 0.0;
    
    // Agar 3 se kam hain toh unhi ka average le lo
    if (n < 3) {
        double sum = 0;
        for (int i = 0; i < n; i++) sum += quizzes[i];
        return sum / n;
    }

    // Manual Sort (Descending)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (quizzes[j] > quizzes[i]) {
                double temp = quizzes[i];
                quizzes[i] = quizzes[j];
                quizzes[j] = temp;
            }
        }
    }

    // Top 3 ka average
    return (quizzes[0] + quizzes[1] + quizzes[2]) / 3.0;
}

double computeWeightedTotal(double quizAvg, double asgnAvg, double mid, double finalScore) {
    return (quizAvg * 0.10) + (asgnAvg * 0.10) + (mid * 0.30) + (finalScore * 0.50);
}

string getLetterGrade(double total) {
    if (total >= 85) return "A";
    if (total >= 80) return "B+";
    if (total >= 70) return "B";
    if (total >= 65) return "C+";
    if (total >= 60) return "C";
    if (total >= 50) return "L"; // Assignment document specifically says L for >= 50
    return "F";
}

void enterMarks(string roll, string courseCode, string semester, vector<double> quizzes, vector<double> assignments, double mid, double finalScore) {
    // Validations
    if (mid < 0 || mid > 40) {
        cout << "Mid marks out of bounds!" << endl;
        return;
    }
    if (finalScore < 0 || finalScore > 60) {
        cout << "Final marks out of bounds!" << endl;
        return;
    }

    double quizAvg = bestThreeOfFive(quizzes);
    
    // Asgn avg manual computation
    double asgnAvg = 0;
    if (!assignments.empty()) {
        double sum = 0;
        for (int i = 0; i < assignments.size(); i++) sum += assignments[i];
        asgnAvg = sum / assignments.size();
    }

    double total = computeWeightedTotal(quizAvg, asgnAvg, mid, finalScore);
    string grade = getLetterGrade(total);

    // Save format: roll, course_code, semester, mid, final, total, grade
    vector<string> row = {
        roll, courseCode, semester, 
        to_string(mid), to_string(finalScore), 
        to_string(total), grade
    };
    
    appendTXT("grades.txt", row);
    cout << "Marks entered successfully! Total: " << total << ", Grade: " << grade << endl;
}

double computeGPA(string roll, string semester) {
    vector<vector<string>> grades = readTXT("grades.txt");
    vector<vector<string>> courses = readTXT("courses.txt");
    
    double totalPoints = 0;
    int totalCredits = 0;

    for (int i = 0; i < grades.size(); i++) {
        if (grades[i][0] == roll && grades[i][2] == semester) {
            string courseCode = grades[i][1];
            string grade = grades[i][6];
            
            // Get credit hours
            int credits = 0;
            for (int j = 0; j < courses.size(); j++) {
                if (courses[j][0] == courseCode) {
                    credits = stoi(courses[j][2]);
                    break;
                }
            }

            double gpaPoints = 0.0;
            if (grade == "A") gpaPoints = 4.0;
            else if (grade == "B+") gpaPoints = 3.33;
            else if (grade == "B") gpaPoints = 3.0;
            else if (grade == "C+") gpaPoints = 2.33;
            else if (grade == "C") gpaPoints = 2.0;
            else if (grade == "L") gpaPoints = 1.0;
            
            totalPoints += (gpaPoints * credits);
            totalCredits += credits;
        }
    }

    if (totalCredits == 0) return 0.0;
    return totalPoints / totalCredits;
}

Stats computeClassStats(string courseCode) {
    vector<vector<string>> grades = readTXT("grades.txt");
    vector<double> totals;

    for (int i = 0; i < grades.size(); i++) {
        if (grades[i][1] == courseCode) {
            totals.push_back(stod(grades[i][5])); // Total is at index 5
        }
    }

    Stats s = {0, 0, 0, 0};
    int n = totals.size();
    if (n == 0) return s;

    // Manual Sort for stats (Ascending)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (totals[j] < totals[i]) {
                double temp = totals[i];
                totals[i] = totals[j];
                totals[j] = temp;
            }
        }
    }

    s.lowest = totals[0];
    s.highest = totals[n - 1];
    
    double sum = 0;
    for (int i = 0; i < n; i++) sum += totals[i];
    s.mean = sum / n;

    // Median logic
    if (n % 2 == 0) {
        s.median = (totals[n / 2 - 1] + totals[n / 2]) / 2.0;
    } else {
        s.median = totals[n / 2];
    }

    return s;
}

void applyAttendancePenalty(string roll, string courseCode) {
    double pct = getAttendancePct(roll, courseCode);
    if (pct < 75.0) {
        vector<vector<string>> grades = readTXT("grades.txt");
        bool updated = false;

        for (int i = 0; i < grades.size(); i++) {
            if (grades[i][0] == roll && grades[i][1] == courseCode) {
                grades[i][6] = "F"; // Force grade F
                updated = true;
            }
        }

        if (updated) {
            vector<string> header = {"roll", "course_code", "semester", "mid", "final", "total", "grade"};
            writeTXT("grades.txt", header, grades);
            cout << "Penalty applied! Grade changed to F due to low attendance." << endl;
        }
    }
}