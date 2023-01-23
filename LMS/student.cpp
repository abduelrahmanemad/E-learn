#include "student.h"
#include "config.h"

Student::Student(int ID) {
    studentID = ID;
    dashboard->studentsNum++;
}

Student::~Student() {
    dashboard->studentsNum--;
    dashboard->yearsStudentsNum[grade]--;
    dashboard->departmentStudentsNum[department]--;
    if (gender == "Male")
        dashboard->maleStudents--;
    else
        dashboard->femaleStudents--;
}

int Student::getStudentID() {
    return studentID;
}

short Student::getGrade() {
    return grade;
}

string Student::getDepartment() {
    return department;
}

string Student::getGender() {
    return gender;
}

void Student::setGrade(short grade) {
    this->grade = grade;
    dashboard->yearsStudentsNum[grade]++;
}

void Student::setDepartment(string department) {
    this->department = department;
    dashboard->departmentStudentsNum[department]++;
}

void Student::setGender(string gender) {
    this->gender = gender;
    if (gender == "Male")
        dashboard->maleStudents++;
    else
        dashboard->femaleStudents++;
}

void Student::addCourse(int courseID, int grade) {
    Course *addedCourse = courses[courseID];
    studentCourses[courseID] = grade;
    addedCourse->totalStudents++;
    addedCourse->totalGrades += grade;
    addedCourse->averageGrade = float(addedCourse->totalGrades) / float(addedCourse->totalStudents);
    if (grade >= (0.5 * float(addedCourse->getCourseGrade())))
        addedCourse->passedStudents++;
    addedCourse->passRatio = (float(addedCourse->passedStudents) / float(addedCourse->totalStudents)) * 100;
    addedCourse->students[studentID] = grade;
}

void Student::removeCourse(int courseID) {
    Course *removedCourse = courses[courseID];
    int grade = studentCourses[courseID];
    studentCourses.erase(courseID);
    removedCourse->students.erase(studentID);

    removedCourse->totalStudents--;
    removedCourse->totalGrades -= grade;
    if (removedCourse->totalStudents > 0)
        removedCourse->averageGrade = float(removedCourse->totalGrades) / float(removedCourse->totalStudents);
    else
        removedCourse->averageGrade = 0;
    if (grade >= (0.5 * float(removedCourse->getCourseGrade()))) {
        removedCourse->passedStudents--;}
    if (removedCourse->totalStudents > 0)
        removedCourse->passRatio = (float(removedCourse->passedStudents) / float(removedCourse->totalStudents)) * 100;
    else
        removedCourse->passRatio = 0;
}

map<int, int> Student::getCourses() {
    return studentCourses;
}

void Student::showCourse() {
    for (auto const &p: studentCourses) {
        std::cout << p.first << ' ' << p.second << '\n';
    }
}
