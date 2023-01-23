#ifndef ADMIN_H
#define ADMIN_H

#include "person.h"

using namespace std;

class Admin : public Person {
private:
    int adminID;
    string username;
    string password;
    string gender;

public:
    Admin(int ID);

    string getUsername();

    string getPassword();

    string getGender();

    int getAdminID();

    void setUsername(string username);

    void setPassword(string password);

    void setGender(string gender);

};

#endif // ADMIN_H
