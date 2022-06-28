#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "TequilaService.hpp"

#include <src/services/tools/JsonResponses.hpp>
#include <src/services/tools/Parameters.hpp>

using namespace Walrus;

inline Json::Value readJson(const std::string path)
{
    Json::Value root;
    std::ifstream stream(path);
    stream >> root;

    return root;
}

Json::Value TequilaService::getPassengerParameters(const Json::Value passengers)
{
    PassengerGroup adults;
    PassengerGroup children;
    PassengerGroup infants;

    if (!passengers.isArray())
        return JsonResponses::getResponse(JsonResponses::INVALID_PARAMETER, "Passengers must be of type array", Json::nullValue);

    for (const Json::Value passenger : passengers)
    {
        // Remove reductCards to prevent empty response
        // passenger["reductCards"] = Json::arrayValue;

        if (!passenger["age"].isInt())
            return JsonResponses::getResponse(JsonResponses::MANDATORY_PARAM_MISSING, "Not all passengers have the required 'age' parameter.", Json::nullValue);
        if (!passenger["handBags"].isInt())
            return JsonResponses::getResponse(JsonResponses::MANDATORY_PARAM_MISSING, "Not all passengers have the required 'handBags' parameter.", Json::nullValue);
        if (!passenger["holdBags"].isInt())
            return JsonResponses::getResponse(JsonResponses::MANDATORY_PARAM_MISSING, "Not all passengers have the required 'holdBags' parameter.", Json::nullValue);

        const int age = passenger["age"].asInt();
        const int handBags = passenger["handBags"].asInt();
        const int holdBags = passenger["holdBags"].asInt();

        if (handBags > 1 || holdBags > 2)
            return JsonResponses::getResponse(JsonResponses::INVALID_PARAMETER, "You can only have 1 handBag and 2 holdBags per passenger..", Json::nullValue);

        if (passenger["age"].asInt() < 2)
        {
            Passenger infant;
            infants.add(infant);
            if (handBags > 0 || holdBags > 0)
                return JsonResponses::getResponse(JsonResponses::INVALID_PARAMETER, "Passengers of type 'infant' are not allowed to have baggage.", Json::nullValue);
        }
        else if (passenger["age"].asInt() > 11)
        {
            Passenger adult;
            adult.handBags += handBags;
            adult.holdBags += holdBags;
            adults.add(adult);
        }
        else
        {
            Passenger child;
            child.handBags += handBags;
            child.holdBags += holdBags;
            children.add(child);
        }
    }

    Parameters params;
    params.set("adults", adults.size());
    params.set("adult_hold_bag", adults.getHoldBagsString());
    params.set("adult_hand_bag", adults.getHandBagsString());
    params.set("children", children.size());
    params.set("child_hold_bag", children.getHoldBagsString());
    params.set("child_hand_bag", children.getHandBagsString());
    params.set("infants", infants.size());

    return JsonResponses::getResponse(JsonResponses::OK, "Success", params.toString());
}

TequilaService::TequilaService()
{
    // Load API key
    std::string path = FilePaths::getExecutableDir() + "/data/keys.json";
    this->tequilaKey = readJson(path)["tequila"].asString();
}

Json::Value TequilaService::getJourney(const double departureLat, const double departureLon, const double arrivalLat, const double arrivalLon, const int64_t ms, const int radius, const Json::Value passengers)
{
    std::stringstream ss;

    // Tequila wants our lat's and lon's to be rounded to 2 decimals
    const double roundedDepLat = std::round(departureLat * 100) / 100;
    const double roundedDepLon = std::round(departureLon * 100) / 100;
    const double roundedArrLat = std::round(arrivalLat * 100) / 100;
    const double roundedArrLon = std::round(arrivalLon * 100) / 100;

    const int rad = radius == 0 ? 100 : radius;

    ss << "fly_from=" << roundedDepLat << "-" << roundedDepLon << "-" << rad << "km";
    ss << "&";
    ss << "fly_to=" << roundedArrLat << "-" << roundedArrLon << "-" << rad << "km";
    ss << "&";
    ss << "limit=3&flight_type=oneway&sort=quality";
    ss << "&";
    ss << "&date_from=" << DateTime::getDateString(ms, "%Y-%m-%d", true);
    ss << "&";
    ss << "&date_to=" << DateTime::getDateString(ms, "%Y-%m-%d", true);

    if (passengers != Json::nullValue)
    {
        Json::Value passengerResponse = getPassengerParameters(passengers);

        if (passengerResponse["data"].isString())
            ss << "&" << passengerResponse["data"].asString();
        else
            return passengerResponse;
    }

    std::string parameters = ss.str();
    Json::Value response = request(parameters);
    Json::Value formatted = Tequila::JourneyFormatter::formatJourneys(response);
    return JsonResponses::getResponse(JsonResponses::OK, "Success", formatted);
}

Json::Value TequilaService::request(std::string parameters)
{
    if (this->tequilaKey == "")
        return JsonResponses::getResponse(JsonResponses::API_KEY_MISSING, "Tequila API Key not available. Please contact website administrator.", Json::nullValue);

    cpr::Response r = cpr::Get(
        cpr::Url{"tequila-api.kiwi.com/v2/search?" + parameters},
        cpr::Header{{"apikey", this->tequilaKey}});

    Json::Value root;
    Json::Reader reader;

    reader.parse(r.text.c_str(), root);

    return root["data"];
}