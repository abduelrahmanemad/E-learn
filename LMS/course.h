#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Course {
private:
    int courseID;
    string courseName;
    int totalGrade;
    string lectureHall;
    short lectureDay;
    short lectureTime;
    string department;

public:
    float totalGrades = 0;
    int totalStudents = 0;
    float averageGrade = 0;
    int passedStudents = 0;
    float passRatio = 0;
    map<int, int> students;
    vector<int> staff;

    Course(int ID);

    ~Course();

    string getCourseName();

    int getCourseID();

    int getCourseGrade();

    string getLectureHall();

    short getLectureDay();

    short getLectureTime();

    string getCourseDepartment();

    void setCourseName(string courseName);

    void setCourseGrade(int grade);

    void setLectureHall(string lecHall);

    void setLectureDay(short lecDay);

    void setLectureTime(short lecTime);

    void setCourseDepartment(string department);
};


#endif
