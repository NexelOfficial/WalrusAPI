#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "NearbyStations.hpp"

using namespace Walrus;

inline Json::Value stations;

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

    return similarity / (double)str2.length();
}

NearbyStations::NearbyStations()
{
    // Load HAFAS Profiles
    std::string path = FilePaths::getExecutableDir() + "/data/";
    stations = readJson(path + "stations.json");
}

Json::Value NearbyStations::fromBeneCode(const std::string code)
{
    for (Json::Value station : stations)
        if (station["beneCode"].asString() == code)
            return station;

    return Json::Value();
}

Json::Value NearbyStations::fromName(const std::string _name)
{
    Json::Value bestMatch = Json::Value();
    double bestSimilarity = 0;

    for (Json::Value station : stations)
    {
        for (Json::Value name : station["names"])
        {
            double similarity = getStringSimilarity(name.asString(), _name);

            if (similarity > bestSimilarity)
            {
                bestSimilarity = similarity;
                bestMatch = station;
            }
        }
    }
    return bestMatch;
}

Json::Value NearbyStations::getNearby(const double lat, const double lng, const int rad)
{
    std::vector<Json::Value> results;

    for (Json::Value station : stations)
    {
        const double distance = Locations::calculateDistance(lat, lng, station["location"]["latitude"].asDouble(), station["location"]["longitude"].asDouble());

        station["distance"] = distance;

        if (distance < rad)
        {
            if (results.size() <= 0)
                results.insert(results.begin(), station);
            else
                for (int i = 0; i < results.size(); i++)
                {
                    if (station["type"].asString() == "top-destination")
                    {
                        if (results[i]["type"].asString() == "top-destination")
                        {
                            if (distance < results[i]["distance"].asDouble())
                            {
                                results.insert(results.begin(), station);
                                break;
                            }
                        }
                        else
                        {
                            results.insert(results.begin() + i, station);
                            break;
                        }
                    }

                    if (results[i]["distance"].asDouble() < distance || results[i]["type"].asString() == "top-destination")
                        continue;

                    results.insert(results.begin() + i, station);
                    break;
                }
        }
    }

    Json::Value jsonMap = Json::arrayValue;

    for (const Json::Value result : results)
        jsonMap.append(result);

    return jsonMap;
}