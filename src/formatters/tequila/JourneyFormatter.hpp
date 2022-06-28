#pragma once

#include <drogon/drogon.h>

#include <cmath>

#include <src/services/NearbyAirports.hpp>

using namespace drogon;

namespace Walrus
{
    namespace Tequila
    {
        class JourneyFormatter
        {
        public:
            static Json::Value formatJourneys(Json::Value results);
            static Json::Value formatJourney(Json::Value results);

        private:
            JourneyFormatter() {}
        };
    }
}