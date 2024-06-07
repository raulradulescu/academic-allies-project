#include "JSONHandler.h"

json JSONHandler::readJSON(const std::string& filename) {
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

void JSONHandler::writeJSON(const std::string& filename, const json& j) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }
    file << j.dump(4); // Pretty-print with 4 spaces
}
