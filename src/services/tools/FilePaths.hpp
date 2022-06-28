#pragma once

#include <string>
#include <cstring>

namespace FilePaths
{
std::string getExecutablePath();
std::string getExecutableDir();
std::string mergePaths(std::string pathA, std::string pathB);
bool checkIfFileExists(const std::string &filePath);
}