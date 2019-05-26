#pragma once

#include "value.h"
#include "Flag.h"


class CCore
{
public:
	CCore();
	~CCore();
	
	DECLARE_SINGLE(CCore);
	
private:
	enum MAIN_MENU
	{
		MM_NONE,
		MM_MAP,
		MM_STORE,
		MM_INVENTORY,
		MM_INFORMATION,
		MM_HOSPITAL,
		MM_SAVE,
		MM_LOAD,
		MM_BACK
	};

	enum STORE_TYPE
	{
		ST_NONE,
		ST_WEAPON,
		ST_ARMOR,
		ST_RUBY,
		ST_SELL,
		ST_BACK
	};
public:
	void Run();
	bool Init();
	bool Load();
	int OutPutMenu();

	int SaveOutPutMenu();
	int LoadOutPutMenu();

	void CCore::SavePlayer(const char* pName);
	void		LoadPlayer(const char* pName);

	void Store();
	int StoreOutPutMenu();

	void SaveDelete();
	
};

