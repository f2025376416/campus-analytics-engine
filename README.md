# Campus Analytics Engine

[cite_start]A multi-file, menu-driven data analytics system for a fictional university campus built entirely in C++ (Programming Fundamentals Project)[cite: 6, 7].

## Features
* [cite_start]**Student Operations:** Add, search, update, and soft-delete student records[cite: 59].
* [cite_start]**Course Operations:** Enroll/drop courses, validate credit loads (max 21 hrs), and check prerequisites[cite: 46, 62].
* [cite_start]**Attendance:** Mark daily attendance (P/A/L), calculate percentages, and generate `<75%` shortage lists[cite: 46, 67].
* [cite_start]**Grades & GPA:** Calculate best 3 of 5 quizzes, weighted totals, assign letter grades, and compute semester GPA[cite: 70].
* [cite_start]**Fee Tracker:** Record payments, calculate late fines manually, and generate formatted receipts[cite: 46, 72].
* [cite_start]**Reports:** Generate merit lists, defaulter reports, department summaries, and export data to TXT files[cite: 78].

## Compilation & Execution
This project strictly follows the constraints of using foundational C++ constructs.

### How to Compile:
Open your terminal in the project directory and run:
```bash
g++ *.cpp -o main
./main