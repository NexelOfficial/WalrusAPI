#pragma once

#include <drogon/drogon.h>

#include <src/parsers/hafas/CommonParser.hpp>
#include <src/services/tools/Locations.hpp>

using namespace drogon;

namespace Walrus
{
namespace Hafas
{
class JourneyFormatter
{
public:
    static Json::Value formatJourneys(Json::Value result, Json::Value common);
    static Json::Value formatJourney(Json::Value result, CommonParser parser);

private:
    JourneyFormatter() {}
};
}
}