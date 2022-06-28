#include "DateTime.hpp"

using namespace Walrus;

std::string DateTime::getDateString(const int64_t ms, const std::string format, const bool returnUTC)
{
    char s[100];
    time_t secs = ms / 1000;

    if (ms == 0)
    {
        auto time = std::chrono::system_clock::now();
        secs = std::chrono::system_clock::to_time_t(time);
    }

    tm time;

#ifdef __linux__
    gmtime_r(&secs, &time);
#endif
#if (_WIN32)
    gmtime_s(&time, &secs);
#endif

    std::strftime(s, sizeof(s), format.c_str(), &time);

    return (std::string)s;
}