#include "LineFormatter.hpp"

using namespace Walrus;
using namespace Hafas;

Json::Value LineFormatter::formatLineResult(const Json::Value result)
{
    Json::Value formatted;

    formatted["name"] = result["name"];
    formatted["operator"]["name"] = result["oprX"]["icoX"]["txt"].asString();
    formatted["type"] = result["prodCtx"]["catOutL"];

    std::string opr = formatted["operator"]["name"].asString();

    if (opr == "")
        opr = formatted["type"].asString();

    opr.erase(std::remove (opr.begin(), opr.end(), '_'), opr.end());
    opr.erase(std::remove (opr.begin(), opr.end(), ' '), opr.end());
    opr.erase(std::remove (opr.begin(), opr.end(), '-'), opr.end());
    opr.erase(std::remove (opr.begin(), opr.end(), '.'), opr.end());
    std::transform(opr.begin(), opr.end(), opr.begin(), ::tolower);

    const std::string imageUrl = "https://images.fly-n.nl/" + (opr != "" ? opr : "passenger") + ".png";

    formatted["operator"]["image"] = imageUrl;

    return formatted;
}

Json::Value LineFormatter::formatLineResults(const Json::Value results)
{
    Json::Value formatted;

    for (int i = 0; i < results.size(); i++)
        formatted.append(formatLineResult(results[i]));

    return formatted;
}