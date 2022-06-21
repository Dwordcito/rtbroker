#ifndef _REALTIME_IMPLEMENTATION_HPP
#define _REALTIME_IMPLEMENTATION_HPP

#include <string>
#include "json.hpp"
#include "singleton.hpp"
#include "sqlite3/isqlite.hpp"

class RTImplementation : public Singleton<RTImplementation>
{
    public:
        void init();
        void close();
        nlohmann::json getData(const std::string & query) const;
    private:
        std::shared_ptr<SQLite::IConnection> m_Connection;
};

#endif //_REALTIME_IMPLEMENTATION_HPP

