//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: ObjectFilterBase.h
///     Author	: Prabhat Kumar Gupta
///
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "IObjectfilter.h"
#include "IObjectFilterInternal.h"


#define STRING_SIZE 512

/// <summary>
/// Base class for filtering objects.
/// <summary>
class CObjectFilterBase : public IObjectFilterInternal, public IObjectFilter
{
public:
	virtual ~CObjectFilterBase(void);
	CObjectFilterBase(void);

	virtual int GetFilteredResult(IN char *strFilter, IN void* p_ObjectListIn, OUT void* p_ObjectListOut) PURE;

	virtual std::string GetVTableQueryString() PURE;
	virtual int NextRow(sqlite3_vtab_cursor*) PURE;
	virtual int GetCurrentColumnValue(sqlite3_vtab_cursor*, sqlite3_context*, int) PURE;
	virtual int SetFilterParameters(sqlite3_vtab_cursor *p_vtc, int idxNum, const char *idxStr, int argc, sqlite3_value **argv);

protected:
	int GetFilteredObject(IN char *objModelName, IN char *strFilter, IN void* p_ObjectListIn, OUT void* p_ObjectListOut);
};
