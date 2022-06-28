#pragma once

#include <drogon/drogon.h>

#include <src/formatters/hafas/LocationFormatter.hpp>
#include <src/formatters/hafas/LineFormatter.hpp>

using namespace drogon;

namespace Walrus
{
class CommonParser
{
public:
    void insertAndParse(Json::Value& parsed, Json::Value& original, const std::string tag);
    void parseCommonAll(Json::Value& json);

    CommonParser(Json::Value commonList);

private:
    void doParse(Json::Value &value, const Json::Value commonEntry, const std::string tag);
    void doParseAll(Json::Value& json, std::set<std::string> list);
};
}