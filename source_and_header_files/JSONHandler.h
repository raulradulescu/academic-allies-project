#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

class JSONHandler {
public:
    static json readJSON(const std::string& filename);
    static void writeJSON(const std::string& filename, const json& j);
};

#endif // JSONHANDLER_H