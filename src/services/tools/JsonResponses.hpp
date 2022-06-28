#pragma once

#include <drogon/drogon.h>

#include <string>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <iomanip>
#include <sstream>

namespace Walrus
{
class JsonResponses
{
public:
    enum ResponseType
    {   
        OK = 0,
        MANDATORY_PARAM_MISSING = 1,
        API_KEY_MISSING = 2,
        INVALID_PARAMETER = 3
    };
    static Json::Value getResponse(const int type, const std::string message, const Json::Value data);
    static Json::Value getResponse(const int type, const std::string message, const std::string data);
    static Json::Value nullResponse();
};
}