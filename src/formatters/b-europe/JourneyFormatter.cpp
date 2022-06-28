#include "JourneyFormatter.hpp"
#include "LocationFormatter.hpp"

using namespace Walrus;
using namespace BEurope;

bool isNumber(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

inline std::string formatTrainLabel(const std::string trainLabel)
{
    std::vector<std::string> parts = cpr::util::split(trainLabel, ' ');
    for (auto& part : parts)
        if (isNumber(part))
            part = "";

    std::string result;

    for (auto& part : parts)
        if (!part.empty())
            result += part + " ";

    return result.substr(0, result.size() - 1);
}

inline std::string JourneyFormatter::getDeeplink(Json::Value body)
{
    std::string deeplink = "https://www.b-europe.com/NL/Kopen/Treintickets?autoactivatestep2=true&origin=" + body["origin"].asString() + "&dest=" + body["destination"].asString() + "&traveltype=OneWay&outbound=" + body["departurePreference"]["date"].asString() + "&outboundt=" + body["departurePreference"]["time"].asString();

    return deeplink;
}

Json::Value JourneyFormatter::formatJourney(const Json::Value result, const Json::Value legs, const Json::Value body)
{
    double totalDistance = 0;
    double totalCO2 = 0;

    Json::Value formatted;
    Json::Value &stopovers = formatted["stopovers"];
    Json::Value offer = result["secondClass"]["seat"]["pages"][0]["connectionOffers"][0];

    // Load all stops
    for (const Json::Value journey : legs)
    {
        Json::Value stopover;

        stopover["arrival"]["platform"] = journey["arrivalPlatform"];
        stopover["departure"]["platform"] = journey["departurePlatform"];
        stopover["arrival"]["location"] = LocationFormatter::getLocation(journey["destination"].asString());
        stopover["departure"]["location"] = LocationFormatter::getLocation(journey["origin"].asString());
        const Json::Value arrCrd = stopover["arrival"]["location"]["location"];
        const Json::Value depCrd = stopover["departure"]["location"]["location"];
        const double distance = Locations::calculateDistance(arrCrd["latitude"].asDouble(), arrCrd["longitude"].asDouble(), depCrd["latitude"].asDouble(), depCrd["longitude"].asDouble());

        stopover["distance"] = (int)distance;
        stopover["environment"]["CO2"] = (int)Locations::getEmission(journey["trainLabel"].asString(), distance);

        stopover["direction"] = "";
        stopover["arrival"]["time"] = DateTime::getParsedString(result["mobileNoHourActionBarLabel"].asString(), journey["arrivalTimeLabel"].asString());
        stopover["departure"]["time"] = DateTime::getParsedString(result["mobileNoHourActionBarLabel"].asString(), journey["departureTimeLabel"].asString());
        stopover["line"]["type"] = journey["isWalk"].asBool() == true ? "Walk" : formatTrainLabel(journey["trainLabel"].asString());
        stopover["line"]["operator"]["name"] = Json::nullValue;
        stopover["line"]["name"] = Json::nullValue;

        if (distance < 20)
            continue;

        std::string opr = journey["trainIcon"].asString().erase(0, 8);
        std::transform(opr.begin(), opr.end(), opr.begin(), ::tolower);
        const std::string imageUrl = "https://images.fly-n.nl/" + (opr != "" ? opr : "passenger") + ".png";
        stopover["line"]["operator"]["image"] = imageUrl;

        totalDistance += distance;
        totalCO2 += (int)Locations::getEmission(journey["trainLabel"].asString(), distance);
        stopovers.append(stopover);
    }

    std::string deeplink = getDeeplink(body);

    // General information
    formatted["deeplink"] = deeplink;
    formatted["isDiscountApplied"] = offer["discountCard"];
    formatted["price"] = offer["price"];
    formatted["distance"] = (int)totalDistance;
    formatted["environment"]["CO2"] = (int)totalCO2;
    formatted["provider"] = "B-Europe";

    return formatted;
}

Json::Value JourneyFormatter::formatJourneys(const Json::Value connections, const std::string dossierId, const Json::Value body)
{
    Json::Value formatted;

    for (int i = 0; i < std::min((int)connections.size(), 3); i++)
        formatted.append(formatJourney(connections[i], getLegs(connections[i]["connectionId"].asString(), dossierId), body));

    return formatted;
}

Json::Value JourneyFormatter::getLegs(const std::string connectionId, const std::string dossierId)
{
    Json::Value body;
    body["sharedCacheDossierId"] = dossierId;
    body["connectionId"] = connectionId;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    const std::string bodyStr = Json::writeString(builder, body);

    cpr::Response r = cpr::Post(
                          cpr::Url{"https://www.b-europe.com/MVCUpdate/bEurope/BookingTrainSelection/GetSchedule?lang=en&ds=d22e9e15-f7ae-4af3-b38e-e0fbdefdff18&channelId=99425697-0ef8-4bbd-bffd-f658c7115007"},
                          cpr::Body{bodyStr},
    cpr::Header{{"Content-Type", "application/json"}});

    Json::Value root;
    Json::Reader reader;

    reader.parse(r.text.c_str(), root);

    return root["legs"];
}