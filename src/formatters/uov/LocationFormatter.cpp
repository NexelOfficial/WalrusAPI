#include "LocationFormatter.hpp"

using namespace Walrus;
using namespace UOv;

Json::Value LocationFormatter::formatLocation(const Json::Value result)
{
    Json::Value formatted;

    formatted["location"]["latitude"] = result["place"]["coordinates"]["lat"];
    formatted["location"]["longitude"] = result["place"]["coordinates"]["lng"];
    formatted["name"] = result["place"]["name"];
    formatted["id"] = result["place"]["technicalIdentification"]["stopCode"];
    formatted["type"] = "stop-uov";

    return formatted;
}