#pragma once

#include <ctime>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>

namespace Walrus
{
class DateTime
{
public:
    static std::string getDateString(const int64_t ms, const std::string format, const bool returnUTC);

private:
    DateTime() {}
};
}