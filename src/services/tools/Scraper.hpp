#pragma once

#include <string>
#include <cpr/cpr.h>

namespace Walrus
{
class Scraper
{
public:
    static std::string getResult(const std::string url, const std::string keyword, const std::string endKeyword);
private:
    Scraper();
};
}