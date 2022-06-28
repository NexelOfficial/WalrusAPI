#pragma once
#include <drogon/HttpController.h>

#include <chrono>
#include <thread>

#include <src/services/tools/Logger.hpp>
#include <src/services/tools/JsonResponses.hpp>
#include <src/services/TequilaService.hpp>

using namespace drogon;

namespace Walrus
{
    class Air : public drogon::HttpController<Air>
    {
    public:
        Air();

        METHOD_LIST_BEGIN
        METHOD_ADD(Air::getJourney, "journey?departure_lat={1}&departure_lng={2}&arrival_lat={3}&arrival_lng={4}&ms={5}&radius={6}&passengers={7}", Get);
        METHOD_LIST_END

        void getJourney(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback,
                        const double departureLat,
                        const double departureLng,
                        const double arrivalLat,
                        const double arrivalLng,
                        const int64_t ms,
                        const int radius,
                        const Json::Value passengers);
    };
}