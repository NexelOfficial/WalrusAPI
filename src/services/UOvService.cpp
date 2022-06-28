#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "UOvService.hpp"

#include <src/services/tools/JsonResponses.hpp>

using namespace Walrus;

Json::Value UOvService::getJourney(const double departureLat, const double departureLon, const double arrivalLat, const double arrivalLon, const int64_t ms, const bool isArrival)
{
    Json::Value body;

    body["intermediates"] = Json::arrayValue;
    body["excludeTraffic"] = false;
    body["preferences"]["walkSpeed"] = "SLOW";
    body["requestForArrival"] = isArrival;
    body["timestamp"] = DateTime::getDateString(ms, "%Y-%m-%dT%H:%M:%S.000Z", false);
    body["prohibitedTransportOperators"] = Json::arrayValue;
    body["prohibitedAssetAvailabilities"].append("RENTAL");
    body["prohibitedAssetAvailabilities"].append("ONDEMAND");
    body["prohibitedAssetClasses"].append("AIRPLANE");
    body["prohibitedAssetClasses"].append("BICYCLE");
    body["prohibitedAssetClasses"].append("MOPED");

    body["from"]["lng"] = departureLon;
    body["from"]["lat"] = departureLat;
    body["to"]["lng"] = arrivalLon;
    body["to"]["lat"] = arrivalLat;

    Json::Value response = request(body);
    Json::Value formatted = UOv::JourneyFormatter::formatJourneys(response);

    return JsonResponses::getResponse(JsonResponses::OK, "Success", formatted);
}

Json::Value UOvService::request(Json::Value body)
{
    trantor::EventLoop* loop = drogon::app().getIOLoop(drogon::app().getCurrentThreadIndex());
    std::promise<Json::Value> prom;
    auto f = prom.get_future();

    loop->runInLoop([&]() {
        cpr::Response r = cpr::Post(
            cpr::Url{"https://www.u-ov.info/api/mmr/"}, 
            cpr::Body{body.toStyledString()}, 
            cpr::Header{{"Content-Type", "application/json"}});

        Json::Value root;
        Json::Reader reader;

        reader.parse(r.text.c_str(), root);
        prom.set_value(root);
    });

    return f.get()["trips"];
}