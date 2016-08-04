//////////////////////////////////////////////////////////////////////////////////////////
///
///     Project : SqliteObjectFilter
///     File	: UserObjectFilter.cpp
///     Author	: Prabhat Kumar Gupta, STS
///
//////////////////////////////////////////////////////////////////////////////////////////

#include "Object.h"
#include "ObjectFilter.h"

#include <iostream>
#include <vector>
using namespace std;


int main()
{
	vector<CObject*> vecObjectList;
	vector<CObject*> vecObjectListResult;

	// Creating List of Object
	CObject::CreateObjectList(vecObjectList);
	CObject::DisplayObjectList(vecObjectList);

	// Creating Instance for ObjectFilteration
	IObjectFilter *pObjectFilter = new CObjectFilter();
	pObjectFilter->GetFilteredResult("OBJECT_ID = 2", &vecObjectList, &vecObjectListResult);

	//LIMIT 2;
	//LIMIT 3 OFFSET 1

	cout << "\n DISPLAY FILTERED RESULT WITH QUERY 'OBJECT_ID = 2'\n";
	// Display List of Objects
	CObject::DisplayObjectList(vecObjectListResult);

	// Clear Query Result
	vecObjectListResult.clear();


	// Destroy List of Objects
	CObject::DestroyObjectList(vecObjectList);

	getchar();
	return 0;
}