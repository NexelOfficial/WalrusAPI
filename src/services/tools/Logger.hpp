#pragma once

#include <drogon/drogon.h>

#include <ctime>
#include <iomanip>

using namespace drogon;

namespace Walrus
{
class Logger
{
public:
    static void logRequest(HttpRequestPtr req);
private:
    Logger() {}
};

}