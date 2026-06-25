#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include <string>
#include <vector>
using namespace std;

enum EnrollResult {
    SUCCESS,
    STUDENT_NOT_ACTIVE,
    COURSE_NOT_FOUND,
    NO_SEATS,
    ALREADY_ENROLLED,
    CREDIT_OVERLOAD,
    PREREQ_NOT_MET
};

EnrollResult enrollStudent(string roll, string courseCode, string semester);
void dropCourse(string roll, string courseCode, string semester);
int getCreditLoad(string roll, string semester);
bool checkPrerequisite(string roll, string courseCode);
vector<vector<string>> listEnrolledStudents(string courseCode);

#endif