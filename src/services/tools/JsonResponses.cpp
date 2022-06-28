#include "JsonResponses.hpp"

using namespace Walrus;

const std::string types[4] = {"OK", "MANDATORY_PARAMETER_MISSING", "API_KEY_MISSING", "INVALID_PARAMETER"};

/**
 * Get a formatted JSON message.
 *
 * Provide a ResponseType
 *
 * @param error The error type to give. Use the ErrorType enum for this.
 * @param message The message to appear.
 * @return A formatted JSON error.
 */

Json::Value createResponse(const int type, const std::string message, const bool isEmpty)
{
    Json::Value result;

    result["type"] = "INVALID_JSON_ERROR";
    result["message"] = "An invalid error type was provied. Please contact the site administrator if you believe this is a mistake.";

    if (type > (sizeof(types) / sizeof(*types)))
        return result;

    result["type"] = types[type];
    result["message"] = message;

    if (type == JsonResponses::OK && isEmpty)
    {
        result["type"] = "NO_DATA";
        result["message"] = "No results are found. Please contact the site administrator if you believe this is a mistake.";
    }

    if (isEmpty)
        return result;

    return result;
}

Json::Value JsonResponses::getResponse(const int type, const std::string message, const Json::Value data)
{
    Json::Value result = createResponse(type, message, data == Json::nullValue || data.size() == 0);
    result["data"] = data;

    return result;
}

Json::Value JsonResponses::getResponse(const int type, const std::string message, const std::string data)
{
    Json::Value result = createResponse(type, message, data == "");
    result["data"] = data;

    return result;
}

Json::Value JsonResponses::nullResponse()
{
    Json::Value result;
    result["type"] = "NO_DATA";
    result["message"] = "No results are found. Please contact the site administrator if you believe this is a mistake.";
    return result;
}