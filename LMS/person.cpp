#include "person.h"

string Person::getFirstName() {
    return firstName;
}

string Person::getLastName() {
    return lastName;
}

int Person::getAge() {
    return age;
}

string Person::getMobile() {
    return mobile;
}

string Person::getEmail() {
    return email;
}

void Person::setFirstName(string firstName) {
    this->firstName = firstName;
}

void Person::setLastName(string lastName) {
    this->lastName = lastName;
}

void Person::setAge(int age) {
    this->age = age;
}

void Person::setMobile(string mobile) {
    this->mobile = mobile;
}

void Person::setEmail(string email) {
    this->email = email;
}
