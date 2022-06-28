#pragma once

#include <drogon/drogon.h>

#include <cmath>

#include <src/parsers/hafas/CommonParser.hpp>


using namespace drogon;

namespace Walrus
{
namespace UOv
{
class JourneyFormatter
{
public:
    static Json::Value formatJourneys(Json::Value result);
    static Json::Value formatJourney(Json::Value result);

private:
    static std::string getDeeplink(Json::Value formatted);
    JourneyFormatter() {}
};
}
}