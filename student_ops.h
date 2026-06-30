#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include <string>
#include <vector> // c
using namespace std;

bool validateRollFormat(string roll);
bool addStudent(string roll, string name, string dept, string cgpa);
vector<string> searchByRoll(string roll);
vector<vector<string>> searchByName(string name);
void updateStudent(string roll, int fieldIndex, string newValue);
void softDelete(string roll);
vector<vector<string>> listActiveStudents();
void searchAsYouType();

#endif