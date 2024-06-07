#include "Admin.h"
#include "JSONHandler.h"
#include "User.h"
#include <map>
#include <string>
#include <fstream>

extern std::map<int, json> disciplines;

void Admin::displayMenu() {
    std::cout << "Admin Menu:\n";
    std::cout << "1. Add Discipline\n";
    std::cout << "2. Manage Users\n";
    std::cout << "3. Logout\n";
}

void Admin::addDiscipline() {
    int id;
    std::string name, leader;
    std::cout << "Enter Discipline ID: ";
    std::cin >> id;

    if (disciplines.find(id) != disciplines.end()) {
        std::cout << "Discipline ID already exists. Please choose a different ID.\n";
        return;
    }

    std::cout << "Enter Discipline Name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Enter Discipline Leader: ";
    std::getline(std::cin, leader);

    disciplines[id] = {{"id", id}, {"name", name}, {"leader", leader}};
    JSONHandler::writeJSON("disciplines.json", disciplines);

    std::ofstream file("discipline_" + std::to_string(id) + ".json");
    if (!file.is_open()) {
        std::cerr << "Error creating file for discipline: " << id << std::endl;
        return;
    }

    json newDisciplineData = {{"grades", json::object()}};
    file << newDisciplineData.dump(4);
    file.close();

    std::cout << "Discipline and corresponding grade file added successfully.\n";
}

void Admin::manageUsers() {
    int choice;
    std::cout << "Manage Users:\n";
    std::cout << "1. Add User\n";
    std::cout << "2. Remove User\n";
    std::cout << "3. List Users\n";
    std::cout << "4. Back to Main Menu\n";
    std::cin >> choice;

    if (choice == 1) {
        int id, type;
        std::string name;
        std::cout << "Enter User ID: ";
        std::cin >> id;
        std::cout << "Enter User Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << "Enter User Type (0: Admin, 1: Professor, 2: Student): ";
        std::cin >> type;

        if (users.find(id) == users.end()) {
            if (type == 0) {
                users[id] = new Admin(id, name);
            } else if (type == 1) {
                users[id] = new Professor(id, name);
            } else if (type == 2) {
                users[id] = new Student(id, name);
            }
            JSONHandler::writeJSON("users.json", users);
            std::cout << "User added successfully.\n";
        } else {
            std::cout << "User ID already exists.\n";
        }
    } else if (choice == 2) {
        int id;
        std::cout << "Enter User ID to remove: ";
        std::cin >> id;

        if (users.find(id) != users.end()) {
            delete users[id];
            users.erase(id);
            JSONHandler::writeJSON("users.json", users);
            std::cout << "User removed successfully.\n";
        } else {
            std::cout << "User ID not found.\n";
        }
    } else if (choice == 3) {
        std::cout << "List of Users:\n";
        for (const auto& pair : users) {
            std::cout << "ID: " << pair.second->getId() << ", Name: " << pair.second->getName() << ", Type: " << pair.second->getType() << std::endl;
        }
    } else if (choice == 4) {
        return;
    } else {
        std::cout << "Invalid choice. Try again.\n";
    }
}

Admin::Admin(int id, std::string name) : User(id, name, 0) {}
