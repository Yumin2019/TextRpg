#include "CStageManager.h"
#include "CStage.h"
#include "CFilestream.h"

DEFINITION_SINGLE(CStageManager);

CStageManager::CStageManager()
{
}


CStageManager::~CStageManager()
{
	Safe_Delete_VecList(m_vecStage);
}

void CStageManager::Run()
{
	int iMenu = 0;
	while (iMenu == 0)
	{
		iMenu = OutPutMenu();

		if (iMenu == 999)
			return;

		else if (iMenu == 0)
			continue;


		// 스테이지를 선택.

		m_vecStage[iMenu-1]->Run();
		iMenu = 0;
	}
}

bool CStageManager::Init()
{
	CFilestream file;


	if (file.Open("Stage.stg", "rb"))
	{
		// 스테이지의 수를 읽는다.

		int iSize = 0;

		file.Read(&iSize, 4);

		// 스테이지 수에 맞춰 읽는다

		for (int i = 0; i < iSize; ++i)
		{
			CStage* pStage = new CStage;

			pStage->Load(&file);

			m_vecStage.push_back(pStage);
		}
	}


	else
	{
		int iSize = 0;
		for (int i = 0; i < 3; ++i)
		{
			CStage*		pStage = new CStage;

			switch (i)
			{
			case 0:
				pStage->SetStageName("고블린의 집");
				pStage->iStageNumber = m_vecStage.size();
				m_vecStage.push_back(pStage);
				break;
			case 1:
				pStage->SetStageName("오크의 집");
				pStage->iStageNumber = m_vecStage.size();
				m_vecStage.push_back(pStage);
				break;
			case 2:
				pStage->SetStageName("드래곤의 집");
				pStage->iStageNumber = m_vecStage.size();
				m_vecStage.push_back(pStage);
				break;
			}
		}
	}

	// 스테이지를 불러온 이후에는 셋팅이 필요하다.
	int iSize = m_vecStage.size();

	for (int i = 0; i < iSize; ++i)
	{
		m_vecStage[i]->Init(i);
	}
	return true;
}

int CStageManager::OutPutMenu()
{
	// 스테이지를 선택한다.

	system("cls");

	cout << endl;
	SHOW("던전");
	int iSize = m_vecStage.size();

	for (int i = 0; i < iSize; ++i)
	{
		SHOW2(i + 1 << ". " << m_vecStage[i]->strStageName);
	}
	SHOW2(iSize + 1 << ". 뒤로가기");
	cout << "입장할 던전을 선택하세요 : ";
	int iMenu = Input<int>();

	if (iMenu <= 0 || iMenu > iSize + 1)
		return 0;

	else if (iSize + 1 == iMenu)
		return 999;

	return iMenu;

	system("pause");
}

void CStageManager::StageList()
{
	system("cls");
	int iSize = m_vecStage.size();

	SHOW("스테이지 출력");
	for (int i = 0; i < iSize; ++i)
	{
		SHOW2(i + 1);
		m_vecStage[i]->DeleteVecter(); // 일단 내부에 몬스터 벡터를 버린다.
		m_vecStage[i]->iMonsterCount = 0; // 몬스터수를 0으로 
		m_vecStage[i]->Init(i); // 모든 몬스터를 대상으로 (하여 없으면 0 안 잡음) push작업을 수행한다.
		m_vecStage[i]->Render(); // 그 데이터를 가지고 출력한다.
								// 이것을 출력할 때 마다 반복한다.
		cout << endl;
	}

	system("pause");
}

void CStageManager::CreateStage()
{
	CStage*		pStage = new CStage;

	char * strName = new char[NAME_SIZE_MAX];
	Ignore();
	cout << "스테이지 이름 : ";
	cin.getline(strName, NAME_SIZE_MAX - 1, '\n');
	// 스테이지 넘버는 자동으로 데입 count = 0

	pStage->SetStageName(strName);
	SAFE_DELETE_ARRAY(strName);

	int iNumber = m_vecStage.size();

	pStage->SetStage(iNumber);
	m_vecStage.push_back(pStage);
	cout << "스테이지를 추가했습니다." << endl;
	system("pause");
}

void CStageManager::StageSave()
{
	system("cls");
	CFilestream file("Stage.stg", "wb");

	// 스테이지의 수를 기록한다.

	int iSize = m_vecStage.size();

	file.Write(&iSize, 4);

	// 스테이지 수에 맞춰 저장

	for (int i = 0; i < iSize; ++i)
	{
		m_vecStage[i]->Save(&file);
	}

	cout << "스테이지가 저장되었습니다. " << endl;
	system("pause");
}

void CStageManager::StageLoad()
{
	system("cls");
	Safe_Delete_VecList(m_vecStage);
	// 읽어올 때 우리가 가지고 있던 데이터는 버려야 한다.

	CFilestream file("Stage.stg", "rb");

	// 스테이지의 수를 읽는다.

	int iSize = 0;

	file.Read(&iSize, 4);

	// 스테이지 수에 맞춰 읽는다

	for (int i = 0; i < iSize; ++i)
	{
		CStage* pStage = new CStage;

		pStage->Load(&file);

		m_vecStage.push_back(pStage);
	}

	cout << "스테이지를 불러왔습니다." << endl;
	system("pause");
}

void CStageManager::DeleteVector()
{
	Safe_Delete_VecList(m_vecStage);
}
