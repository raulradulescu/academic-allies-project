#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <iostream>

class Student : public User {
public:
    Student(int id, std::string name);
    void displayMenu() override;
    void viewGrades();
};

#endif // STUDENT_H