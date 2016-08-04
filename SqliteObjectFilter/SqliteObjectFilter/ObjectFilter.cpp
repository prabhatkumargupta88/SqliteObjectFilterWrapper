//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: ObjectFilter.h
///     Author	: Prabhat Kumar Gupta, STS
///
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Object.h"
#include "ObjectFilter.h"
#include "SqliteModule.h"
#include <vector>


/// <summary>
/// CObjectFilter Constructor
/// <summary>
CObjectFilter::CObjectFilter()
{
}

/// <summary>
/// CObjectFilter Destructor
/// <summary>>
CObjectFilter::~CObjectFilter()
{
}

/// </summary>
/// Function to filter objects based on query and Objectlist and provide result 
/// Format the value according to a format string.
/// queryIn:         Buffer with the format string
/// readResultIn:        Target buffer with length LengthOfTarget
/// filteredResultOut: Size of buffer pTarget in bytes
/// The function returns the number of written bytes into target
/// </summary>
/// <returns>Return code. Set to -1 in case of error or exception.</returns>
int CObjectFilter::GetFilteredResult(IN char *strFilter, IN void* p_ObjectListIn, OUT void* p_ObjectListOut)
{
	m_vecObjectIn = reinterpret_cast<vector<CObject*>*> (p_ObjectListIn);

	return GetFilteredObject("OBJECT", strFilter, p_ObjectListIn, p_ObjectListOut);
}

/// </summary>
/// Function to get query to create object table based on its properties.
/// </summary>
/// <returns>string (query)</returns>
std::string CObjectFilter::GetVTableQueryString()
{
	return "CREATE TABLE OBJECT_TABLE  (OBJECT_REFERENCE			BLOB,			\
										OBJECT_ID					INT,			\
										OBJECT_NAME					VARCHAR(100),	\
										OBJECT_COMMENT				VARCHAR(100),	\
										OBJECT_CREDITS				DOUBLE);";
}

/// </summary>
/// Function to iterate to next Object in list.
/// </summary>
/// <returns>Int (Next Row)</returns>
int CObjectFilter::NextRow(sqlite3_vtab_cursor* cur)
{
	data_cursor *pCursor = (data_cursor*)cur;

	// Increment the current row count. 
	pCursor->count += 1;

	// Arbitrary contstraint: when we get to all rows, then stop. 
	if (pCursor->count == (m_vecObjectIn->size() + 1))
	{
		pCursor->eof = 1;
	}

	return SQLITE_OK;
}

/// </summary>
/// Function to get respective column value of current row
/// </summary>
/// <returns>Result code (SQLITE_OK/SQLITE_ERROR)</returns>
int CObjectFilter::GetCurrentColumnValue(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int columnID)
{
	// Just return the ordinal of the column requested. 
	data_cursor *p_cur = (data_cursor*)cur;

	// Just use the current row count as the rowid. 
	CObject *pObject = m_vecObjectIn->at((p_cur->count) - 1);

	switch (columnID)
	{
	case OBJECT_REFERENCE:
	{
		char strObjectAddress[32] = { 0 };

		// Storing reference into 'long int' format.
		sprintf_s(strObjectAddress, "%ld", pObject);

		sqlite3_result_text(ctx, (char*)strObjectAddress, (int)strlen((char*)strObjectAddress), SQLITE_TRANSIENT);
		break;
	}
	case OBJECT_ID:
	{
		sqlite3_result_int(ctx, pObject->GetObjectID());
		break;
	}
	case OBJECT_NAME:
	{
		sqlite3_result_text(ctx, pObject->GetObjectName().c_str(), (int)strlen(pObject->GetObjectName().c_str()), SQLITE_TRANSIENT);
		break;
	}
	case OBJECT_COMMENTS:
	{
		sqlite3_result_text(ctx, pObject->GetObjectComments().c_str(), (int)strlen(pObject->GetObjectComments().c_str()), SQLITE_TRANSIENT);
		break;
	}
	case OBJECT_CREDITS:
	{
		sqlite3_result_int(ctx, pObject->GetObjectCredits());
		break;
	}
	default:
	{
		sqlite3_result_int(ctx, columnID); // Invalid ColumnID
		break;
	}
	};

	return SQLITE_OK;
}
