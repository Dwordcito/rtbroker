#include "vTabsFunctions.hpp"
#include "tableManager.hpp"

int tables::sqlite::xCreate(sqlite3* db,
                            void*/* pAux*/,
                            int argc,
                            const char* const* argv,
                            sqlite3_vtab** ppVtab,
                            char** /*pzErr*/)
{
    auto rc = SQLITE_NOMEM;

    if (argc != 0 && argv[0] != nullptr)
    {

        auto* pVtab { new tables::sqlite::VirtualTable() };
        pVtab->base = {};
        pVtab->name = std::string(argv[0]);
        const auto statement
        {
            "CREATE TABLE " + pVtab->name + TableManager::instance().columnSchema(pVtab->name)
        };
        rc = sqlite3_declare_vtab(db, statement.c_str());

        if (SQLITE_OK == rc)
        {
            *ppVtab = (sqlite3_vtab*)pVtab;
        }
    }
    return rc;
}

int tables::sqlite::xDestroy(sqlite3_vtab* pVTab)
{
    delete reinterpret_cast<tables::sqlite::VirtualTable*>(pVTab);
    return SQLITE_OK;
}

int tables::sqlite::xBestIndex(sqlite3_vtab* /*pVTab*/,
                               sqlite3_index_info* /*pInfo*/)
{
    return SQLITE_OK;
}

int tables::sqlite::xOpen(sqlite3_vtab* pVTab,
                          sqlite3_vtab_cursor** ppCursor)
{
    auto pCur { new tables::sqlite::BaseCursor };
    *ppCursor = reinterpret_cast<sqlite3_vtab_cursor*>(pCur);
    auto pVirtualTable { reinterpret_cast<tables::sqlite::VirtualTable *>(pVTab) };
    pCur->data = TableManager::instance().data(pVirtualTable->name);
    pCur->n = pCur->data.size();
    pCur->rowData = pCur->data.begin();
    pCur->table = pVirtualTable->name;
    return SQLITE_OK;
}

int tables::sqlite::xClose(sqlite3_vtab_cursor* cur)
{
    delete reinterpret_cast<tables::sqlite::BaseCursor*>(cur);
    return SQLITE_OK;
}

int tables::sqlite::xFilter(sqlite3_vtab_cursor* /*pVtabCursor*/,
                            int /*idxNum*/,
                            const char* /*idxStr*/,
                            int /*argc*/,
                            sqlite3_value** /*argv*/)
{
    return SQLITE_OK;
}

int tables::sqlite::xNext(sqlite3_vtab_cursor* pVtabCursor)
{
    auto pCur { reinterpret_cast<BaseCursor*>(pVtabCursor) };
    pCur->row++;
    pCur->rowData++;
    return SQLITE_OK;
}

int tables::sqlite::xEof(sqlite3_vtab_cursor* pVtabCursor)
{
    auto pCur { reinterpret_cast<BaseCursor*>(pVtabCursor) };
    return pCur->rowData == pCur->data.end();
}

int tables::sqlite::xColumn(sqlite3_vtab_cursor* pVtabCursor,
                            sqlite3_context* pContext,
                            int N)
{
    auto pCur { reinterpret_cast<BaseCursor*>(pVtabCursor) };
    auto schema { TableManager::instance().columns(pCur->table) };

    auto element { std::next(schema.begin(), N) };
    const auto columnType { element->second };

    if (!pCur->rowData->contains(element->first))
    {
        sqlite3_result_null(pContext);
    }
    else
    {
        if (columnType == INTEGER)
        {
            sqlite3_result_int(pContext, pCur->rowData->at(element->first).get<int>());
        }
        else if (columnType == TEXT)
        {
            sqlite3_result_text(pContext, pCur->rowData->at(element->first).get_ref<const std::string&>().c_str(), -1, SQLITE_TRANSIENT);
        }
        else if (columnType == BIGINT)
        {
            sqlite3_result_int64(pContext, pCur->rowData->at(element->first).get<long long>());
        }
        else if (columnType == DOUBLE)
        {
            sqlite3_result_double(pContext, pCur->rowData->at(element->first).get<double>());
        }
        else if (columnType == DATETIME)
        {
            sqlite3_result_text(pContext, pCur->rowData->at(element->first).get_ref<const std::string&>().c_str(), -1, SQLITE_TRANSIENT);
        }
        else if (columnType == BOOLEAN)
        {
            sqlite3_result_int(pContext, pCur->rowData->at(element->first).get<bool>());
        }
        else
        {
            sqlite3_result_null(pContext);
        }
    }
    return SQLITE_OK;
}

int tables::sqlite::xRowid(sqlite3_vtab_cursor* /*pVtabCursor*/,
                           sqlite_int64* /*pRowid*/)
{
    return SQLITE_OK;
}

