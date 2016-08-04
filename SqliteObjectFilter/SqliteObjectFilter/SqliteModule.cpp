//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: SqliteModule.cpp
///     Author	: Prabhat Kumar Gupta, STS
///
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <vector>
#include "sqlite3.h"
#include "SqliteModule.h"
#include "ObjectFilterBase.h"
#include "Object.h"
#include <iostream>


int CSqlite_module::iVersion = 1;

/// </summary>
/// Funtion to create virtual table and allocate memory
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns>
int CSqlite_module::CreateVirtualTable(sqlite3 *db,
	void *p_aux,
	int argc, const char *const*argv,
	sqlite3_vtab **pp_vt,
	char **pzErr)
{
	int rc = SQLITE_ERROR;

	SObjectVTab *pObjectVTabStruct = static_cast<SObjectVTab*>(p_aux);
	IObjectFilterInternal *pObjectFilter = static_cast<IObjectFilterInternal*>(pObjectVTabStruct->m_pMyObjectFilter);
	rc = sqlite3_declare_vtab(db, pObjectFilter->GetVTableQueryString().c_str());

	if (rc != SQLITE_OK)
	{
		return SQLITE_ERROR;
	}

	*pp_vt = static_cast<sqlite3_vtab*>(p_aux);

	return SQLITE_OK;
}

/// </summary>
/// Function to connect/create virtual table
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns>
int CSqlite_module::ConnectVirtualTable(sqlite3 *db, void *p_aux,
	int argc, char const*const*argv,
	sqlite3_vtab **pp_vt, char **pzErr)
{
	return CreateVirtualTable(db, p_aux, argc, argv, pp_vt, pzErr);
}

/// </summary>
/// Funtion to disconnect 
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns>
int CSqlite_module::DisconnectVirtualTable(sqlite3_vtab *pVtab)
{
	try
	{
		delete pVtab;
	}
	catch (std::exception e)
	{
		return SQLITE_ERROR;
	}

	return SQLITE_OK;
}

/// </summary>
/// Function to free all memory
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns>
int CSqlite_module::DestroyVirtualTable(sqlite3_vtab *pVtab)
{
	try
	{
		delete pVtab;
	}
	catch (std::exception e)
	{
		return SQLITE_ERROR;
	}

	return SQLITE_OK;
}

/// </summary>
/// Function to open a cursor
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns>
int CSqlite_module::OpenCursor(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **pp_cursor)
{
	data_cursor *p_cur = (data_cursor*)sqlite3_malloc(sizeof(data_cursor));
	*pp_cursor = (sqlite3_vtab_cursor*)p_cur;

	return (p_cur ? SQLITE_OK : SQLITE_NOMEM);
}

/// </summary>
/// Function to close cursur
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns>
int CSqlite_module::CloseCursor(sqlite3_vtab_cursor *cur)
{
	data_cursor *p_cur = (data_cursor*)cur;
	sqlite3_free(p_cur);

	return SQLITE_OK;
}

/// </summary>
/// Function to indicate end of rows/records
/// </summary>
/// <returns>int(isEof or not)</returns>
int CSqlite_module::Eof(sqlite3_vtab_cursor *cur)
{
	return ((data_cursor*)cur)->eof;
}

/// </summary>
/// Function to get row
/// </summary>
/// <returns>Int (Current Row)</returns>
int CSqlite_module::GetRow(sqlite3_vtab_cursor *cur, sqlite_int64 *p_rowid)
{
	data_cursor *p_cur = (data_cursor*)cur;

	/* Just use the current row count as the rowid. */
	*p_rowid = p_cur->count;

	return SQLITE_OK;
}

/// </summary>
/// Function to iterate to next row.
/// </summary>
/// <returns>Int (Next Row)</returns>
int CSqlite_module::NextRow(sqlite3_vtab_cursor *cur)
{
	SObjectVTab *pObjectVTabStruct = static_cast<SObjectVTab*>(cur->pVtab);
	IObjectFilterInternal *pObjectFilter = static_cast<IObjectFilterInternal*>(pObjectVTabStruct->m_pMyObjectFilter);
	return pObjectFilter->NextRow(cur);
}

/// </summary>
/// Function to get respective column value of current row
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns>
int CSqlite_module::GetCurrentColumnValue(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int columnID)
{
	SObjectVTab *pObjectVTabStruct = static_cast<SObjectVTab*>(cur->pVtab);
	IObjectFilterInternal *pObjectFilter = static_cast<IObjectFilterInternal*>(pObjectVTabStruct->m_pMyObjectFilter);
	return pObjectFilter->GetCurrentColumnValue(cur, ctx, columnID);
}

/// </summary>
/// Function to initialize parameters
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns>
int CSqlite_module::FilterVirtualTable(sqlite3_vtab_cursor *p_vtc,
	int idxNum, const char *idxStr,
	int argc, sqlite3_value **argv)
{
	SObjectVTab *pObjectVTabStruct = static_cast<SObjectVTab*>(p_vtc->pVtab);
	IObjectFilterInternal *pObjectFilter = static_cast<IObjectFilterInternal*>(pObjectVTabStruct->m_pMyObjectFilter);
	return pObjectFilter->SetFilterParameters(p_vtc, idxNum, idxStr, argc, argv);
}

/// </summary>
/// Function to initialize parameters for Ordering
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns>
int CSqlite_module::BestIndexVirtualTable(sqlite3_vtab *tab, sqlite3_index_info *pIdxInfo)
{
	tab->zErrMsg = NULL;
	return SQLITE_OK;
}

/// </summary>
/// Callback Function to Store ResultSet
/// </summary>
/// <returns>int</returns>
int CSqlite_module::CallbackResultSet(void *data, int argc, char **argv, char **azColName)
{
	int i;

	for (i = 0; i < argc; i++)
	{
		if (strcmp(azColName[i], "OBJECT_REFERENCE") == 0)
		{
			char object_address[32] = { 0 };
			memcpy(object_address, argv[i], strlen(argv[i]));

			CObject *pFilteredObject = (CObject*)_atoi64(object_address);
			vector<CObject*> *pVecObjectFilter = static_cast<vector<CObject*>*>(data);
			pVecObjectFilter->push_back(pFilteredObject);
		}
	}
	return SQLITE_OK;
}
