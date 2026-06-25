#ifndef REPORTS_H
#define REPORTS_H

#include <string>
#include <vector>

using namespace std;

void printMeritList();
void printAttendanceDefaulters();
void printFeeDefaulters(string currentDate);
void printSemesterResult(string roll, string semester);
void printDepartmentSummary();
void exportReportToFile(int reportType, string arg1, string arg2);

#endif