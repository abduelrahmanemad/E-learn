#ifndef STUDENT_H
#define STUDENT_H

#include <map>
#include "person.h"
#include "course.h"

using namespace std;

class Student : public Person {
private:
    int studentID;
    short grade;
    string department;
    string gender;
    map<int, int> studentCourses;

public:
    Student(int ID);

    ~Student();

    int getStudentID();

    map<int, int> getCourses();

    short getGrade();

    string getDepartment();

    string getGender();

    void setGrade(short grade);

    void setDepartment(string department);

    void setGender(string gender);

    void addCourse(int courseID, int grade);

    void removeCourse(int courseID);

    void showCourse();
};

#endif

