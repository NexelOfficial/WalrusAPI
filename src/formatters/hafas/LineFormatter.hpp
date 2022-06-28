#pragma once

#include <drogon/drogon.h>

using namespace drogon;

namespace Walrus
{
namespace Hafas
{
class LineFormatter
{
public:
    static Json::Value formatLineResults(const Json::Value result);
    static Json::Value formatLineResult(const Json::Value result);

private:
    LineFormatter() {}
};
}
}