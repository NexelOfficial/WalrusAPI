#pragma once

#include <string>

namespace Walrus
{
namespace BEurope
{
class DateTime
{
public:
    /**
     * Parse a date and time string to js compatible timestamp
     *
     * @param date The date given in format DD Mon (e.g. 27 Apr)
     * @param time The time given in format HH:MM (e.g. 12:00)
     * @return the timestamp
     */
    static std::string getParsedString(const std::string date, const std::string time);

private:
    DateTime() {}
};
}
}