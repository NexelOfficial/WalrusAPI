#include "Formatters.hpp"

using namespace Walrus;
using namespace Hafas;

std::string Formatters::formatType(const std::string type)
{
    std::string parsed = "unknown";

    parsed = (type == "A" ? "address" : parsed);
    parsed = (type == "S" ? "stop" : parsed);
    parsed = (type == "P" ? "point-of-interest" : parsed);

    return parsed;
}

std::string Formatters::formatType(const Json::Value type)
{
    return formatType(type.asString());
}

std::string Formatters::formatFilter(const bool stops, const bool addresses, const bool poi)
{
    if (!stops && !addresses && !poi)
        return "S";

    std::string result = (stops ? std::string("S") : "") + (addresses ? "A" : "") + (poi ? "P" : "");
    return result;
}

std::string Formatters::formatDateTime(const std::string date, const std::string time, const int extraHours)
{
    if (time.size() != 6 && time.size() != 8)
        return "invalid size";

    std::string dStr = "";

    if (date.size() == 8)
        dStr = date.substr(0, 4) + "-" + date.substr(4, 2) + "-" + date.substr(6, 2);

    const int s = time.size();
    const std::string tStr = time.substr(s - 6, 2) + ":" + time.substr(s - 4, 2) + ":" + time.substr(s - 2, 2);

    const std::string hoursStr = std::to_string(extraHours / 60);

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << hoursStr;

    return (dStr != "" ? dStr + "T" : "") + tStr + (dStr != "" ? "+" + ss.str() + ":00" : "");
}

std::string Formatters::formatDateTime(const Json::Value date, const Json::Value time, const Json::Value extraHours)
{
    if (!time.isString())
        return "unknown type";

    return formatDateTime(date.isString() ? date.asString() : "", time.asString(), extraHours.asInt());
}

double Formatters::formatPrice(const Json::Value trfRes)
{
    if (!trfRes.isObject())
        return -1;

    if (trfRes["fareSetL"].size() == 0)
        return -1;

    std::string price;

    if (trfRes["totalPrice"]["amount"].isInt())
        price = trfRes["totalPrice"]["amount"].asString();
    else
        price = trfRes["fareSetL"][0]["fareL"][0]["price"]["amount"].asString();

    int s = price.size();
    std::string priceStr = price.substr(0, s - 2);

    if (priceStr.length() > 0)
        priceStr += "." + price.substr(s - 2, 2);
    else
        priceStr = price.substr(s - 2, 2);

    double finalAmt = std::stod(priceStr);
    return std::ceil(finalAmt * 100.0) / 100.0;
}