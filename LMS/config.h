#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <bits/stdc++.h>
#include "course.h"
#include "student.h"
#include "staff.h"
#include "admin.h"

using namespace std;

extern int currentAdminID;
extern int currentCourseID;
extern int currentStudentID;
extern int currentStaffID;
extern int currentAdmin;
extern vector<string> halls;
extern string days[6];
extern string times[6];
extern string years[5];
extern string genders[2];
extern map<int, Course *> courses;
extern map<int, Student *> students;
extern map<int, Staff *> staffList;
extern map<int, Admin *> admins;

extern char answer;

extern map<int, vector<string>> hallDates;
extern map<string, string> departments;

extern map<char, string> positions;

struct statistcs {
    int studentsNum;
    int maleStudents;
    int femaleStudents;
    map<string, int> departmentStudentsNum;
    map<int, int> yearsStudentsNum;
    int staffNum;
    int maleStaff;
    int femaleStaff;
    int profNum;
    int taNum;
    int maleProfNum;
    int maleTaNum;
    int coursesNum;
    map<string, int> departmentCoursesNum;
    map<string, int> departmentStaffNum;
};

extern statistcs *dashboard;

#endif
