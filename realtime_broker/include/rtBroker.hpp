#ifndef _REALTIME_BROKER_HPP
#define _REALTIME_BROKER_HPP

// Define EXPORTED for any platform
#ifdef _WIN32
#ifdef WIN_EXPORT
#define EXPORTED __declspec(dllexport)
#else
#define EXPORTED __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define EXPORTED __attribute__((visibility("default")))
#else
#define EXPORTED
#endif

#include <string>
#include "json.hpp"

class EXPORTED RTBroker
{
    public:
        RTBroker();
        // LCOV_EXCL_START
        virtual ~RTBroker();
        // LCOV_EXCL_STOP
        nlohmann::json getData(const std::string & query) const;
};

#endif //_REALTIME_BROKER_HPP

