#include <string>
#include <stdio.h>
#ifndef UTILS_H
#define UTILS_H

class Utils {
public:
    static std::string readFile(const std::string& filePath);
    static bool writeFile(const std::string &filePath, const std::string &content);
};

#endif