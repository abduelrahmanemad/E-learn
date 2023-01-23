#include "config.h"

int currentAdminID = 1;

int currentCourseID = 10;

int currentStudentID = 200;

int currentStaffID = 300;

int currentAdmin = 1;

vector<string> halls = {"3201", "3101", "3001", "3102", "3002", "17200", "19200", "1125", "1313", "20106", "19301"};

string days[6] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday"};

string times[6] = {"08:00AM - 09:00AM", "09:15AM - 010:15AM", "10:45AM - 11:45AM", "12:00PM - 01:00PM", "01:30PM - 02:30PM",
                   "02:45PM - 03:45PM"};

string years[5] = {"Preparatory", "First", "Second", "Third", "Fourth"};

string genders[2] = {"Male", "Female"};

map<int, Course *> courses;

map<int, Student *> students;

map<int, Staff *> staffList;

map<int, Admin *> admins;

map<int, vector<string>> hallDates;

map<string, string> departments = {{"sbe", "Systems and Biomedical Engineering"},
                                   {"elc",  "Communication and Electronics Engineering"},
                                   {"cmp",  "Computer Engineering"},
                                   {"epe",  "Electrical Power Engineering"},
                                   {"mec",  "Mechanical Engineering"},
                                   {"cve",  "Civil Engineering"}};


map<char, string> positions = {{'p', "Professor"},
                               {'t', "Teaching Assistant"}};

statistcs *dashboard = new statistcs();

char answer = 'y';
