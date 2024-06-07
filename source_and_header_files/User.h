#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    int id;
    std::string name;
    int type; // 0: Admin, 1: Professor, 2: Student
public:
    User(int id, std::string name, int type);
    virtual void displayMenu() = 0;
    int getId() const;
    int getType() const;
    std::string getName() const;
};

#endif // USER_H