#include "staff.h"
#include "config.h"

Staff::Staff(int ID) {
    staffID = ID;
    dashboard->staffNum++;
}

Staff::~Staff() {
    dashboard->staffNum--;
    if (position == 'p') {
        if (gender == "Male")
            dashboard->maleProfNum--;
        dashboard->profNum--;
    } else {
        if (gender == "Male")
            dashboard->maleTaNum--;
        dashboard->taNum--;
    }
    dashboard->departmentStaffNum[department]--;
    if (gender == "Male")
        dashboard->maleStaff--;
    else
        dashboard->femaleStaff--;
}

int Staff::getStaffID() {
    return staffID;
}

char Staff::getPosition() {
    return position;
}

vector<int> Staff::getCourses() {
    return staffCourses;
}

string Staff::getGender() {
    return gender;
}

string Staff::getDepartment() {
    return department;
}

void Staff::setPosition(char position) {
    this->position = position;
    if (position == 'p') {
        if (gender == "Male")
            dashboard->maleProfNum++;
        dashboard->profNum++;
    } else {
        if (gender == "Male")
            dashboard->maleTaNum++;
        dashboard->taNum++;
    }
}

void Staff::setDepartment(string department) {
    this->department = department;
    dashboard->departmentStaffNum[department]++;
}

void Staff::setGender(string gender) {
    this->gender = gender;
    if (gender == "Male")
        dashboard->maleStaff++;
    else
        dashboard->femaleStaff++;
}

void Staff::addCourse(int courseID) {
    Course *addedCourse = courses[courseID];
    staffCourses.push_back(courseID);
    addedCourse->staff.push_back(staffID);
}

void Staff::removeCourse(int courseID) {
    Course *removedCourse = courses[courseID];
    for (int i = 0; i < int(staffCourses.size()); i++) {
        if (staffCourses[i] == 0)
            continue;;
        if (staffCourses[i] == courseID)
            staffCourses[i] = 0;
    }
    for (int i = 0; i < int(removedCourse->staff.size()); i++) {
        if (removedCourse->staff[i] == 0)
            continue;
        if (removedCourse->staff[i] == staffID)
            removedCourse->staff[i] = 0;
    }
}
