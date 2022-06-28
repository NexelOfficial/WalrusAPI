#pragma once

#include <drogon/drogon.h>

#include <drogon/utils/Utilities.h>
#include <math.h>
#include <filesystem>
#include <fstream>
#include <cpr/cpr.h>

#include <src/parsers/hafas/CommonParser.hpp>
#include <src/services/tools/Checksum.hpp>
#include <src/formatters/hafas/Formatters.hpp>
#include <src/formatters/hafas/LocationFormatter.hpp>
#include <src/formatters/hafas/JourneyFormatter.hpp>
#include <src/services/tools/FilePaths.hpp>
#include <src/parsers/DateTime.hpp>

using namespace drogon;

namespace Walrus
{
class HafasService
{
public:
    HafasService();

    Json::Value getNearby(const int max, const double lat, const double lng, const int rad, const Json::Value profile);
    Json::Value getStopInfo(const int stopId);
    Json::Value getProfile(const std::string country, const std::string city);
    Json::Value getProfile(const std::string profile);
    Json::Value getProfile(const int id);
    Json::Value getJourney(const int departureId, const int arrivalId, const int64_t ms, const int minTransferTime, const int maxTransferTime, const int max, const bool isArrival, const Json::Value profile);
private:
    Json::Value request(const Json::Value svcReqL, const Json::Value profile);
    Json::Value requestAll(const Json::Value svcReqL);
};
}