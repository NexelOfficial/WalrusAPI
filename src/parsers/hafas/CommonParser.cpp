#include "CommonParser.hpp"

using namespace Walrus;

Json::Value common;

CommonParser::CommonParser(Json::Value commonList)
{
    common = commonList;
}

std::map<std::string, std::string> combis{
    {"prodX", "prodL"},
    {"pRefL", "prodL"},
    {"oprX", "opL"},
    {"locX", "locL"},
    {"himX", "himL"},
    {"polyXL", "polyL"},
    {"icoX", "icoL"}};

void CommonParser::doParse(Json::Value &value, const Json::Value commonEntry, const std::string tag)
{
    if (!value.isInt())
        return;

    value = commonEntry[value.asInt()];

    for (const auto &combi : combis)
        if (value.isMember(combi.first))
            this->insertAndParse(value[combi.first], value[combi.first], combi.first);
}

void CommonParser::doParseAll(Json::Value& json, std::set<std::string> list)
{
    for (Json::Value &entry : json)
    {
        if (!entry.empty() || entry.size() > 0)
            this->doParseAll(entry, list);

        if (!entry.isObject())
            continue;

        for (std::string key : list)
        {
            const bool isMember = entry.isMember(key);

            if (!isMember || (!entry[key].isArray() && !entry[key].isInt()))
                continue;

            const Json::Value& commonEntry = common[combis[key]];

            if (entry[key].isArray())
                for (Json::Value &value : entry[key])
                    this->doParse(value, commonEntry, key);
            else
                this->doParse(entry[key], commonEntry, key);
        }
    }
}

void CommonParser::parseCommonAll(Json::Value& json)
{
    std::set<std::string> list;

    for (auto const x : combis)
        if (common.isMember(x.second))
            list.insert(x.first);

    this->doParseAll(json, list);
}

void CommonParser::insertAndParse(Json::Value& parsed, Json::Value& original, const std::string tag)
{
    const Json::Value& commonEntry = common[combis[tag]];

    if (original.isArray())
        for (Json::Value &value : original)
            this->doParse(value, commonEntry, tag);
    else
        this->doParse(original, commonEntry, tag);

    parsed = original;
}