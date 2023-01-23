#ifndef STAFF_H
#define STAFF_H

#include <vector>
#include <map>
//#include <bits/stdc++.h>
#include "person.h"
#include "course.h"

class Staff : public Person {
private:
    int staffID;
    char position;
    string department;
    string gender;
    vector<int> staffCourses;

public:
    Staff(int id);

    ~Staff();

    int getStaffID();

    char getPosition();

    string getDepartment();

    string getGender();

    vector<int> getCourses();

    void setPosition(char position);

    void setDepartment(string department);

    void setGender(string gender);

    void addCourse(int courseID);

    void removeCourse(int courseID);

    void showCourse();
};

#endif
