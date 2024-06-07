#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"
#include <map>
#include <sstream>

using json = nlohmann::json;

class User {
protected:
    int id;
    std::string name;
    int type; // 0: Admin, 1: Professor, 2: Student
public:
    User(int id, std::string name, int type) : id(id), name(name), type(type) {}
    virtual void displayMenu() = 0;
    int getId() const { return id; }
    int getType() const { return type; }
    std::string getName() const { return name; }
};

class Admin : public User {
public:
    Admin(int id, std::string name) : User(id, name, 0) {}
    void displayMenu() override {
        std::cout << "Admin Menu:\n";
        std::cout << "1. Add Discipline\n";
        std::cout << "2. Manage Users\n";
        std::cout << "3. Logout\n";
    }
    void addDiscipline();
    void manageUsers();
};

class Professor : public User {
public:
    Professor(int id, std::string name) : User(id, name, 1) {}
    void displayMenu() override {
        std::cout << "Professor Menu:\n";
        std::cout << "1. Add Grades\n";
        std::cout << "2. Manage Disciplines\n";
        std::cout << "3. Logout\n";
    }
    void addGrades();
    void manageDisciplines();
};

class Student : public User {
public:
    Student(int id, std::string name) : User(id, name, 2) {}
    void displayMenu() override {
        std::cout << "Student Menu:\n";
        std::cout << "1. View Grades\n";
        std::cout << "2. Logout\n";
    }
    void viewGrades();
};

class JSONHandler {
public:
    static json readJSON(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return json::object();
        }
        json j;
        try {
            file >> j;
        } catch (json::parse_error& e) {
            std::cerr << "Error parsing JSON from " << filename << ": " << e.what() << std::endl;
            return json::object();
        }
        return j;
    }

    static void writeJSON(const std::string& filename, const json& j) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }
        file << j.dump(4); // Pretty-print with 4 spaces
    }
};

std::map<int, User*> users;

void loadUsers() {
    json data = JSONHandler::readJSON("users.json");
    if (data.empty()) {
        std::cerr << "No users found in users.json\n";
    } else {
        std::cout << "Users loaded successfully.\n";
    }
    for (const auto& item : data) {
        int id = item["id"];
        int type = item["type"];
        std::string name = item["name"];
        if (type == 0) {
            users[id] = new Admin(id, name);
        } else if (type == 1) {
            users[id] = new Professor(id, name);
        } else if (type == 2) {
            users[id] = new Student(id, name);
        }
    }
}

void saveUsers() {
    json data;
    for (const auto& pair : users) {
        data.push_back({
                               {"id", pair.second->getId()},
                               {"type", pair.second->getType()},
                               {"name", pair.second->getName()}
                       });
    }
    JSONHandler::writeJSON("users.json", data);
}

std::map<int, json> disciplines;

void loadDisciplines() {
    json data = JSONHandler::readJSON("disciplines.json");
    if (data.empty()) {
        std::cerr << "No disciplines found in disciplines.json\n";
    } else {
        std::cout << "Disciplines loaded successfully.\n";
    }
    for (const auto& item : data) {
        int id = item["id"];
        disciplines[id] = item;
    }
}

void saveDisciplines() {
    json data;
    for (const auto& pair : disciplines) {
        data.push_back(pair.second);
    }
    JSONHandler::writeJSON("disciplines.json", data);
}
void Admin::addDiscipline() {
    int id;
    std::string name, leader;
    std::cout << "Enter Discipline ID: ";
    std::cin >> id;

    // Check if the discipline ID already exists
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
    saveDisciplines();

    // Create a new JSON file for the discipline to store grades
    std::ofstream file("discipline_" + std::to_string(id) + ".json");
    if (!file.is_open()) {
        std::cerr << "Error creating file for discipline: " << id << std::endl;
        return;
    }

    json newDisciplineData = {{"grades", json::object()}};
    file << newDisciplineData.dump(4); // Pretty-print with 4 spaces
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
            saveUsers();
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
            saveUsers();
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
#include <iomanip>
void Professor::addGrades() {
    int disciplineId;
    std::cout << "Enter Discipline ID: ";
    std::cin >> disciplineId;

    // Load disciplines
    json disciplines = JSONHandler::readJSON("disciplines.json");

    // Check if the professor is the leader of the discipline
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

    // Load existing grades for the discipline
    json disciplineData = JSONHandler::readJSON("discipline_" + std::to_string(disciplineId) + ".json");

    int studentId;
    float grade;
    std::cout << "Enter Student ID to add grade: ";
    std::cin >> studentId;
    std::cout << "Enter grade: ";
    std::cin >> grade;

    // Format the grade to two decimal places
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << grade;
    std::string formattedGrade = stream.str();

    // Check if the student already has grades in this discipline
    if (!disciplineData["grades"].contains(std::to_string(studentId))) {
        disciplineData["grades"][std::to_string(studentId)] = json::array();
    }

    // Add the new grade to the student's grades array
    disciplineData["grades"][std::to_string(studentId)].push_back(formattedGrade);

    // Save the updated grades back to the file
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
void Student::viewGrades() {
    int disciplineId;
    std::cout << "Enter Discipline ID: ";
    std::cin >> disciplineId;

    // Load disciplines
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


void mainMenu(User* user) {
    while (true) {
        user->displayMenu();
        int choice;
        std::cin >> choice;

        if (user->getType() == 0) { // Admin
            Admin* admin = dynamic_cast<Admin*>(user);
            if (choice == 1) admin->addDiscipline();
            else if (choice == 2) admin->manageUsers();
            else if (choice == 3) break;
            else std::cout << "Invalid choice. Try again.\n";
        } else if (user->getType() == 1) { // Professor
            Professor* professor = dynamic_cast<Professor*>(user);
            if (choice == 1) professor->addGrades();
            else if (choice == 2) professor->manageDisciplines();
            else if (choice == 3) break;
            else std::cout << "Invalid choice. Try again.\n";
        } else if (user->getType() == 2) { // Student
            Student* student = dynamic_cast<Student*>(user);
            if (choice == 1) student->viewGrades();
            else if (choice == 2) break;
            else std::cout << "Invalid choice. Try again.\n";
        }
    }
}

int main() {
    loadUsers();
    loadDisciplines();

    int userId;
    std::cout << "Enter User ID: ";
    std::cin >> userId;

    if (users.find(userId) != users.end()) {
        User* user = users[userId];
        mainMenu(user);
    } else {
        std::cout << "Invalid User ID" << std::endl;
    }

    saveUsers();
    saveDisciplines();

    return 0;
}


/*void testLoadUsers() {
    loadUsers();
    if (users.size() == 50) {
        std::cout << "Test Load Users: PASSED\n";
    } else {
        std::cout << "Test Load Users: FAILED\n";
    }
}
void testLoadDisciplines() {
    loadDisciplines();
    if (disciplines.size() == 10) {
        std::cout << "Test Load Disciplines: PASSED\n";
    } else {
        std::cout << "Test Load Disciplines: FAILED\n";
    }
}
void testAdminAddDiscipline() {
    Admin admin(1, "Test Admin");
    admin.addDiscipline();
    // Verify by loading the disciplines again
    loadDisciplines();
    if (disciplines.size() == 11) { // Assuming 10 initial disciplines
        std::cout << "Test Admin Add Discipline: PASSED\n";
    } else {
        std::cout << "Test Admin Add Discipline: FAILED\n";
    }
}
void testAdminAddUser() {
    Admin admin(1, "Test Admin");
    admin.manageUsers();
    // Verify by loading the users again
    loadUsers();
    if (users.size() == 51) { // Assuming 50 initial users
        std::cout << "Test Admin Add User: PASSED\n";
    } else {
        std::cout << "Test Admin Add User: FAILED\n";
    }
}
void testAdminRemoveUser() {
    Admin admin(1, "Test Admin");
    admin.manageUsers();
    // Verify by loading the users again
    loadUsers();
    if (users.size() == 50) { // Assuming 50 initial users and one removal
        std::cout << "Test Admin Remove User: PASSED\n";
    } else {
        std::cout << "Test Admin Remove User: FAILED\n";
    }
}
void testProfessorAddGrades() {
    Professor professor(2, "Test Professor");
    professor.addGrades();
    // Verify grades by loading the specific discipline file
    json disciplineData = JSONHandler::readJSON("discipline_1.json"); // Assuming discipline ID 1
    if (!disciplineData["grades"].empty()) {
        std::cout << "Test Professor Add Grades: PASSED\n";
    } else {
        std::cout << "Test Professor Add Grades: FAILED\n";
    }
}
void testProfessorManageDisciplines() {
    Professor professor(2, "Test Professor");
    professor.manageDisciplines();
    // This is an interactive test; visually check the output
}
void testStudentViewGrades() {
    Student student(3, "Test Student");
    student.viewGrades();
    // This is an interactive test; visually check the output
}
void testInvalidUserID() {
    int userId = 99999; // Assuming this ID does not exist
    if (users.find(userId) == users.end()) {
        std::cout << "Test Invalid User ID: PASSED\n";
    } else {
        std::cout << "Test Invalid User ID: FAILED\n";
    }
}
void testMissingFiles() {
    std::ifstream usersFile("users.json");
    std::ifstream disciplinesFile("disciplines.json");
    if (!usersFile.is_open() || !disciplinesFile.is_open()) {
        std::cout << "Test Missing Files: PASSED\n";
    } else {
        std::cout << "Test Missing Files: FAILED\n";
    }
}
int main() {
    loadUsers();
    loadDisciplines();

    testLoadUsers();
    testLoadDisciplines();
    testAdminAddDiscipline();
    testAdminAddUser();
    testAdminRemoveUser();
    testProfessorAddGrades();
    testInvalidUserID();
    testMissingFiles();

    // You can interactively test the following
    // testProfessorManageDisciplines();
    // testStudentViewGrades();

    return 0;
}*/