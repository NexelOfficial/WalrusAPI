#pragma once

#include <drogon/drogon.h>

#include <drogon/utils/Utilities.h>
#include <math.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cpr/cpr.h>

#include <src/parsers/DateTime.hpp>
#include <src/formatters/tequila/JourneyFormatter.hpp>
#include <src/parsers/DateTime.hpp>
#include <src/services/tools/FilePaths.hpp>

using namespace drogon;

namespace Walrus
{
class TequilaService
{
public:
    TequilaService();
    Json::Value getJourney(const double departureLat, const double departureLon, const double arrivalLat, const double arrivalLon, const int64_t ms, const int radius, const Json::Value passengers);

private:
    enum PassengerType
    {
        INFANT = 0,
        CHILD = 1,
        ADULT = 2
    };

    struct Passenger
    {
        int holdBags = 0;
        int handBags = 0;
    };

    struct PassengerGroup
    {
        std::vector<Passenger> passengers;

        void add(const Passenger p)
        {
            passengers.push_back(p);
        }

        std::string getHoldBagsString()
        {
            std::stringstream holdBags;

            for (const Passenger passenger : passengers)
                holdBags << passenger.holdBags << ",";

            std::string holdStr = holdBags.str();

           if (holdStr.size() > 0)
                holdStr.pop_back();

            return holdStr;
        }

        std::string getHandBagsString()
        {
            std::stringstream handBags;

            for (const Passenger passenger : passengers)
                handBags << passenger.handBags << ",";

            std::string handStr = handBags.str();

            if (handStr.size() > 0)
                handStr.pop_back();

            return handStr;
        }

        int size()
        {
            return passengers.size();
        }
    };

    std::string tequilaKey;

    Json::Value request(std::string parameters);
    Json::Value getPassengerParameters(const Json::Value passengers);
};
}