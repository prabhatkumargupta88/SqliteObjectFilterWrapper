//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: Object.cpp
///     Author	: Prabhat Kumar Gupta
///
/////////////////////////////////////////////////////////////////////////////////////////
#include "object.h"
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

CObject::~CObject(void)
{
}

CObject::CObject(int p_nObjectID)
{
	m_nObjectID = p_nObjectID;

	std::ostringstream stream;
	stream << "ObjectName(" << rand() % TOTAL_OBJECT_COUNT <<")";
	m_strObjectName = stream.str();

	std::ostringstream streamComment;
	streamComment << "ObjectComment[" << rand() % TOTAL_OBJECT_COUNT << "]";
	m_strObjectComments = streamComment.str();

	m_lObjectCredits = (m_nObjectID * 100) / (1+(rand() % TOTAL_OBJECT_COUNT));
}


int CObject::GetObjectID(void) const
{
	return m_nObjectID;
}
string CObject::GetObjectName(void) const
{
	return m_strObjectName;
}
string CObject::GetObjectComments(void) const
{
	return m_strObjectComments;
}
long CObject::GetObjectCredits(void) const
{
	return m_lObjectCredits;
}

void CObject::DisplayObjectDetails(void){
	cout<< " " << m_nObjectID <<" -> "<< m_strObjectName << " -> " << m_strObjectComments << " -> " << m_lObjectCredits << endl;
}

void CObject::CreateObjectList(vector<CObject*> &p_vecObjectIn)
{
	// initialize randomizer
	srand(time(NULL));	

	cout << "\n LIST OF ALL OBJECTS : \n OBJECT_ID :: OBJECT_NAME :: OBJECT_COMMENT :: OBJECT_CREDITS\n" << endl;

	for(int i = 0; i < TOTAL_OBJECT_COUNT; i++)
	{
		CObject *pObject = new CObject(i);
		p_vecObjectIn.push_back(pObject);
	}
}

void CObject::DisplayObjectList(vector<CObject*> &p_vecObjectIn)
{	
	vector<CObject*>::iterator itr = p_vecObjectIn.begin();
	while(itr != p_vecObjectIn.end())
	{
		CObject *pObject = dynamic_cast<CObject*>(*itr);
		pObject->DisplayObjectDetails();
		itr++;
	}
}

void CObject::DestroyObjectList(vector<CObject*> &p_vecObjectIn)
{
	// Free all objects from heap
	vector<CObject*>::iterator itr = p_vecObjectIn.begin();
	itr = p_vecObjectIn.begin();
	while(itr != p_vecObjectIn.end())
	{
		CObject *pObject = dynamic_cast<CObject*>(*itr);
		delete pObject;
		itr++;
	}
}