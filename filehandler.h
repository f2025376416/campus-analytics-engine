#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
using namespace std;

vector<vector<string>> readTXT(string filename);
void writeTXT(string filename, vector<string> header, vector<vector<string>> data);
void appendTXT(string filename, vector<string> row);
vector<string> findRow(string filename, int colIndex, string value);
bool rowExists(string filename, int colIndex, string value);

#endif