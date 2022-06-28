#pragma once

#include <drogon/drogon.h>
#include <trantor/net/EventLoopThread.h>

#include <drogon/utils/Utilities.h>
#include <filesystem>
#include <fstream>
#include <cpr/cpr.h>

#include <src/formatters/b-europe/JourneyFormatter.hpp>
#include <src/parsers/DateTime.hpp>
#include <src/services/tools/Scraper.hpp>

using namespace drogon;

namespace Walrus
{
class BEuropaService
{
public:
    Json::Value getJourney(const std::string departureId, const std::string arrivalId, const int64_t ms, const bool isArrival, const Json::Value passengers);
private:
    Json::Value request(Json::Value body);
};
}