#ifndef PERSON_H
#define PERSON_H

#include <iostream>

using namespace std;

class Person {
private:
    string firstName;
    string lastName;
    int age;
    string mobile;
    string email;

public:
    string getFirstName();

    string getLastName();

    int getAge();

    string getMobile();

    string getEmail();

    void setFirstName(string firstName);

    void setLastName(string lastName);

    void setAge(int age);

    void setMobile(string mobile);

    void setEmail(string email);
};


#endif
