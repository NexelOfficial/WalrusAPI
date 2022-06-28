#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "BEuropaService.hpp"

#include <src/services/tools/JsonResponses.hpp>

using namespace Walrus;

std::string getPassengerType(const int age)
{
    if (age > 64)
        return "65";
    if (age > 59 && age < 65)
        return "S";
    if (age > 25 && age < 60)
        return "A";
    if (age > 14 && age < 26)
        return "Y";
    if (age > 11 && age < 15)
        return "12";
    if (age > 5 && age < 12)
        return "6";
    if (age > 3 && age < 6)
        return "4";

    return "0";
}

// TODO: Add maximum of 4 kids under the age of 4 per adult.
Json::Value getPartyMembers(const Json::Value passengers)
{
    Json::Value party;

    if (passengers.isArray())
    {
        for (int i = 0; i < passengers.size(); i++)
        {
            const int age = passengers[i]["age"].asInt();
            const std::string passengerType = getPassengerType(age);

            if (passengerType == "0")
                continue;

            party[i]["passengerType"] = passengerType;
            party[i]["reductCards"] = passengers[i]["reductCards"];
        }
    }

    return party;
}

Json::Value BEuropaService::getJourney(const std::string departureId, const std::string arrivalId, const int64_t ms, const bool isArrival, const Json::Value passengers)
{
    Json::Value body;
    Json::Value partyMembers = getPartyMembers(passengers);
    std::string scrapeResult = Scraper::getResult("https://www.b-europe.com/NL/Kopen/Treintickets#TravelWish", "dossier=", "&quot;");

    body["sharedCacheDossierId"] = scrapeResult;
    body["origin"] = departureId;
    body["destination"] = arrivalId;
    body["departurePreference"]["date"] = DateTime::getDateString(ms, "%d/%m/%Y", false);
    body["departurePreference"]["time"] = std::stoi(DateTime::getDateString(ms, "%H", false));
    body["departurePreference"]["timePreference"] = isArrival ? 1 : 0;
    body["maximumNumberOfTransfers"] = 5;
    body["partyMembers"] = partyMembers;

    Json::Value response = request(body);
    Json::Value allConnections = response["trainSelectionZoneData"]["trainSelectionData"]["days"][0]["connections"];
    Json::Value secondClassConnections = allConnections[0]["secondClass"]["seat"]["pages"][0]["connectionOffers"];

    if (allConnections.size() < 1)
        return JsonResponses::nullResponse();

    const Json::Value formatted = BEurope::JourneyFormatter::formatJourneys(allConnections, scrapeResult, body);

    if (formatted.isArray() && formatted[0]["stopovers"].isArray())
        return JsonResponses::getResponse(JsonResponses::OK, "Success", formatted);
    else
        return JsonResponses::nullResponse();
}

Json::Value BEuropaService::request(Json::Value body)
{
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    const std::string bodyStr = Json::writeString(builder, body);

    trantor::EventLoop* loop = drogon::app().getIOLoop(drogon::app().getCurrentThreadIndex());
    std::promise<Json::Value> prom;
    auto f = prom.get_future();

    loop->runInLoop([&]() {
        cpr::Response r = cpr::Post(
            cpr::Url{"https://www.b-europe.com/MVCUpdate/bEurope/BookingTrainSelection/ShowTrainSelectionOutbound?lang=en&ds=d22e9e15-f7ae-4af3-b38e-e0fbdefdff18&channelId=99425697-0ef8-4bbd-bffd-f658c7115007"},
            cpr::Body{bodyStr},
            cpr::Header{{"Content-Type", "application/json"}});

        Json::Value root;
        Json::Reader reader;

        reader.parse(r.text.c_str(), root);
        prom.set_value(root);
    });

    return f.get();
}