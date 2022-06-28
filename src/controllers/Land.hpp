#pragma once
#include <drogon/HttpController.h>
#include <chrono>

#include <src/services/tools/JsonResponses.hpp>
#include <src/services/NearbyStations.hpp>
#include <src/services/UOvService.hpp>
#include <src/services/HafasService.hpp>
#include <src/services/BEuropaService.hpp>
#include <src/services/tools/Logger.hpp>

using namespace drogon;

namespace Walrus
{
class Land : public drogon::HttpController<Land>
{
public:
    Land();

    METHOD_LIST_BEGIN
    METHOD_ADD(Land::getJourney, "journey?departure_lat={1}&departure_lng={2}&arrival_lat={3}&arrival_lng={4}&ms={5}&country={6}&city={7}&max={8}&is_arrival={9}&passengers={10}", Get);
    METHOD_LIST_END

    void getJourney(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&callback,
                    const double departureLat,
                    const double departureLng,
                    const double arrivalLat,
                    const double arrivalLng,
                    const int64_t ms,
                    const std::string country,
                    const std::string city,
                    const int max,
                    const bool isArrival,
                    const Json::Value passengers);
};
}