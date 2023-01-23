#include "admin.h"

Admin::Admin(int ID)
{
    adminID = ID;
}

string Admin::getUsername() {
    return username;
}

string Admin::getPassword() {
    return password;
}

string Admin::getGender() {
    return gender;
}

int Admin::getAdminID() {
    return adminID;
}

void Admin::setUsername(string username) {
    this->username = username;
}

void Admin::setPassword(string password) {
    this->password = password;
}

void Admin::setGender(string gender) {
    this->gender = gender;
}
