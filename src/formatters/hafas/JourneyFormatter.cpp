#include "JourneyFormatter.hpp"
#include "Formatters.hpp"
#include "LocationFormatter.hpp"
#include "LineFormatter.hpp"

using namespace Walrus;
using namespace Hafas;

Json::Value JourneyFormatter::formatJourney(Json::Value results, CommonParser parser)
{
    parser.parseCommonAll(results);

    double totalDistance = 0;
    double totalCO2 = 0;

    Json::Value formatted;
    Json::Value &stopovers = formatted["stopovers"];

    std::string date;

    // Load all stops
    for (int i = 0; i < results["secL"].size(); i++)
    {
        Json::Value &arrival = results["secL"][i]["arr"];
        Json::Value &departure = results["secL"][i]["dep"];
        Json::Value &journey = results["secL"][i]["jny"];

        stopovers[i]["arrival"]["location"] = LocationFormatter::formatLocation(arrival["locX"]);
        stopovers[i]["departure"]["location"] = LocationFormatter::formatLocation(departure["locX"]);
        stopovers[i]["line"] = LineFormatter::formatLineResult(journey["prodX"]);

        const Json::Value arrCrd = stopovers[i]["arrival"]["location"]["location"];
        const Json::Value depCrd = stopovers[i]["departure"]["location"]["location"];
        const double distance = Locations::calculateDistance(arrCrd["latitude"], arrCrd["longitude"], depCrd["latitude"], depCrd["longitude"]);

        stopovers[i]["distance"] = (int) distance;
        stopovers[i]["environment"]["CO2"] = (int) Locations::getEmission(journey["prodX"]["icoX"]["res"].asString(), distance);

        // Update variable if date is given
        if (journey["trainStartDate"].isString())
            date = journey["trainStartDate"].asString();

        stopovers[i]["direction"] = journey["dirTxt"];
        stopovers[i]["arrival"]["platform"] = arrival["aPltfS"]["txt"];
        stopovers[i]["departure"]["platform"] = departure["dPltfS"]["txt"];
        stopovers[i]["arrival"]["time"] = Formatters::formatDateTime(date, arrival["aTimeS"], arrival["aTZOffset"]);
        stopovers[i]["departure"]["time"] = Formatters::formatDateTime(date, departure["dTimeS"], departure["dTZOffset"]);
        totalDistance += distance;
        totalCO2 += Locations::getEmission(journey["prodX"]["icoX"]["res"].asString(), distance);
    }

    // General information
    formatted["isDiscountApplied"] = false;
    formatted["distance"] = (int)totalDistance;
    formatted["environment"]["CO2"] = (int)totalCO2;
    formatted["duration"] = Formatters::formatDateTime(Json::nullValue, results["dur"], Json::intValue);
    formatted["price"] = Formatters::formatPrice(results["trfRes"]);
    formatted["provider"] = "HAFAS";

    return formatted;
}

Json::Value JourneyFormatter::formatJourneys(Json::Value results, Json::Value common)
{
    Json::Value formatted;

    for (int i = 0; i < results.size(); i++)
        formatted.append(formatJourney(results[i], CommonParser(common)));

    return formatted;
}