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


		// ���������� ����.

		m_vecStage[iMenu-1]->Run();
		iMenu = 0;
	}
}

bool CStageManager::Init()
{
	CFilestream file;


	if (file.Open("Stage.stg", "rb"))
	{
		// ���������� ���� �д´�.

		int iSize = 0;

		file.Read(&iSize, 4);

		// �������� ���� ���� �д´�

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
				pStage->SetStageName("����� ��");
				pStage->iStageNumber = m_vecStage.size();
				m_vecStage.push_back(pStage);
				break;
			case 1:
				pStage->SetStageName("��ũ�� ��");
				pStage->iStageNumber = m_vecStage.size();
				m_vecStage.push_back(pStage);
				break;
			case 2:
				pStage->SetStageName("�巡���� ��");
				pStage->iStageNumber = m_vecStage.size();
				m_vecStage.push_back(pStage);
				break;
			}
		}
	}

	// ���������� �ҷ��� ���Ŀ��� ������ �ʿ��ϴ�.
	int iSize = m_vecStage.size();

	for (int i = 0; i < iSize; ++i)
	{
		m_vecStage[i]->Init(i);
	}
	return true;
}

int CStageManager::OutPutMenu()
{
	// ���������� �����Ѵ�.

	system("cls");

	cout << endl;
	SHOW("����");
	int iSize = m_vecStage.size();

	for (int i = 0; i < iSize; ++i)
	{
		SHOW2(i + 1 << ". " << m_vecStage[i]->strStageName);
	}
	SHOW2(iSize + 1 << ". �ڷΰ���");
	cout << "������ ������ �����ϼ��� : ";
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

	SHOW("�������� ���");
	for (int i = 0; i < iSize; ++i)
	{
		SHOW2(i + 1);
		m_vecStage[i]->DeleteVecter(); // �ϴ� ���ο� ���� ���͸� ������.
		m_vecStage[i]->iMonsterCount = 0; // ���ͼ��� 0���� 
		m_vecStage[i]->Init(i); // ��� ���͸� ������� (�Ͽ� ������ 0 �� ����) push�۾��� �����Ѵ�.
		m_vecStage[i]->Render(); // �� �����͸� ������ ����Ѵ�.
								// �̰��� ����� �� ���� �ݺ��Ѵ�.
		cout << endl;
	}

	system("pause");
}

void CStageManager::CreateStage()
{
	CStage*		pStage = new CStage;

	char * strName = new char[NAME_SIZE_MAX];
	Ignore();
	cout << "�������� �̸� : ";
	cin.getline(strName, NAME_SIZE_MAX - 1, '\n');
	// �������� �ѹ��� �ڵ����� ���� count = 0

	pStage->SetStageName(strName);
	SAFE_DELETE_ARRAY(strName);

	int iNumber = m_vecStage.size();

	pStage->SetStage(iNumber);
	m_vecStage.push_back(pStage);
	cout << "���������� �߰��߽��ϴ�." << endl;
	system("pause");
}

void CStageManager::StageSave()
{
	system("cls");
	CFilestream file("Stage.stg", "wb");

	// ���������� ���� ����Ѵ�.

	int iSize = m_vecStage.size();

	file.Write(&iSize, 4);

	// �������� ���� ���� ����

	for (int i = 0; i < iSize; ++i)
	{
		m_vecStage[i]->Save(&file);
	}

	cout << "���������� ����Ǿ����ϴ�. " << endl;
	system("pause");
}

void CStageManager::StageLoad()
{
	system("cls");
	Safe_Delete_VecList(m_vecStage);
	// �о�� �� �츮�� ������ �ִ� �����ʹ� ������ �Ѵ�.

	CFilestream file("Stage.stg", "rb");

	// ���������� ���� �д´�.

	int iSize = 0;

	file.Read(&iSize, 4);

	// �������� ���� ���� �д´�

	for (int i = 0; i < iSize; ++i)
	{
		CStage* pStage = new CStage;

		pStage->Load(&file);

		m_vecStage.push_back(pStage);
	}

	cout << "���������� �ҷ��Խ��ϴ�." << endl;
	system("pause");
}

void CStageManager::DeleteVector()
{
	Safe_Delete_VecList(m_vecStage);
}
