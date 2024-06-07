#include "User.h"

User::User(int id, std::string name, int type) : id(id), name(name), type(type) {}

int User::getId() const { return id; }
int User::getType() const { return type; }
std::string User::getName() const { return name; }