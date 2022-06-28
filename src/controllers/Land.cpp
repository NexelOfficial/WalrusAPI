#include "Land.hpp"
#include <thread>

using namespace std::chrono;
using namespace Walrus;

inline NearbyStations stations;
inline UOvService uov;
inline HafasService hafas;
inline BEuropaService beurope;

Land::Land()
{
    stations = NearbyStations();
    uov = UOvService();
    hafas = HafasService();
    beurope = BEuropaService();
}

inline HttpResponsePtr getMandatoryError(bool mandatoryCondition)
{
    HttpResponsePtr resp = HttpResponse::newHttpResponse();
    resp->setContentTypeCode(CT_APPLICATION_JSON);

    Json::Value res;

    if (mandatoryCondition)
    {
        res = JsonResponses::getResponse(JsonResponses::MANDATORY_PARAM_MISSING, "A mandatory parameter is missing.", Json::nullValue);
        resp->setStatusCode(k400BadRequest);
        resp->setBody(res.toStyledString());
    }

    return resp;
}

void Land::getJourney(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const double departureLat, const double departureLng, const double arrivalLat, const double arrivalLng, const int64_t ms, const std::string country, const std::string city, const int max, const bool isArrival, const Json::Value passengers)
{
    Logger::logRequest(req);
    HttpResponsePtr resp = getMandatoryError(departureLat == 0 || departureLng == 0 || arrivalLat == 0 || arrivalLng == 0);

    if (resp->statusCode() == k200OK)
    {
        Json::Value result;

        if (country == "Netherlands")
        {
            result = uov.getJourney(departureLat, departureLng, arrivalLat, arrivalLng, ms, isArrival);
        }
        else
        {
            const Json::Value profile = hafas.getProfile(country, city);

            const Json::Value nearbyDep = stations.getNearby(departureLat, departureLng, 10);
            const Json::Value nearbyArr = stations.getNearby(arrivalLat, arrivalLng, 10);

            if (nearbyDep.size() > 0 && nearbyArr.size() > 0)
            {
                const std::string idDep = nearbyDep[0]["beneCode"].asString();
                const std::string idArr = nearbyArr[0]["beneCode"].asString();

                result = beurope.getJourney(idDep, idArr, ms, isArrival, passengers);
            }
        }

        if (result["data"] == Json::nullValue)
        {
            const Json::Value profile = hafas.getProfile(country, city);

            const Json::Value nearbyDep = stations.getNearby(departureLat, departureLng, 10);
            const Json::Value nearbyArr = stations.getNearby(arrivalLat, arrivalLng, 10);

            if (nearbyDep.size() > 0 && nearbyArr.size() > 0) {
                const int idDep = nearbyDep[0]["hafasCodes"][0].asInt();
                const int idArr = nearbyArr[0]["hafasCodes"][0].asInt();

                result = hafas.getJourney(idDep, idArr, ms, 0, 0, max, isArrival, profile);
            }
        }

        resp->setBody(result.toStyledString());
    }

    callback(resp);
}
