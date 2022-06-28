#include "DateTime.hpp"

#include <ctime>
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace Walrus;
using namespace BEurope;

std::string DateTime::getParsedString(const std::string date, const std::string time)
{
    const std::string fullString = date + "-" + time;

    // Parse date and time from string
    std::tm tm;
    std::istringstream ss(fullString);
    ss >> std::get_time(&tm, "%d %b-%H:%M");

    // Add year by getting current year
    std::time_t t = std::time(nullptr);
    std::tm* pTInfo = std::localtime(&t);

    tm.tm_year = pTInfo->tm_year;

    char s[100];
    std::strftime(s, sizeof(s), "%Y-%m-%dT%H:%M:00", &tm);

    return (std::string) s;
}