#ifndef _VTASKSFUNCTIONS_HPP
#define _VTASKSFUNCTIONS_HPP

#include "sqlite3.h"
#include "json.hpp"
#include <cstddef>

namespace tables
{
    namespace sqlite
    {
        struct BaseCursor final
        {
            sqlite3_vtab_cursor base;
            size_t id{0};
            nlohmann::json data;
            size_t row{0};
            size_t n{0};
            nlohmann::json::iterator rowData;
            std::string table;
        };

        struct VirtualTable final
        {
            sqlite3_vtab base;
            std::string name;
        };

        int xCreate(sqlite3* db,
                    void* pAux,
                    int argc,
                    const char* const* argv,
                    sqlite3_vtab** ppVtab,
                    char** pzErr);

        int xBestIndex(sqlite3_vtab* pVTab,
                       sqlite3_index_info* pInfo);

        int xDestroy(sqlite3_vtab* pVTab);

        int xOpen(sqlite3_vtab* pVTab,
                  sqlite3_vtab_cursor** ppCursor);

        int xClose(sqlite3_vtab_cursor* cur);

        int xFilter(sqlite3_vtab_cursor* pVtabCursor,
                    int idxNum,
                    const char* idxStr,
                    int argc,
                    sqlite3_value** argv);

        int xNext(sqlite3_vtab_cursor* pVtabCursor);

        int xEof(sqlite3_vtab_cursor* pVtabCursor);

        int xColumn(sqlite3_vtab_cursor* pVtabCursor,
                    sqlite3_context* pContext,
                    int N);

        int xRowid(sqlite3_vtab_cursor* pVtabCursor,
                   sqlite_int64* pRowid);
    };
};

#endif // _VTASKSFUNCTIONS_HPP

