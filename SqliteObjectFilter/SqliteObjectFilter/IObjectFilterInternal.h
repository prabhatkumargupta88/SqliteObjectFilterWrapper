//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: IObjectFilterInternal.h
///     Author	: Prabhat Kumar Gupta
///
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "sqlite3.h"
#include <string>

#define IN
#define OUT
#define PURE =0

/// <summary>
/// Interface having methods to override internal Sqlite APIs for different different objects.
/// <summary>
class IObjectFilterInternal {
public:
	virtual ~IObjectFilterInternal() {};

	virtual std::string GetVTableQueryString() PURE;
	virtual int NextRow(sqlite3_vtab_cursor*) PURE;
	virtual int GetCurrentColumnValue(sqlite3_vtab_cursor*, sqlite3_context*, int) PURE;
	virtual int SetFilterParameters(sqlite3_vtab_cursor *p_vtc, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) PURE;

};