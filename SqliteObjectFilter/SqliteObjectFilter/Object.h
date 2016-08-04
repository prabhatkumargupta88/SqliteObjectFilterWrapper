//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: Object.h
///     Author	: Prabhat Kumar Gupta
///
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <vector>
using namespace std;

#define TOTAL_OBJECT_COUNT 10


class CObject 
{
public :

	~CObject(void);
	CObject(int p_nObjectID);
	
	int GetObjectID(void) const;
	string GetObjectName(void) const;
	string GetObjectComments(void) const;
	long GetObjectCredits(void) const;

	void DisplayObjectDetails(void);
	static void CreateObjectList(vector<CObject*> &p_vecObjectIn);
	static void DestroyObjectList(vector<CObject*> &p_vecObjectIn);
	static void DisplayObjectList(vector<CObject*> &p_vecObjectIn);

private :

	int m_nObjectID;
	string m_strObjectName;
	string m_strObjectComments;
	long m_lObjectCredits;
};


// TODO: reference additional headers your program requires here
