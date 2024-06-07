#include "Professor.h"
#include "JSONHandler.h"
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

extern std::map<int, json> disciplines;

void Professor::displayMenu() {
    std::cout << "Professor Menu:\n";
    std::cout << "1. Add Grades\n";
    std::cout << "2. Manage Disciplines\n";
    std::cout << "3. Logout\n";
}

void Professor::addGrades() {
    int disciplineId;
    std::cout << "Enter Discipline ID: ";
    std::cin >> disciplineId;

    json disciplines = JSONHandler::readJSON("disciplines.json");

    bool isLeader = false;
    for (const auto& discipline : disciplines) {
        if (discipline["id"] == disciplineId && discipline["leader"] == this->name) {
            isLeader = true;
            break;
        }
    }

    if (!isLeader) {
        std::cout << "You are not the leader of this discipline and cannot add grades.\n";
        return;
    }

    json disciplineData = JSONHandler::readJSON("discipline_" + std::to_string(disciplineId) + ".json");

    int studentId;
    float grade;
    std::cout << "Enter Student ID to add grade: ";
    std::cin >> studentId;
    std::cout << "Enter grade: ";
    std::cin >> grade;

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << grade;
    std::string formattedGrade = stream.str();

    if (!disciplineData["grades"].contains(std::to_string(studentId))) {
        disciplineData["grades"][std::to_string(studentId)] = json::array();
    }

    disciplineData["grades"][std::to_string(studentId)].push_back(formattedGrade);

    JSONHandler::writeJSON("discipline_" + std::to_string(disciplineId) + ".json", disciplineData);
    std::cout << "Grade added successfully.\n";
}

void Professor::manageDisciplines() {
    int choice;
    std::cout << "Manage Disciplines:\n";
    std::cout << "1. List Disciplines\n";
    std::cout << "2. Back to Main Menu\n";
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "List of Disciplines:\n";
        for (const auto& pair : disciplines) {
            std::cout << "ID: " << pair.second["id"] << ", Name: " << pair.second["name"] << ", Leader: " << pair.second["leader"] << std::endl;
        }
    } else if (choice == 2) {
        return;
    } else {
        std::cout << "Invalid choice. Try again.\n";
    }
}

Professor::Professor(int id, std::string name) : User(id, name, 1) {}
