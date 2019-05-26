#include "CObjManager.h"
#include "CFilestream.h"
#include "CStageManager.h"
#include "CStage.h" 

DEFINITION_SINGLE(CObjManager);

CObjManager::CObjManager()
{
}


CObjManager::~CObjManager()
{
	Safe_Delete_VecList(m_tMonster);
	SAFE_DELETE(m_tCurPlayer);
	PlayerSaveVecFree();

	LevelUpTable.clear();
	Safe_Delete_VecList(m_vecWeapon);
	Safe_Delete_VecList(m_vecArmor);
	Safe_Delete_VecList(m_vecRuby);

}

void CObjManager::CreateLevelTable()
{
	// �츮�� ������ �˰� �ֱ� ������ �׳� for�� ������ �ȴ�.

	system("cls");
	cout << endl;
	SHOW("������ ���̺� ����");
	cout << "�ִ� ������ �Է��ϼ���(1 ~ 100): ";
	int iLevel = Input<int>();

	cout << "1 -> 2 �������� �ʿ��� ����ġ�� �Է��ϼ��� : ";
	int One = Input<int>();

	cout << "���� : (���� ���� ����ġ * 2) * A " << endl;

	cout << "���� : A ��õ �� 0.7 " << endl;
	cout << "A(double) �� �Է��ϼ��� : ";
	double A = Input<double>();


	LevelUpTable.push_back(One);

	for (int i = 1; i < iLevel-1; ++i)
	{
		LevelUpTable.push_back(LevelUpTable[i - 1] * 2 * A);
	}

	cout << "������ ���̺��� �����߽��ϴ�." << endl;
	system("pause");
}

void CObjManager::SaveLevelUpTable()
{
	CFilestream file("LevelUpTabel.Lv", "wb");

	// ������ ������ ����.

	int iSize = LevelUpTable.size();

	file.Write(&iSize, 4);

	for (int i = 0; i < iSize; ++i) // ������ ������ŭ ����.
	{
		file.Write(&LevelUpTable[i], sizeof(long long));
	}

	cout << "������ ���ƽ��ϴ�." << endl;
	system("pause");
}

void CObjManager::LoadLevelUpTable()
{

	// �ҷ����� ���� �ϴ� �����͸� ����

	LevelUpTable.clear();
	CFilestream file("LevelUpTabel.Lv", "rb");

	// ������ ������ �д´�.

	int iSize = 0;

	file.Read(&iSize, 4);

	for (int i = 0; i < iSize; ++i) // ������ ������ŭ �о �����Ѵ�.
	{
		long long a;
		file.Read(&a, sizeof(long long));

		LevelUpTable.push_back(a);
	}

	cout << "�ҷ����⸦ ���ƽ��ϴ�." << endl;
	system("pause");
}

void CObjManager::LevelUpTableList()
{
	system("cls");
	cout << endl;
	SHOW("������ ���̺� ���");

	int Max = LevelUpTable.size();

	for (int i = 0; i < Max; ++i)
	{
		SHOW2(i + 1 << " Lv");
		cout << LevelUpTable[i] << endl << endl;
	}

	cout << "����� �������ϴ�." << endl;
	system("pause");
}

void CObjManager::DeleteLevelUpTable()
{
	LevelUpTable.clear(); // �̰��� �Ҵ��� �� ���� �ƴϹǷ� �׳� ������ �ȴ�.

	cout << "��� ����ġ �����͸� �����߽��ϴ�." << endl;
	system("pause");
}

bool CObjManager::InitLoad()
{
	int iSize = 0;

	CFilestream file;

	if (file.Open("Monster.mst", "rb")) // true�� ������ ����.
	{

		file.Read(&iSize, 4);

		for (int i = 0; i < iSize; ++i)
		{
			CMonster*   pMonster = (CMonster*)CreateObject(OT_MONSTER);
			pMonster->Load(&file);
			m_tMonster.push_back(pMonster);
		}
	}
	else
	{
		MonsterInit();
	}

	file.Close();


	if (file.Open("RubyStore.str", "rb"))
	{
		int iSize = 0;

		file.Read(&iSize, 4);

		for (int i = 0; i < iSize; ++i)
		{
			CItem*   pRuby = new CItem;
			pRuby->Load(&file);
			m_vecRuby.push_back(pRuby);
		}
	}

	else
	{
		// ��ű� ����


		CItem*	pItem = (CItem*)CreateObject(OT_RUBY);

		pItem->SetItemInfo(0, 0, 0, 0, 5, 10, 0, 0.2, 0, 0, 0, 1000, 500);
		pItem->SetName("���� ���", OT_RUBY);
		pItem->SetDesc("���� ��");

		m_vecRuby.push_back(pItem);


		pItem = (CItem*)CreateObject(OT_RUBY);

		pItem->SetItemInfo(100, 100, 5, 5, 5, 5, 3, 0, 0, 0, 0, 15000, 10000);
		pItem->SetName("�ʷ� ���", OT_RUBY);
		pItem->SetDesc("�ʷ� ��");

		m_vecRuby.push_back(pItem);

		pItem = (CItem*)CreateObject(OT_RUBY);

		pItem->SetItemInfo(150, 0, 10, 20, 10, 15, 3, 1, 0, 0, 0, 70000, 35500);
		pItem->SetName("�Ķ� ���", OT_RUBY);
		pItem->SetDesc("�Ķ� ��");

		m_vecRuby.push_back(pItem);
	}
	file.Close();


	if (file.Open("WeaponStore.str", "rb"))
	{
		int iSize = 0;

		file.Read(&iSize, 4);

		for (int i = 0; i < iSize; ++i)
		{
			CItem*   pWeapon = new CItem;
			pWeapon->Load(&file);
			m_vecWeapon.push_back(pWeapon);
		}
	}
	else
	{

		CItem*	pItem = (CItem*)CreateObject(OT_WEAPON);

		pItem->SetItemInfo(0, 0, 5, 10, 0, 0, 5, 0, 0, 0, 0, 1000, 500);
		pItem->SetName("���", OT_WEAPON);
		pItem->SetDesc("������ ���� ��");
		m_vecWeapon.push_back(pItem);


		pItem = (CItem*)CreateObject(OT_WEAPON);

		pItem->SetItemInfo(0, 0, 25, 30, 0, 0, 10, 0.5, 0, 0, 0, 5000, 2500);
		pItem->SetName("ö��", OT_WEAPON);
		pItem->SetDesc("ö�� ���� ��");

		m_vecWeapon.push_back(pItem);


		pItem = (CItem*)CreateObject(OT_WEAPON);

		pItem->SetItemInfo(200, 0, 25, 30, 0, 0, 35, 0, 0, 0, 0, 25000, 12500);
		pItem->SetName("���� ����", OT_WEAPON);
		pItem->SetDesc("���� ����");

		m_vecWeapon.push_back(pItem);

	}
	file.Close();


	if (file.Open("ArmorStore.str", "rb"))
	{
		int iSize = 0;
		DeleteArmorStore();


		file.Read(&iSize, 4);

		for (int i = 0; i < iSize; ++i)
		{
			CItem*   pArmor = new CItem;
			pArmor->Load(&file);
			m_vecArmor.push_back(pArmor);
		}

	}
	else
	{
		// �� ����

		CItem*	pItem = (CItem*)CreateObject(OT_ARMOR);

		pItem->SetItemInfo(0, 0, 0, 0, 5, 10, 0, 0.2, 0, 0, 0, 1000, 500);
		pItem->SetName("���װ���", OT_ARMOR);
		pItem->SetDesc("���� ������ ����");

		m_vecArmor.push_back(pItem);

		pItem = (CItem*)CreateObject(OT_ARMOR);

		pItem->SetItemInfo(50, 0, 0, 0, 30, 60, 0, 0, 0, 0, 0, 5000, 2500);
		pItem->SetName("������ ����", OT_ARMOR);
		pItem->SetDesc("��������, ���̴�");

		m_vecArmor.push_back(pItem);


		pItem = (CItem*)CreateObject(OT_ARMOR);

		pItem->SetItemInfo(500, 50, 5, 5, 20, 30, 0, 0, 0, 0, 0, 50000, 25000);
		pItem->SetName("������ ���� ����", OT_ARMOR);
		pItem->SetDesc("���� ����ٴϴ� �� �ϴ�");

		m_vecArmor.push_back(pItem);

	}

	// ������ �ް� ������ �ϵ��ڵ�
	return true;
}

bool CObjManager::ItemInit()
{
	// ���⿡ ���� ó���� �Ѵ�.

	CItem*	pItem = (CItem*)CreateObject(OT_WEAPON);

	pItem->SetItemInfo(0, 0, 5, 10, 0, 0, 5, 0, 0, 0, 0, 1000, 500);
	pItem->SetName("���", OT_WEAPON);
	pItem->SetDesc("������ ���� ��");
	m_vecWeapon.push_back(pItem);


	pItem = (CItem*)CreateObject(OT_WEAPON);

	pItem->SetItemInfo(0, 0, 25, 30, 0, 0, 10, 0.5, 0, 0, 0, 5000, 2500);
	pItem->SetName("ö��", OT_WEAPON);
	pItem->SetDesc("ö�� ���� ��");

	m_vecWeapon.push_back(pItem);


	pItem = (CItem*)CreateObject(OT_WEAPON);

	pItem->SetItemInfo(200, 0, 25, 30, 0, 0, 35, 0, 0, 0, 0, 25000, 12500);
	pItem->SetName("���� ����", OT_WEAPON);
	pItem->SetDesc("���� ����");

	m_vecWeapon.push_back(pItem);


	// �� ����

	pItem = (CItem*)CreateObject(OT_ARMOR);

	pItem->SetItemInfo(0, 0, 0, 0, 5, 10, 0, 0.2, 0, 0, 0, 1000, 500);
	pItem->SetName("���װ���", OT_ARMOR);
	pItem->SetDesc("���� ������ ����");

	m_vecArmor.push_back(pItem);

	pItem = (CItem*)CreateObject(OT_ARMOR);

	pItem->SetItemInfo(50, 0, 0, 0, 30, 60, 0, 0, 0, 0, 0, 5000, 2500);
	pItem->SetName("������ ����", OT_ARMOR);
	pItem->SetDesc("��������, ���̴�");

	m_vecArmor.push_back(pItem);


	pItem = (CItem*)CreateObject(OT_ARMOR);

	pItem->SetItemInfo(500, 50, 5, 5, 20, 30, 0, 0, 0, 0, 0, 50000, 25000);
	pItem->SetName("������ ���� ����", OT_ARMOR);
	pItem->SetDesc("���� ����ٴϴ� �� �ϴ�");

	m_vecArmor.push_back(pItem);

	// ��ű� ����


	pItem = (CItem*)CreateObject(OT_RUBY);

	pItem->SetItemInfo(0, 0, 0, 0, 5, 10, 0, 0.2, 0, 0, 0, 1000, 500);
	pItem->SetName("���� ���", OT_RUBY);
	pItem->SetDesc("���� ��");

	m_vecRuby.push_back(pItem);


	pItem = (CItem*)CreateObject(OT_RUBY);

	pItem->SetItemInfo(100, 100, 5, 5, 5, 5, 3, 0, 0, 0, 0, 15000, 10000);
	pItem->SetName("�ʷ� ���", OT_RUBY);
	pItem->SetDesc("�ʷ� ��");

	m_vecRuby.push_back(pItem);

	pItem = (CItem*)CreateObject(OT_RUBY);

	pItem->SetItemInfo(150, 0, 10, 20, 10, 15, 3, 1, 0, 0, 0, 70000, 35500);
	pItem->SetName("�Ķ� ���", OT_RUBY);
	pItem->SetDesc("�Ķ� ��");

	m_vecRuby.push_back(pItem);
	return true;
}

bool CObjManager::MonsterInit()
{

	CMonster*	pMonster = (CMonster*)CreateObject(OT_MONSTER);

	// �� ������ ������ �ο��ϰ� ����.
	pMonster->SetName("���", OT_MONSTER);
	pMonster->SetInfo(100, 10, 10, 15, 5, 7, 500, 1);
	pMonster->SetDropGold(500, 1000);
	pMonster->SetStage(0);
	pMonster->OT_type = OT_MONSTER;
	m_tMonster.push_back(pMonster);

	pMonster = (CMonster*)CreateObject(OT_MONSTER);

	pMonster->SetName("��ũ", OT_MONSTER);
	pMonster->SetInfo(2000, 10, 50, 70, 50, 70, 3000, 5);
	pMonster->SetDropGold(5000, 6000);
	pMonster->SetStage(1);

	m_tMonster.push_back(pMonster);

	pMonster = (CMonster*)CreateObject(OT_MONSTER);

	pMonster->SetName("�巡��", OT_MONSTER);
	pMonster->SetInfo(5500, 300, 150, 200, 100, 150, 10000, 10);
	pMonster->SetDropGold(25000, 30000);
	pMonster->SetStage(2);

	m_tMonster.push_back(pMonster);

	return true;
}

bool CObjManager::ObjectInit()
{
	// ���� & ������ �ϵ��ڵ�
	ItemInit();

	MonsterInit();
	
	return true;
}

void CObjManager::LoadRubyStore()
{
	DeleteRubyStore(); // �о����� ������ �ִ� ���� ��������.
	int iSize = 0;

	CFilestream file("RubyStore.str", "rb");

	file.Read(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		CItem*   pRuby = new CItem;
		pRuby->Load(&file);
		m_vecRuby.push_back(pRuby);
	}

	cout << "��ű��� �о�Խ��ϴ�. " << endl;
	system("pause");
}

void CObjManager::LoadWeaponStore()
{
	DeleteWeaponStore();
	int iSize = 0;

	CFilestream file("WeaponStore.str", "rb");

	file.Read(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		CItem*   pWeapon = new CItem;
		pWeapon->Load(&file);
		m_vecWeapon.push_back(pWeapon);
	}

	cout << "���⸦ �о�Խ��ϴ�. " << endl;
	system("pause");
}

void CObjManager::LoadArmorStore()
{
	int iSize = 0;
	DeleteArmorStore();

	CFilestream file("ArmorStore.str", "rb");

	file.Read(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		CItem*   pArmor = new CItem;
		pArmor->Load(&file);
		m_vecArmor.push_back(pArmor);
	}

	cout << "���� �о�Խ��ϴ�. " << endl;
	system("pause");
}



void CObjManager::SaveRubyStore()
{
	int iSize = m_vecRuby.size();

	CFilestream file("RubyStore.str", "wb");

	file.Write(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		m_vecRuby[i]->Save(&file);
	}

	cout << "��ű��� �����߽��ϴ�. " << endl;
	system("pause");
}

void CObjManager::SaveWeaponStore()
{
	int iSize = m_vecWeapon.size();

	CFilestream file("WeaponStore.str", "wb");

	file.Write(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		m_vecWeapon[i]->Save(&file);
	}

	cout << "���⸦ �����߽��ϴ�. " << endl;
	system("pause");
}

void CObjManager::SaveArmorStore()
{
	// ���� ������ �����Ѵ�.

	int iSize = m_vecArmor.size();

	CFilestream file("ArmorStore.str", "wb");

	file.Write(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		m_vecArmor[i]->Save(&file);
	}

	cout << "���� �����߽��ϴ�. " << endl;
	system("pause");
}

void CObjManager::DeleteMonsterAll()
{
	Safe_Delete_VecList(m_tMonster);
}

void CObjManager::DeleteRubyStore()
{
	Safe_Delete_VecList(m_vecRuby);

}

void CObjManager::DeleteArmorStore()
{
	Safe_Delete_VecList(m_vecArmor);

}

void CObjManager::DeleteWeaponStore()
{
	Safe_Delete_VecList(m_vecWeapon);

}

void CObjManager::SaveMonster()
{
	// ������ ���� �����Ѵ�.

	int iSize = m_tMonster.size();

	CFilestream file("Monster.mst", "wb");

	file.Write(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		m_tMonster[i]->Save(&file);
	}
	// ������ �� ���� �׳� ���� ���缭 �����ϰ� ������ �ȴ�. 
	// ������ �о���� ��쿡�� �Ҵ��� �ؼ� ������ �ؾ� �Ѵ�.

	cout << "���͸� �����߽��ϴ�. " << endl;
	system("pause");
}

void CObjManager::LoadMonster()
{
	// ���� �����ʹ� ���ش�.
	DeleteMonsterAll();
	// ������ ���� �о�´�.

	int iSize = 0;

	CFilestream file("Monster.mst", "rb");

	file.Read(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		CMonster*   pMonster = (CMonster*)CreateObject(OT_MONSTER);
		pMonster->Load(&file);
		m_tMonster.push_back(pMonster);
	}

	cout << "���͸� �о�Խ��ϴ�. " << endl;
	system("pause");
}

void CObjManager::CreateMonster()
{
	CMonster*	pMonster = (CMonster*)CreateObject(OT_MONSTER);

	// �� ������ ������ �ο��ϰ� ����.

	Ignore();
	cout << "���� �̸� : ";
	char* str = new char[NAME_SIZE_MAX];

	cin.getline(str, NAME_SIZE_MAX - 1, '\n');
	pMonster->SetName(str, OT_MONSTER);

	SAFE_DELETE_ARRAY(str);

	int iAttackMax, iAttackMin, iArmorMax, iArmorMin, iHP, iMP, iLevel, iStage, iGoldMin, iGoldMax;
	long long iExp;

	cout << "���� : "; cin >> iLevel;
	cout << "ü�� : "; cin >> iHP;
	cout << "���� : "; cin >> iMP;
	cout << "�ּ� ���ݷ� : "; cin >> iAttackMin;
	cout << "�ִ� ���ݷ� : "; cin >> iAttackMax;
	cout << "�ּ� ���� : "; cin >> iArmorMin;
	cout << "�ִ� ���� : "; cin >> iArmorMax;
	cout << "ȹ�� ����ġ : "; cin >> iExp;
	cout << "ȹ�� �ּ� ��� : "; cin >> iGoldMin;
	cout << "ȹ�� �ִ� ��� : "; cin >> iGoldMax;
	
	// �������� ������ ����ؾ� �Ѵ�.
	
	int iCount = GET_SINGLE(CStageManager)->m_vecStage.size();

	cout << " ========== �������� ���� =========== " << endl << endl;

	for (int i = 0; i < iCount; ++i)
	{
		cout << i + 1 << ".";
		GET_SINGLE(CStageManager)->m_vecStage[i]->DeleteVecter();
		GET_SINGLE(CStageManager)->m_vecStage[i]->iMonsterCount = 0;
		GET_SINGLE(CStageManager)->m_vecStage[i]->Init(i);
		GET_SINGLE(CStageManager)->m_vecStage[i]->Render(); 
		cout << endl;
	}
	// ������ �� -1 ���� ���������� �� �� �ִ�.
	int iSize = GET_SINGLE(CStageManager)->m_vecStage.size() - 1; // �ִ�.

	while (true)
	{
		cout << endl;
		cout << "���� �������� : ";
		iStage = Input<int>();

		if (iStage < 0 || iStage > iSize)
			continue;

		// ����� �Է�
		break;
	}
	pMonster->SetInfo(iHP, iMP, iAttackMin, iAttackMax, iArmorMin, iArmorMax, iExp, iLevel);
	pMonster->SetDropGold(iGoldMin, iGoldMax);
	pMonster->SetStage(iStage);
	m_tMonster.push_back(pMonster);


	cout << "���Ͱ� �߰� �Ǿ����ϴ�. " << endl;
	system("pause");
}

void CObjManager::CreateItem(OBJECT_TYPE eType)
{
	Ignore();
	CItem* pItem = (CItem*)CreateObject(eType);
	
	char* str = new char[NAME_SIZE_MAX]; 
	cout << "�������� �̸� : ";
	cin.getline(str, NAME_SIZE_MAX - 1, '\n');
	pItem->SetName(str, eType);

	SAFE_DELETE_ARRAY(str);

	char* strDesc = new char[256]; 

	cout << "������ ���� : ";
	cin.getline(strDesc, 255, '\n');

	pItem->SetDesc(strDesc);
	SAFE_DELETE_ARRAY(strDesc);

	// ������ ���� ������ ��
	int		iHP;
	int		iMP;
	int		iAttackMin;
	int		iAttackMax;
	int		iArmorMin;
	int		iArmorMax;
	int		iCriticalPercent;
	double	iAttackPercent;
	double	iArmorPercent;
	double	iAvoidPercent;
	double	iLuckyPercent;
	int		iPrice;
	int		iSell;

	cout << "�߰� ü�� : "; cin >> iHP;
	cout << "�߰� ���� : "; cin >> iMP;
	cout << "�ּ� ���ݷ� : "; cin >> iAttackMin;
	cout << "�ִ� ���ݷ� : "; cin >> iAttackMax;
	cout << "�ּ� ���� : "; cin >> iArmorMin;
	cout << "�ִ� ���� : "; cin >> iArmorMax;
	cout << "ũ��Ƽ�� Ȯ�� : "; cin >> iCriticalPercent;
	cout << "�߰� ���ݷ�(%, double) : "; cin >> iAttackPercent;
	cout << "�߰� ����(%, double) : "; cin >> iArmorPercent;
	cout << "�߰� ȸ�Ƿ�(%, double) : "; cin >> iAvoidPercent;
	cout << "�߰� ����(%, double) : "; cin >> iLuckyPercent;
	cout << "���� �ǸŰ� : "; cin >> iPrice;
	cout << "���� ���԰� : "; cin >> iSell;

	pItem->SetItemInfo(iHP, iMP, iAttackMin, iAttackMax, iArmorMin, iArmorMax, iCriticalPercent, iAttackPercent, iArmorPercent, iAvoidPercent, iLuckyPercent, iPrice, iSell);

	switch (eType)
	{
	case OT_ARMOR:
		cout << "���� �߰��߽��ϴ�. " << endl;
		m_vecArmor.push_back(pItem);
		break;
	case OT_RUBY:
		cout << "��ű��� �߰��߽��ϴ�. " << endl;
		m_vecRuby.push_back(pItem);
		break;
	case OT_WEAPON:
		cout << "���⸦ �߰��߽��ϴ�. " << endl;
		m_vecWeapon.push_back(pItem);
		break;
	}
	system("pause");
}

void CObjManager::PlayerSaveVecFree()
{
	SAFE_DELETE(m_tPlayer[0]);
	SAFE_DELETE(m_tPlayer[1]);
	SAFE_DELETE(m_tPlayer[2]);
}

CObj * CObjManager::CreateObject(OBJECT_TYPE eType)
{
	CObj*	pObj = NULL;

	switch (eType)
	{
	case OT_PLAYER:
		pObj = new CPlayer;
		break;
	case OT_MONSTER:
		pObj = new CMonster;
		break;
	case OT_ARMOR:
	case OT_RUBY:
	case OT_WEAPON:
		pObj = new CItem;
		break;
	}
	
	if (!pObj->Init())
	{
		SAFE_DELETE(pObj);
		return NULL;
	}

	return pObj;
}

void CObjManager::Run()
{
}

bool CObjManager::Init()
{
	// ���Ϳ� �÷��̾ �ʱ�ȭ�Ѵ�.
	CPlayer*	pPlayer = (CPlayer*)CreateObject(OT_PLAYER);
	
	// ������ ���� �ɷ�ġ�� �ο��ϰ� ����.
	switch (pPlayer->GetJob())
	{
	case JOB_KNIGHT:
		pPlayer->SetInfo(200, 50, 10, 15, 15, 25, 0, 1);
		pPlayer->SetPlayer(0, 0.5, 0, 0, 0, 500);

		break;
	case JOB_ARCHER:
		pPlayer->SetInfo(150, 100, 15, 20, 10, 15, 0, 1);
		pPlayer->SetPlayer(0, 0, 2, 0, 0, 500);

		break;
	case JOB_WIZARD:
		pPlayer->SetInfo(120, 200, 20, 25, 5, 7, 0, 1);
		pPlayer->SetPlayer(0.5, 0, 0, 0, 0, 500);

		break;
	case JOB_THIEF:
		pPlayer->SetInfo(90, 150, 27, 30, 3, 5, 0, 1);
		pPlayer->SetPlayer(0, 0, 0, 2, 0, 500);

		break;

	case JOB_SWORD:
		pPlayer->SetInfo(120, 70, 20, 25, 5, 10, 0, 1);
		pPlayer->SetPlayer(0, 0, 0, 0, 2, 500);

		break;

	case JOB_GIANT: // Ư�������� ���� %�� ����. �翬�� �ʹݿ��� ����.
		pPlayer->SetInfo(210, 50, 10, 14, 15, 17, 0, 1);
		pPlayer->SetPlayer(0, 0, 0, 0, 0, 500);
		break;

	case JOB_ELF:
		pPlayer->SetInfo(100, 130, 15, 20, 7, 8, 0, 1);
		pPlayer->SetPlayer(0, 0, 0, 0, 0, 500);
		break;

	case JOB_DEMON:
		pPlayer->SetInfo(80, 100, 20, 25, 6, 7, 0, 1);
		pPlayer->SetPlayer(0, 0, 0, 0, 0, 500);
		break;

	case JOB_DRAGON:
		pPlayer->SetInfo(90, 120, 15, 16, 18, 20, 0, 1);
		pPlayer->SetPlayer(0, 0, 0, 0, 0, 500);
		break;

	case JOB_ORC:
		pPlayer->SetInfo(140, 70, 10, 17, 17, 23, 0, 1);
		pPlayer->SetPlayer(0, 0, 0, 0, 0, 500);
		break;
	}

	pPlayer->OT_type = OT_PLAYER;
	// �⺻ ������ �ϰ� ������ info�� �����ؾ� ��.


	m_tCurPlayer = pPlayer;


	InitLoad(); // ������ �ް� ������ �ϵ��ڵ�.

	return true;

}

bool CObjManager::InitSavePlayer()
{
	const char* arr[3] = { "Player1.ply", "Player2.ply" , "Player3.ply" };

	CFilestream file;
	for (int i = 0; i < 3; ++i)
	{
		file.Open(arr[i], "rb");

		CPlayer* pPlayer = new CPlayer;

		pPlayer->Load(&file);

		m_tPlayer[i] = pPlayer;
		
		file.Close();
	}
	return true;
}

void CObjManager::ArmorStoreList()
{
	int iMenu;
	while(true)
	{
			system("cls");
			StoreRender(OT_ARMOR, false);

			SHOW2(m_vecArmor.size() + 1 << ". �ڷΰ���");
			cout << endl << "������ : " << m_tCurPlayer->GetGold() << endl;

			cout << "� ������ ��ðڽ��ϱ� ? : ";

			iMenu = Input<int>();

			if (iMenu == m_vecArmor.size() + 1)
				return;

			else if (iMenu <= 0 || iMenu > m_vecArmor.size() + 1)
				continue;

			// ���� �ִ��� üũ�ϰ� ������ m_vec�� �־����.

			if (m_tCurPlayer->GetGold() >= m_vecArmor[iMenu - 1]->GetiPrice())
			{
				// ������ �����Ѵ�.
				m_tCurPlayer->AddGold(-m_vecArmor[iMenu - 1]->GetiPrice()); // ���� ���� ����
				m_tCurPlayer->AddItem(m_vecArmor[iMenu - 1]); // ���� ������ �����Ͽ� ����.

				cout << "�������� �����߽��ϴ�. " << endl;
			}
			else
			{
				cout << "�ܾ��� �����մϴ�." << endl;
			}

			system("pause");
		}
}

void CObjManager::WeaponStoreList()
{
	int iMenu;
	while (true)
	{
		StoreRender(OT_WEAPON, false);

		SHOW2(m_vecWeapon.size() + 1 << ". �ڷΰ���" );
		cout << endl << "������ : " << m_tCurPlayer->GetGold() << endl;

		cout << "� ������ ��ðڽ��ϱ� ? : ";

		iMenu = Input<int>();

		if (iMenu == m_vecWeapon.size() + 1)
			return;

		else if (iMenu <= 0 || iMenu > m_vecWeapon.size() + 1)
			continue;

		// ���� �ִ��� üũ�ϰ� ������ m_vec�� �־����.

		if (m_tCurPlayer->GetGold() >= m_vecWeapon[iMenu - 1]->GetiPrice())
		{
			// ������ �����Ѵ�.
			m_tCurPlayer->AddGold(-m_vecWeapon[iMenu - 1]->GetiPrice()); // ���� ���� ����
			m_tCurPlayer->AddItem(m_vecWeapon[iMenu - 1]); // ���� ������ �����Ͽ� ����.

			cout << "�������� �����߽��ϴ�. " << endl;
		}
		else
		{
			cout << "�ܾ��� �����մϴ�." << endl;
		}

		system("pause");
	}

}

void CObjManager::RubyStoreList()
{

	int iMenu;

	while (true)
	{
		system("cls");

	
		StoreRender(OT_RUBY, false);

		SHOW2(m_vecRuby.size() + 1 << ". �ڷΰ���");
		cout << endl << "������ : " << m_tCurPlayer->GetGold() << endl;
		cout << "� ������ ��ðڽ��ϱ� ? : ";

		iMenu = Input<int>();

		if (iMenu == m_vecRuby.size() + 1)
			return;

		else if (iMenu <= 0 || iMenu > m_vecRuby.size() + 1)
			continue;

			// ���� �ִ��� üũ�ϰ� ������ m_vec�� �־����.

			if (m_tCurPlayer->GetGold() >= m_vecRuby[iMenu - 1]->GetiPrice())
			{
				// ������ �����Ѵ�.
				m_tCurPlayer->AddGold(-m_vecRuby[iMenu - 1]->GetiPrice()); // ���� ���� ����
				m_tCurPlayer->AddItem(m_vecRuby[iMenu - 1]); // ���� ������ �����Ͽ� ����.

				cout << "�������� �����߽��ϴ�. " << endl;
			}
			else
			{
				cout << "�ܾ��� �����մϴ�." << endl;
			}

		system("pause");
	}
	

	
}

void CObjManager::StoreRender(OBJECT_TYPE eType, bool ok)
{
	vector<CItem*>::iterator iter, iterEnd;


	system("cls");

	cout << endl;

	switch (eType)
	{
	case OT_WEAPON:
		SHOW("���� ����");
		iter = m_vecWeapon.begin();
		iterEnd = m_vecWeapon.end();
		break;

	case OT_ARMOR:
		SHOW("�� ����");
		iter = m_vecArmor.begin();
		iterEnd = m_vecArmor.end();
		break;

	case OT_RUBY:
		SHOW("��ű� ����");
		iter = m_vecRuby.begin();
		iterEnd = m_vecRuby.end();
		break;
	}


	for (int i = 1; iter != iterEnd; ++i, ++iter)
	{
		SHOW2(i);
		(*iter)->Render();
		cout << endl;

	}

	if(ok)
	system("pause");
}

