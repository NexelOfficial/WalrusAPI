#include "Scraper.hpp"

using namespace Walrus;

std::string getBodyFromUrl(const std::string url)
{
    std::string body;
    cpr::Response r = cpr::Get(cpr::Url{url});
    body = r.text;

    return body;
}

std::string Scraper::getResult(const std::string url, const std::string keyword, const std::string endKeyword)
{
    std::string body = getBodyFromUrl(url);
    std::string result = body.substr(body.find(keyword) + keyword.length());
    result = result.substr(0, result.find("\""));
    result = result.substr(0, result.find(endKeyword));

    return result;
}