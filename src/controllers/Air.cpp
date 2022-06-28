#include "Air.hpp"

using namespace std::chrono;
using namespace Walrus;

inline TequilaService tequila;

Air::Air()
{
    tequila = TequilaService();
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

void Air::getJourney(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const double departureLat, const double departureLng, const double arrivalLat, const double arrivalLng, const int64_t ms, const int radius, const Json::Value passengers)
{
    Logger::logRequest(req);
    HttpResponsePtr resp = getMandatoryError(departureLat == 0 || departureLng == 0 || arrivalLat == 0 || arrivalLng == 0);

    if (resp->statusCode() == k200OK)
    {
        Json::Value result = tequila.getJourney(departureLat, departureLng, arrivalLat, arrivalLng, ms, radius, passengers);

        resp->setBody(result.toStyledString());
    }

    callback(resp);
}
