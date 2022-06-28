#pragma once

#include <drogon/drogon.h>

#include <cpr/cpr.h>

#include <src/services/NearbyStations.hpp>

using namespace drogon;

namespace Walrus
{
namespace BEurope
{
class LocationFormatter
{
public:
    static Json::Value getLocation(const std::string beuropeName);

private:
    LocationFormatter() {}
    static Json::Value getStation(const std::string beuropeName);
};
}
}