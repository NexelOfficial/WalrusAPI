#pragma once

#include <drogon/drogon.h>

#include <cmath>
#include <sstream>
#include <iomanip>

using namespace drogon;

namespace Walrus
{
namespace Hafas
{
class Formatters
{
public:
    static std::string formatType(const std::string type);
    static std::string formatType(const Json::Value type);
    static std::string formatFilter(const bool stops, const bool addresses, const bool poi);
    static std::string formatDateTime(const std::string date, const std::string time, const int extraHours);
    static std::string formatDateTime(const Json::Value date, const Json::Value time, const Json::Value extraHours);
    static double formatPrice(const Json::Value trfRes);

private:
    Formatters() {}
};
}
}