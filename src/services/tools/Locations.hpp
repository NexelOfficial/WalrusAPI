#pragma once

#include <drogon/drogon.h>

#include <string>
#include <cmath>
#include <algorithm>

namespace Walrus
{
class Locations
{
public:
    static double getEmission(const std::string trainType, const double distance);
    static double toRad(const double degree);
    static double calculateDistance(const double lat1, const double long1, const double lat2, const double long2);
    static double calculateDistance(const Json::Value lat1, const Json::Value long1, const Json::Value lat2, const Json::Value long2);
private:
    Locations();
};
}