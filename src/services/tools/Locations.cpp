#include "Locations.hpp"

using namespace Walrus;

double Locations::getEmission(const std::string trainType, const double distance)
{
    std::string trainTypeLower = trainType;
    std::transform(trainTypeLower.begin(), trainTypeLower.end(), trainTypeLower.begin(), ::tolower);

    if (trainTypeLower == "")
        return 0;

    if (trainTypeLower == "ice") // ICE
        return 0.039 * distance * 1000;
    else if (trainTypeLower == "re") // Regional Express
        return 0.039 * distance * 1000;
    else if (trainTypeLower == "ic") // Intercity
        return 0.039 * distance * 1000;
    else // Average
        return 0.04 * distance * 1000;
}

double Locations::toRad(const double degree)
{
    double one_deg = (M_PI) / 180;
    return (one_deg * degree);
}

double Locations::calculateDistance(const double lat1, const double long1, const double lat2, const double long2)
{
    if (lat1 == lat2 && long1 == long2)
        return 0;

    double dist;
    dist = sin(toRad(lat1)) * sin(toRad(lat2)) + cos(toRad(lat1)) * cos(toRad(lat2)) * cos(toRad(long1 - long2));
    dist = acos(dist);
    dist = 6371 * dist;
    return dist;
}

double Locations::calculateDistance(const Json::Value lat1, const Json::Value long1, const Json::Value lat2, const Json::Value long2)
{
    return calculateDistance(lat1.asDouble(), long1.asDouble(), lat2.asDouble(), long2.asDouble());
}