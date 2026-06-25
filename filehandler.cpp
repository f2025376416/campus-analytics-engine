#include "filehandler.h"
#include <fstream>
#include <iostream>
using namespace std;

// Line ko comma pe split karta hai
vector<string> parseLine(string line) {
    vector<string> fields;
    string field = "";
    bool inQuotes = false;

    for (int i = 0; i < line.length(); i++) {
        if (line[i] == '"') {
            inQuotes = !inQuotes;
        } else if (line[i] == ',' && !inQuotes) {
            fields.push_back(field);
            field = "";
        } else {
            field += line[i];
        }
    }
    fields.push_back(field); // last field
    return fields;
}

// File padhta hai, pehli line skip (header)
vector<vector<string>> readTXT(string filename) {
    vector<vector<string>> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return data;
    }

    string line;
    getline(file, line); // header skip

    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> row = parseLine(line);
        data.push_back(row);
    }

    file.close();
    return data;
}

// File mein likhta hai (overwrite)
void writeTXT(string filename, vector<string> header, vector<vector<string>> data) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return;
    }

    // Header likho
    for (int i = 0; i < header.size(); i++) {
        if (i != 0) file << ",";
        file << header[i];
    }
    file << "\n";

    // Data likho
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (j != 0) file << ",";
            // Agar comma hai field mein toh quotes mein wrap karo
            if (data[i][j].find(',') != string::npos) {
                file << '"' << data[i][j] << '"';
            } else {
                file << data[i][j];
            }
        }
        file << "\n";
    }

    file.close();
}

// File mein ek row add karta hai (append)
void appendTXT(string filename, vector<string> row) {
    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return;
    }

    for (int i = 0; i < row.size(); i++) {
        if (i != 0) file << ",";
        if (row[i].find(',') != string::npos) {
            file << '"' << row[i] << '"';
        } else {
            file << row[i];
        }
    }
    file << "\n";

    file.close();
}

// Specific column mein value dhundta hai, row return karta hai
vector<string> findRow(string filename, int colIndex, string value) {
    vector<vector<string>> data = readTXT(filename);

    for (int i = 0; i < data.size(); i++) {
        if (colIndex < data[i].size() && data[i][colIndex] == value) {
            return data[i];
        }
    }

    return vector<string>(); // empty agar nahi mila
}

// Check karta hai ke value exist karti hai ya nahi
bool rowExists(string filename, int colIndex, string value) {
    vector<string> row = findRow(filename, colIndex, value);
    return !row.empty();
}