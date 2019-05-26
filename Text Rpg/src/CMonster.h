#pragma once
#include "CCharacter.h"
#include "CStage.h"
class CMonster :
	public CCharacter
{
public:
	CMonster(CMonster& p);
	CMonster();
	~CMonster();


private:


public:
	int			iGoldMin;
	int			iGoldMax;
	int			iStageNumber;

public:
	virtual void Run();
	virtual bool Init();

	int GetDropGold()
	{
		return rand() % (1 + iGoldMax - iGoldMin)
			+ iGoldMin;
	}


	void SetDropGold(int iMin, int iMax)
	{
		iGoldMax = iMax;
		iGoldMin = iMin;
	}

	void SetStage(int iNumber)
	{
		iStageNumber = iNumber;
	}

	void Render();

	CMonster* Clone();


	virtual void Save(class CFilestream* file);
	virtual void Load(class CFilestream* file);

};

