#ifndef PROFESSOR_H
#define PROFESSOR_H

#include "User.h"
#include <iostream>

class Professor : public User {
public:
    Professor(int id, std::string name);
    void displayMenu() override;
    void addGrades();
    void manageDisciplines();
};

#endif // PROFESSOR_H