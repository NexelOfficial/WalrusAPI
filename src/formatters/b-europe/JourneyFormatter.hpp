#pragma once

#include <drogon/drogon.h>

#include <cmath>
#include <vector>
#include <cpr/cpr.h>

#include <src/parsers/hafas/CommonParser.hpp>
#include <src/parsers/beurope/DateTime.hpp>
#include <src/services/tools/Locations.hpp>

using namespace drogon;

namespace Walrus
{
namespace BEurope
{
class JourneyFormatter
{
public:
    static Json::Value formatJourneys(const Json::Value connections, const std::string dossierId, const Json::Value body);
    static Json::Value formatJourney(const Json::Value result, const Json::Value legs, const Json::Value body);

private:
    static std::string getDeeplink(Json::Value body);
    static Json::Value getLegs(const std::string connectionId, const std::string dossierId);
    static void addLocation(Json::Value& location, const std::string beuropeName);
    JourneyFormatter() {}
};
}
}