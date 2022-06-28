#include "LocationFormatter.hpp"

using namespace Walrus;
using namespace Hafas;

Json::Value LocationFormatter::formatLocation(const Json::Value result)
{
    Json::Value formatted;

    formatted["location"]["latitude"] = result["crd"]["y"].asDouble() / 1000000;
    formatted["location"]["longitude"] = result["crd"]["x"].asDouble() / 1000000;
    formatted["name"] = result["name"];
    formatted["id"] = result["extId"];
    formatted["type"] = Formatters::formatType(result["type"]);

    return formatted;
}

Json::Value LocationFormatter::formatLocations(const Json::Value results)
{
    Json::Value formatted;

    for (int i = 0; i < results.size(); i++)
        formatted.append(formatLocation(results[i]));

    return formatted;
}