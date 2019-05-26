#pragma once

#include "value.h"
class CEditorCore
{
public:
	CEditorCore();
	~CEditorCore();
	DECLARE_SINGLE(CEditorCore);

private:

	enum EDIT_ITEM
	{
		IT_NONE,
		IT_WEAPON,
		IT_ARMOR,
		IT_RUBY,
		IT_BACK
	};
	enum EDIT_MENU
	{
		ED_NONE,
		ED_INSERT,
		ED_SAVE,
		ED_LIST,
		ED_LOAD,
		ED_DELETE_ALL,
		ED_BACK
	};

	enum MAIN_MENU
	{
		MM_NONE,
		MM_MONSTER,
		MM_ITEM,
		MM_STAGE,
		MM_LEVEL_UP_EXP,
		MM_EXIT
	};
public:

	void ArmorStore();
	void WeaponStore();
	void RubyStore();

	int OutPutStoreMenu();
	int OutPutEditMenu();
	void Run();
	bool Init();
	int OutPutMenu();

	void MonsterList();

};

