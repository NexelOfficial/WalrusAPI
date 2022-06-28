#pragma once

#include <drogon/drogon.h>

#include <drogon/utils/Utilities.h>
#include <math.h>
#include <filesystem>
#include <fstream>
#include <cpr/cpr.h>

#include <src/parsers/DateTime.hpp>
#include <src/formatters/uov/JourneyFormatter.hpp>
#include <src/parsers/DateTime.hpp>

using namespace drogon;

namespace Walrus
{
class UOvService
{
public:
    Json::Value getJourney(const double departureLat, const double departureLon, const double arrivalLat, const double arrivalLon, const int64_t ms, const bool isArrival);
private:
    Json::Value request(Json::Value body);
};
}