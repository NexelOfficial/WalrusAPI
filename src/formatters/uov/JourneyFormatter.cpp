#include "JourneyFormatter.hpp"
#include "LocationFormatter.hpp"

using namespace Walrus;
using namespace UOv;

inline int getCO2(Json::Value uovCo2)
{
    int co2 = uovCo2["value"].asInt();

    if (co2 < 0)
        co2 = 0;

    return co2;
}

inline std::string JourneyFormatter::getDeeplink(Json::Value formatted)
{
    Json::Value firstStopover = formatted["stopovers"][0];
    Json::Value lastStopover = formatted["stopovers"][formatted["stopovers"].size()-1];
    Json::Value departure = firstStopover["departure"]["location"];
    Json::Value arrival = lastStopover["arrival"]["location"];

    std::string replacement = "%20";

    std::string firstName = departure["name"].asString();
    std::string lastName = arrival["name"].asString();

    while (firstName.find(" ") != std::string::npos)
        firstName.replace(firstName.find(" "), replacement.size(), replacement);

    while (lastName.find(" ") != std::string::npos)
        lastName.replace(lastName.find(" "), replacement.size(), replacement);

    std::string firstLatLon = departure["location"]["latitude"].asString() + "," + departure["location"]["longitude"].asString();
    std::string lastLatLon = arrival["location"]["latitude"].asString() + "," + arrival["location"]["longitude"].asString();

    std::string deeplink = "https://www.ns.nl/rpx?ctx=arnu%7CfromLocation%3D" + firstLatLon + "%7CtoLocation%3D" + lastLatLon + "%7CplannedFromTime%3D" + firstStopover["departure"]["time"].asString() + "%7CplannedArrivalTime%3D" + lastStopover["arrival"]["time"].asString() + "%7CyearCard%3Dfalse%7CexcludeHighSpeedTrains%3Dfalse%7CsearchForAccessibleTrip%3Dfalse";

    return deeplink;
}

Json::Value JourneyFormatter::formatJourney(Json::Value results)
{
    double price = 0;
    double totalDistance = 0;
    double totalCO2 = 0;
    Json::Value formatted;
    Json::Value &stopovers = formatted["stopovers"];

    // Load all stops
    for (const Json::Value leg : results["legs"])
    {
        Json::Value journey = leg["possibilities"][0]["partitions"][0];
        Json::Value stopover;

        stopover["arrival"]["location"] = LocationFormatter::formatLocation(journey["to"]);
        stopover["departure"]["location"] = LocationFormatter::formatLocation(journey["from"]);
        stopover["arrival"]["platform"] = journey["to"]["place"]["area"];
        stopover["departure"]["platform"] = journey["from"]["place"]["area"];

        stopover["distance"] = journey["distance"].asInt() / 1000;
        stopover["direction"] = journey["direction"];
        stopover["arrival"]["time"] = journey["endTime"];
        stopover["departure"]["time"] = journey["startTime"];
        stopover["line"]["type"] = journey["brand"] != "" ? journey["brand"] : journey["assets"][0]["assetClass"];
        stopover["line"]["operator"]["name"] = journey["operatorName"];
        stopover["line"]["name"] = journey["traject"];
        stopover["environment"]["CO2"] = getCO2(journey["CO2"]);

        if (journey["distance"].asFloat() < 20)
            continue;

        std::string opr = journey["operatorName"].asString();

        opr.erase(std::remove(opr.begin(), opr.end(), '_'), opr.end());
        opr.erase(std::remove(opr.begin(), opr.end(), ' '), opr.end());
        opr.erase(std::remove(opr.begin(), opr.end(), '-'), opr.end());
        opr.erase(std::remove(opr.begin(), opr.end(), '.'), opr.end());
        std::transform(opr.begin(), opr.end(), opr.begin(), ::tolower);

        const std::string imageUrl = "https://images.fly-n.nl/" + (opr != "" ? opr : "passenger") + ".png";

        stopover["line"]["operator"]["image"] = imageUrl;

        for (Json::Value &asset : journey["assets"])
            // Entry fee + Actual value / 100 because it's in cents and we want it in euro's
            price += (asset["fare"]["entryFee"].asDouble() + asset["fare"]["value"].asDouble()) / 100;

        totalDistance += stopover["distance"].asInt();
        totalCO2 += getCO2(journey["CO2"]);
        stopovers.append(stopover);
    }

    std::string deeplink = getDeeplink(formatted);

    // General information
    formatted["isDiscountApplied"] = false;
    formatted["deeplink"] = deeplink;
    formatted["price"] = std::ceil(price * 100.0) / 100.0;
    formatted["distance"] = (int)totalDistance;
    formatted["environment"]["CO2"] = (int)totalCO2;
    formatted["provider"] = "U-OV";

    return formatted;
}

Json::Value JourneyFormatter::formatJourneys(Json::Value results)
{
    Json::Value formatted;

    for (int i = 0; i < results.size(); i++)
        formatted.append(formatJourney(results[i]));

    return formatted;
}