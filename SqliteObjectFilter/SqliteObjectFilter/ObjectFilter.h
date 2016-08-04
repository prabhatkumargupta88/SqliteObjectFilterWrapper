//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: ObjectFilter.h
///     Author	: Prabhat Kumar Gupta, STS
///
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Object.h"
#include "ObjectFilterBase.h"
#include <string>
#include <vector>

enum EOBJECTPROPERTIES
{
	OBJECT_REFERENCE,
	OBJECT_ID,
	OBJECT_NAME,
	OBJECT_COMMENTS,
	OBJECT_CREDITS
};

/// <summary>
/// Class to provide filter capabilities for Objects.
/// Filtering mechanism is provided using sqlite.
/// <summary>
class CObjectFilter : public CObjectFilterBase
{
public:
	CObjectFilter();
	~CObjectFilter();

	std::string GetVTableQueryString();
	int NextRow(sqlite3_vtab_cursor*);
	int GetCurrentColumnValue(sqlite3_vtab_cursor*, sqlite3_context*, int);
	int GetFilteredResult(IN char *strFilter, IN void* p_ObjectListIn, OUT void* p_ObjectListOut);

private:
	vector<CObject*> *m_vecObjectIn;
};
