#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "NearbyAirports.hpp"

using namespace Walrus;

inline Json::Value airports;
inline Json::Value airlines;

inline Json::Value readJson(const std::string path)
{
    Json::Value root;
    std::ifstream stream(path);
    stream >> root;

    return root;
}

inline double getStringSimilarity(std::string str1, std::string str2)
{
    int similarity = 0;

    std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
    std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);

    for (int i = 0; i < str1.length(); i++)
        if (str1[i] == str2[i])
            similarity++;

    // Prevent floating point exception (division by zero)
    if (str1.length() == 0 || str2.length() == 0)
        return 0;

    return similarity / (double) str2.length();
}

NearbyAirports::NearbyAirports()
{
    // Load HAFAS Profiles
    std::string path = FilePaths::getExecutableDir() + "/data/";
    airports = readJson(path + "airports.json");
    airlines = readJson(path + "airlines.json");
}

Json::Value NearbyAirports::fromId(const std::string id)
{
    for (Json::Value airport : airports)
        if (airport["id"].asString() == id)
            return airport;

    return Json::Value();
}

Json::Value NearbyAirports::airlineFromId(const std::string id)
{
    return airlines[id];
}

Json::Value NearbyAirports::getNearby(const double lat, const double lng, const int rad)
{
    std::vector<Json::Value> results;

    for (Json::Value airport : airports)
    {
        const double distance = Locations::calculateDistance(lat, lng, airport["location"]["latitude"].asDouble(), airport["location"]["longitude"].asDouble());

        airport["distance"] = distance;

        if (distance < rad)
        {
            results.insert(results.begin(), airport);
        }
    }

    Json::Value jsonMap = Json::arrayValue;

    for (const Json::Value result : results)
        jsonMap.append(result);

    return jsonMap;
}