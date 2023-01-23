#include "course.h"
#include "config.h"

Course::Course(int ID) {
    courseID = ID;
    dashboard->coursesNum++;
}

Course::~Course() {
    dashboard->departmentCoursesNum[department]--;
    dashboard->coursesNum--;
}

string Course::getCourseName() {
    return courseName;
}

int Course::getCourseID() {
    return courseID;
}

int Course::getCourseGrade() {
    return totalGrade;
}

string Course::getLectureHall() {
    return lectureHall;
}

string Course::getCourseDepartment() {
    return department;
}

short Course::getLectureDay() {
    return lectureDay;
}

short Course::getLectureTime() {
    return lectureTime;
}

void Course::setCourseName(string courseName) {
    this->courseName = courseName;
}

void Course::setCourseGrade(int grade) {
    totalGrade = grade;
}

void Course::setLectureHall(string lecHall) {
    lectureHall = lecHall;
}

void Course::setLectureTime(short lecTime) {
    lectureTime = lecTime;
}

void Course::setLectureDay(short lecDay) {
    lectureDay = lecDay;
}

void Course::setCourseDepartment(string department) {
    this->department = department;
    dashboard->departmentCoursesNum[department]++;
}
