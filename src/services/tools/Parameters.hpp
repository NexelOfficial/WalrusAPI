#pragma once

#include <string>
#include <map>
#include <sstream>

namespace Walrus
{
struct Parameters
{
    std::string toString()
    {
        std::stringstream ss;

        for (auto const& val : this->parameters)
            ss << val.first << "=" << val.second << "&";

        ss.seekp(-1, std::ios_base::end);
        return ss.str();
    }

    void set(const std::string key, const std::string value)
    {
        this->parameters[key] = value;
    }

    void set(const std::string key, const int value)
    {
        this->set(key, std::to_string(value));
    }

    void set(const std::string key, const float value)
    {
        this->set(key, std::to_string(value));
    }

    private:
        std::map<std::string, std::string> parameters;
};
}