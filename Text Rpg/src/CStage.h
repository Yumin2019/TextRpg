#pragma once

#include "value.h"

class CStage
{
public:
	CStage();
	~CStage();

private:
	enum CHOICE
	{
		CHOICE_NONE,
		CHOICE_ATTACK,
		CHOICE_POTION,
		CHOICE_BACK
	};

private:
	friend class CStageManager;
	friend class CObjManager;
private: 

	char						strStageName[NAME_SIZE_MAX];
	vector<class CMonster*>		m_vecMonster;


	int		iMonsterCount;
	int		iStageNumber;




public:

	void DeleteVecter()
	{
		Safe_Delete_VecList(m_vecMonster);
	}
	char* GetStageName()
	{
		return strStageName;
	}
	void Run();
	bool Init(int iNumber);
	void SetStageName(const char* strName)
	{
		strcpy_s(strStageName, NAME_SIZE_MAX - 1, strName);
		iMonsterCount = 0; 
	}

	void SetStage(int StageNumber);

	int OutPutMenu();

	void Render();

	void Save(class CFilestream* file);
	void Load(class CFilestream* file);
};

