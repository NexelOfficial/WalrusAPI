#pragma once

#include <drogon/drogon.h>

#include <src/formatters/hafas/Formatters.hpp>

using namespace drogon;

namespace Walrus
{
namespace Hafas
{
class LocationFormatter
{
public:
    static Json::Value formatLocations(const Json::Value result);
    static Json::Value formatLocation(const Json::Value result);

private:
    LocationFormatter() {}
};
}
}