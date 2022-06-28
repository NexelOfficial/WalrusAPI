#pragma once

#include <drogon/drogon.h>

#include <drogon/utils/Utilities.h>
#include <filesystem>
#include <fstream>
#include <time.h>

#include <src/parsers/DateTime.hpp>
#include <src/formatters/uov/JourneyFormatter.hpp>
#include <src/services/tools/Locations.hpp>
#include <src/services/tools/FilePaths.hpp>


using namespace drogon;

namespace Walrus
{
class NearbyStations
{
public:
    NearbyStations();
    Json::Value fromName(const std::string _name);
    Json::Value fromBeneCode(const std::string code);
    Json::Value getNearby(const double lat, const double lng, const int rad);
};
}