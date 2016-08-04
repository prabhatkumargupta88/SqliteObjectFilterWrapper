//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: SqliteModule.h
///     Author	: Prabhat Kumar Gupta, STS
///
//////////////////////////////////////////////////////////////////////////////////////////

#include "sqlite3.h"


/// <summary>
/// Stucture for cursor in virtual table.
/// <summary>
struct data_cursor
{
	sqlite3_vtab_cursor base;
	int count;
	int eof;
};

/// <summary>
/// Stucture to store instance of current object to pass to various sqlite APIs.
/// <summary>
struct SObjectVTab : public sqlite3_vtab
{
	void *m_pMyObjectFilter;
};

/// <summary>
/// Class to provide filter capabilities for Objects.
/// Filtering mechanism is provided using sqlite.
/// <summary>
class CSqlite_module {

public:

	static int iVersion;
	static int CreateVirtualTable(sqlite3*, void *pAux, int argc, const char *const*argv, sqlite3_vtab **ppVTab, char**);
	static int ConnectVirtualTable(sqlite3*, void *pAux, int argc, const char *const*argv, sqlite3_vtab **ppVTab, char**);
	static int BestIndexVirtualTable(sqlite3_vtab *pVTab, sqlite3_index_info*);
	static int DisconnectVirtualTable(sqlite3_vtab *pVTab);
	static int DestroyVirtualTable(sqlite3_vtab *pVTab);
	static int OpenCursor(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor);
	static int CloseCursor(sqlite3_vtab_cursor*);
	static int FilterVirtualTable(sqlite3_vtab_cursor*, int idxNum, const char *idxStr, int argc, sqlite3_value **argv);
	static int NextRow(sqlite3_vtab_cursor*);
	static int Eof(sqlite3_vtab_cursor*);
	static int GetCurrentColumnValue(sqlite3_vtab_cursor*, sqlite3_context*, int);
	static int GetRow(sqlite3_vtab_cursor*, sqlite3_int64 *pRowid);

	static int CallbackResultSet(void *data, int argc, char **argv, char **azColName);

};
