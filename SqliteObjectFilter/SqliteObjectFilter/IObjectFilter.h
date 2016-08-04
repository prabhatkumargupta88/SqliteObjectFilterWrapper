//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: IObjectFilter.h
///     Author	: Prabhat Kumar Gupta
///
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define IN
#define OUT
#define PURE =0

/// <summary>
/// Interface to filter objects based on filter string.
/// <summary>
class IObjectFilter {
public:
	virtual ~IObjectFilter() {};
	virtual int GetFilteredResult(IN char *strFilter, IN void* p_ObjectListIn, OUT void* p_ObjectListOut) PURE;
};
