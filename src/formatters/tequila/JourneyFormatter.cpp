#include "JourneyFormatter.hpp"

using namespace Walrus;
using namespace Tequila;

Json::Value JourneyFormatter::formatJourney(Json::Value results)
{
    NearbyAirports* nearbyAirports = new NearbyAirports();
    double totalCO2 = 0;
    Json::Value formatted;
    Json::Value &stopovers = formatted["stopovers"];

    // Load all stops
    for (int i = 0; i < results["route"].size(); i++)
    {
        Json::Value &journey = results["route"][i];

        stopovers[i]["arrival"]["location"] = nearbyAirports->fromId(journey["flyTo"].asString());
        stopovers[i]["departure"]["location"] = nearbyAirports->fromId(journey["flyFrom"].asString());

        stopovers[i]["arrival"]["time"] = journey["utc_arrival"];
        stopovers[i]["departure"]["time"] = journey["utc_departure"];
        stopovers[i]["line"]["type"] = journey["vehicle_type"];
        stopovers[i]["line"]["operator"]["name"] = nearbyAirports->airlineFromId(journey["airline"].asString());
        stopovers[i]["line"]["name"] = journey["flight_no"];

        stopovers[i]["line"]["operator"]["image"] = "https://images.kiwi.com/airlines/64x64/" + journey["airline"].asString() + ".png";
    }

    // General information
    formatted["isDiscountApplied"] = false;
    formatted["deeplink"] = results["deep_link"];
    formatted["price"] = results["price"];
    formatted["distance"] = results["distance"];
    formatted["environment"]["CO2"] = (int)results["distance"].asInt() * 168;
    formatted["provider"] = "Tequila";

    delete nearbyAirports; 
    return formatted;
}

Json::Value JourneyFormatter::formatJourneys(Json::Value results)
{
    Json::Value formatted;

    for (int i = 0; i < results.size(); i++)
        formatted.append(formatJourney(results[i]));

    return formatted;
}