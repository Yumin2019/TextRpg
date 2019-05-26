#pragma once

#include "value.h"
#include "Flag.h"
class CObj
{
public:
	CObj(const CObj& p);
	CObj();
	virtual ~CObj();

public:
	char				strName[NAME_SIZE_MAX];
	OBJECT_TYPE			OT_type;


public:
	virtual void Save(class CFilestream* file);
	virtual void Load(class CFilestream* file);


	virtual void Run() = 0;
	virtual bool Init() = 0;

	void SetName(const char* pName, OBJECT_TYPE eType)
	{
		strcpy_s(strName, NAME_SIZE_MAX-1 ,pName);
		OT_type = eType;
	}

	OBJECT_TYPE GetObjectType()
	{
		return OT_type;
	}

	
	char* GetName()
	{
		return strName;
	}

	virtual CObj* Clone() = 0;
	
};

