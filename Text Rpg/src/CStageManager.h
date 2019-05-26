#pragma once

#include "value.h"


class CStageManager
{
public:
	CStageManager();
	~CStageManager();

	DECLARE_SINGLE(CStageManager);


	vector<class CStage*>		m_vecStage;


public:
	void Run();
	bool Init();

	int OutPutMenu();

	void StageList();
	void CreateStage();

	void StageSave();
	void StageLoad();

	void DeleteVector();
};

