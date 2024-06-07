#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <iostream>

class Admin : public User {
public:
    Admin(int id, std::string name);
    void displayMenu() override;
    void addDiscipline();
    void manageUsers();
};

#endif // ADMIN_H