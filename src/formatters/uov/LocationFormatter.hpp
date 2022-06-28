#pragma once

#include <drogon/drogon.h>

using namespace drogon;

namespace Walrus
{
namespace UOv
{
class LocationFormatter
{
public:
    static Json::Value formatLocation(const Json::Value result);

private:
    LocationFormatter() {}
};
}
}