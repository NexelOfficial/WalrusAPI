#include "Logger.hpp"

using namespace Walrus;

void Logger::logRequest(HttpRequestPtr req)
{
    // Log time and request path
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::cout << std::put_time(&tm, "%d-%m-%Y %H:%M:%S") << " - Path: ";
    std::cout << req->getPath() << " Parameter(s): ";

    // Log all parameters
    for (const auto &parameter : req->getParameters())
        std::cout << parameter.first << ": " << parameter.second + ", ";

    std::cout << std::endl;
}