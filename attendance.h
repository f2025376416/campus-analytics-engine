#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <string>
#include <vector>
using namespace std;

void markAttendance(string courseCode, string date, string semester);
double getAttendancePct(string roll, string courseCode);
vector<vector<string>> getShortageList(string courseCode);
bool undoLastSession(string courseCode, string date);
void printDailySheet(string courseCode, string date);

#endif