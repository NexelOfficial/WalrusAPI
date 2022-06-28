#include "LocationFormatter.hpp"

using namespace Walrus;
using namespace BEurope;

inline std::string_view trim(std::string_view str)
{
    const auto pos(str.find_first_not_of(" \t\n\r\f\v"));
    str.remove_prefix(std::min(pos, str.length()));
    const auto pos2(str.find_last_not_of(" \t\n\r\f\v"));
    str.remove_suffix(std::min(str.length() - pos2 - 1, str.length()));

    return str;
}

Json::Value LocationFormatter::getLocation(const std::string beuropeName)
{
    NearbyStations *stations = new NearbyStations();
    Json::Value location;

    std::string trimmedName{trim(beuropeName)};
    Json::Value station = stations->fromName(beuropeName);

    location["location"] = station["location"];
    location["id"] = std::to_string(station["hafasCodes"][0].asInt());
    location["name"] = station["names"][0];
    location["type"] = "stop-beurope";

    delete stations;
    return location;
}
