#ifndef GRADES_H
#define GRADES_H

#include <string>
#include <vector>

using namespace std;

struct Stats {
    double highest;
    double lowest;
    double mean;
    double median;
};

void enterMarks(string roll, string courseCode, string semester, vector<double> quizzes, vector<double> assignments, double mid, double finalScore);
double bestThreeOfFive(vector<double> quizzes);
double computeWeightedTotal(double quizAvg, double asgnAvg, double mid, double finalScore);
string getLetterGrade(double total);
double computeGPA(string roll, string semester);
Stats computeClassStats(string courseCode);
void applyAttendancePenalty(string roll, string courseCode);

#endif