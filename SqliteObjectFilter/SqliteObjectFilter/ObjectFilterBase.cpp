//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: ObjectFilterBase.cpp
///     Author	: Prabhat Kumar Gupta
///
//////////////////////////////////////////////////////////////////////////////////////////
#include "ObjectFilterBase.h"
#include "SqliteModule.h"
#include <sstream>
#include <stdio.h>


/// <summary>
/// CObjectFilterBase Constructor
/// <summary>
CObjectFilterBase::CObjectFilterBase()
{
}

/// <summary>
/// CObjectFilterBase Destructor
/// <summary>
CObjectFilterBase::~CObjectFilterBase()
{
}

/// </summary>
/// Function to filter Object based on query and Objectlist and provide result 
/// Format the value according to a format string.
/// objModelName:		Buffer with Object Type
/// queryIn:        Buffer with the format string
/// readResultIn:   Target buffer with length LengthOfTarget
/// filteredResultOut: Size of buffer pTarget in bytes
/// The function returns the number of written bytes into target
/// </summary>
/// <returns>Return code. Set to -1 in case of error or exception.</returns>
int CObjectFilterBase::GetFilteredObject(IN char *objModelName, IN char *strFilter, IN void* p_ObjectListIn, OUT void* p_ObjectListOut)
{
	int rc = SQLITE_ERROR;
	sqlite3 *pDatabase = NULL;

	// Open an in-memory database 
	if (sqlite3_open(":memory:", &pDatabase) != SQLITE_OK) {
		exit(1);
	}

	// Enable loadable modules (DSOs) 
	sqlite3_enable_load_extension(pDatabase, 1);

	SObjectVTab *pObjectFilter = new SObjectVTab();
	pObjectFilter->m_pMyObjectFilter = this;

	char strModule[STRING_SIZE] = { 0 };
	sprintf_s(strModule, "%s_MODULE", objModelName);

	/// </summary>
	/// Declaring Sqlite Module APIs.
	/// </summary>
	sqlite3_module Sqlite_module =
	{
		CSqlite_module::iVersion,				 /* iVersion */
		CSqlite_module::CreateVirtualTable,      /* xCreate       - create a vtable */
		CSqlite_module::ConnectVirtualTable,     /* xConnect      - associate a vtable with a connection */
		CSqlite_module::BestIndexVirtualTable,   /* xBestIndex    - best index */
		CSqlite_module::DisconnectVirtualTable,  /* xDisconnect   - disassociate a vtable with a connection */
		CSqlite_module::DestroyVirtualTable,     /* xDestroy      - destroy a vtable */
		CSqlite_module::OpenCursor,				 /* xOpen         - open a cursor */
		CSqlite_module::CloseCursor,			 /* xClose        - close a cursor */
		CSqlite_module::FilterVirtualTable,      /* xFilter       - configure scan constraints */
		CSqlite_module::NextRow,				 /* xNext         - advance a cursor */
		CSqlite_module::Eof,					 /* xEof          - inidicate end of result set*/
		CSqlite_module::GetCurrentColumnValue,   /* xColumn       - read data */
		CSqlite_module::GetRow,					 /* xRowid        - read data */

		NULL,									/* xUpdate       - write data */
		NULL,									/* xBegin        - begin transaction */
		NULL,									/* xSync         - sync transaction */
		NULL,									/* xCommit       - commit transaction */
		NULL,									/* xRollback     - rollback transaction */

		NULL									/* xFindFunction - function overloading <FindFunctionVirtualTable>*/
	};

	// Register Tag Module
	if (sqlite3_create_module(pDatabase, strModule, &Sqlite_module, pObjectFilter) != SQLITE_OK)
	{
		return SQLITE_ERROR;
	}

	char *msg;
	char strCreateQuery[STRING_SIZE] = { 0 };
	sprintf_s(strCreateQuery, "CREATE VIRTUAL TABLE %s_VIRTUAL_TABLE USING %s_MODULE;", objModelName, objModelName);

	// Create Tag Virtual Table. 
	rc = sqlite3_exec(pDatabase, strCreateQuery, NULL, NULL, &msg);
	if (rc != SQLITE_OK)
	{
		return SQLITE_ERROR;
	}

	try
	{
		char strFilterQuery[STRING_SIZE] = { 0 };
		sprintf_s(strFilterQuery, "SELECT OBJECT_REFERENCE FROM %s_VIRTUAL_TABLE WHERE %s ;", objModelName, strFilter);

		// Run Query to filter Object.
		rc = sqlite3_exec(pDatabase, strFilterQuery, CSqlite_module::CallbackResultSet, p_ObjectListOut, &msg);
		if (rc != SQLITE_OK)
		{
			return SQLITE_ERROR;
		}
	}
	catch (std::exception ex)
	{
		return -1;
	}

	// Drop Tag Virtual Table
	char strDropQuery[STRING_SIZE] = { 0 };
	sprintf_s(strDropQuery, "DROP TABLE %s_VIRTUAL_TABLE;", objModelName);

	rc = sqlite3_exec(pDatabase, strDropQuery, NULL, NULL, &msg);
	if (rc != SQLITE_OK)
	{
		return SQLITE_ERROR;
	}
	sqlite3_close(pDatabase);

	return SQLITE_OK;
}

/// </summary>
/// Function to initialize parameters
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns
int CObjectFilterBase::SetFilterParameters(sqlite3_vtab_cursor *p_vtc, int idxNum, const char *idxStr, int argc, sqlite3_value **argv)
{
	// Initialize the cursor structure.
	data_cursor *pCursor = (data_cursor*)p_vtc;

	// Zero rows returned thus far.
	pCursor->count = 0;
	// Have not reached end of set.
	pCursor->eof = 0;
	// Move cursor to first row.
	return NextRow(p_vtc);
}
