#include "Student.h"
#include "JSONHandler.h"
#include <map>
#include <string>

extern std::map<int, json> disciplines;

void Student::displayMenu() {
    std::cout << "Student Menu:\n";
    std::cout << "1. View Grades\n";
    std::cout << "2. Logout\n";
}

void Student::viewGrades() {
    int disciplineId;
    std::cout << "Enter Discipline ID: ";
    std::cin >> disciplineId;

    json disciplines = JSONHandler::readJSON("disciplines.json");

    std::string disciplineName = "Unknown";
    for (const auto& discipline : disciplines) {
        if (discipline["id"] == disciplineId) {
            disciplineName = discipline["name"];
            break;
        }
    }

    json disciplineData = JSONHandler::readJSON("discipline_" + std::to_string(disciplineId) + ".json");

    if (disciplineData["grades"].contains(std::to_string(id))) {
        auto grades = disciplineData["grades"][std::to_string(id)];
        std::cout << "Your grades in discipline " << disciplineName << " are: ";
        for (const auto& grade : grades) {
            std::cout << grade << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No grade found for you in this discipline.\n";
    }
}

Student::Student(int id, std::string name) : User(id, name, 2) {}
