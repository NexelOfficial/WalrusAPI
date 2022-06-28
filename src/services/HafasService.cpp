#include "HafasService.hpp"

#include <src/services/tools/JsonResponses.hpp>

using namespace Walrus;

Json::Value profiles;

inline Json::Value readJson(const std::string path)
{
    Json::Value root;
    std::ifstream stream(path);
    stream >> root;

    return root;
}

HafasService::HafasService()
{
    // Load Stations
    std::string path = FilePaths::getExecutableDir() + "/data/profiles/";

    for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(path))
    {
        std::string entryStr = entry.path().string();
        std::string file = entryStr.substr(entryStr.find_last_of("/\\") + 1);

        if (file.find(".") == std::string::npos)
            continue;

        std::string fileName = file.erase(file.find_first_of("."));
        profiles[fileName] = readJson(entry.path().string());
    }
}

Json::Value HafasService::getStopInfo(const int stopId)
{
    Json::Value svcReq;
    Json::Value &locL = svcReq["req"]["locL"][0];

    svcReq["cfg"]["rtMode"] = "REALTIME";
    svcReq["meth"] = "LocDetails";
    locL["type"] = "S";
    locL["lid"] = "A=1@L=" + std::to_string(stopId) + "@";

    Json::Value profile = this->getProfile(stopId);
    Json::Value results = request(svcReq, profile);

    return Hafas::LocationFormatter::formatLocations(results["res"]["locL"]);
}

Json::Value HafasService::getProfile(const int id)
{
    const std::string str = std::to_string(id);

    for (Json::Value profile : profiles)
    {
        std::string idRegex = profile["id_regex"].isString() ? profile["id_regex"].asString() : "";

        if (std::regex_search(str, std::regex(idRegex)))
            return profile;
    }

    return profiles["db"];
}

Json::Value HafasService::getProfile(const std::string profile)
{
    for (std::string name : profiles.getMemberNames())
    {
        if (name == profile)
            return profiles[name];
    }

    return profiles["db"];
}

Json::Value HafasService::getProfile(const std::string country, const std::string city)
{
    for (std::string profile : profiles.getMemberNames())
    {
        bool isCountryStr = profiles[profile]["country"].isString();
        std::string profileCountry = isCountryStr ? profiles[profile]["country"].asString() : "";

        if (country == profileCountry)
            return profiles[profile];
    }

    return profiles["db"];
}

Json::Value HafasService::getJourney(const int departureId, const int arrivalId, const int64_t ms, const int minTransferTime, const int maxTransferTime, const int max, const bool isArrival, const Json::Value profile)
{
    // TODO: Implement viaLocL
    Json::Value svcReq;
    Json::Value &req = svcReq["req"];
    Json::Value &depLocL = req["depLocL"][0];
    Json::Value &arrLocL = req["arrLocL"][0];
    Json::Value &jnyFltrL = req["jnyFltrL"];
    Json::Value &trfReq = req["trfReq"];

    svcReq["cfg"]["polyEnc"] = "GPA";
    svcReq["cfg"]["rtMode"] = "REALTIME";
    svcReq["meth"] = "TripSearch";

    req["getPasslist"] = true;
    req["maxChg"] = maxTransferTime > 0 && maxTransferTime > minTransferTime ? maxTransferTime : -1;
    req["minChgTime"] = minTransferTime > 5 ? minTransferTime : 5;
    req["getTariff"] = true;
    req["getIV"] = true;
    req["getPolyline"] = false; // Give visible line of the trip
    req["outDate"] = DateTime::getDateString(ms, "%Y%m%d", true);
    req["outTime"] = DateTime::getDateString(ms, "%H%M%S", true);
    req["outFrwd"] = !isArrival;
    req["numF"] = max;

    req["gisFltrL"] = Json::Value(Json::arrayValue);
    req["viaLocL"] = Json::Value(Json::arrayValue);

    depLocL["type"] = "S";
    depLocL["lid"] = "A=1@L=" + std::to_string(departureId) + "@";
    arrLocL["type"] = "S";
    arrLocL["lid"] = "A=1@L=" + std::to_string(arrivalId) + "@";

    jnyFltrL[0]["type"] = "PROD";
    jnyFltrL[0]["mode"] = "INC";
    jnyFltrL[0]["value"] = "1023";

    jnyFltrL[1]["type"] = "META";
    jnyFltrL[1]["mode"] = "INC";
    jnyFltrL[1]["meta"] = "notBarrierfree";

    trfReq["jnyCl"] = 2;
    trfReq["tvlrProf"][0]["type"] = "E";
    trfReq["tvlrProf"][0]["redtnCard"] = Json::nullValue; // what is this?
    trfReq["cType"] = "PK";

    Json::Value results = request(svcReq, profile);
    Json::Value formatted = Hafas::JourneyFormatter::formatJourneys(results["res"]["outConL"], results["res"]["common"]);

    return JsonResponses::getResponse(JsonResponses::OK, "Success", formatted);
}

Json::Value HafasService::request(const Json::Value svcReqL, const Json::Value profile)
{
    Json::Value body;

    body = profile["body"];
    body["svcReqL"].append(svcReqL);

    cpr::Response r = cpr::Post(
                          cpr::Url{profile["endpoint"].asString()},
                          cpr::Body{body.toStyledString()},
    cpr::Header{{"Content-Type", "application/json"}},
    cpr::Parameters{{"checksum", Checksum::generate(body.toStyledString(), profile["salt"].asString())}});

    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();

    Json::Value root;
    std::string err;

    reader->parse(
        r.text.c_str(),
        r.text.c_str() + r.text.size(),
        &root,
        &err);

    if (root["err"].asString() == "OK")
        return root["svcResL"][0];

    return root;
}