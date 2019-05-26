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
	// 우리는 공식을 알고 있기 때문에 그냥 for문 돌리면 된다.

	system("cls");
	cout << endl;
	SHOW("레벨업 테이블 생성");
	cout << "최대 레벨을 입력하세요(1 ~ 100): ";
	int iLevel = Input<int>();

	cout << "1 -> 2 레벨에서 필요한 경험치를 입력하세요 : ";
	int One = Input<int>();

	cout << "공식 : (이전 레벨 경험치 * 2) * A " << endl;

	cout << "참고 : A 추천 값 0.7 " << endl;
	cout << "A(double) 를 입력하세요 : ";
	double A = Input<double>();


	LevelUpTable.push_back(One);

	for (int i = 1; i < iLevel-1; ++i)
	{
		LevelUpTable.push_back(LevelUpTable[i - 1] * 2 * A);
	}

	cout << "레벨업 테이블을 생성했습니다." << endl;
	system("pause");
}

void CObjManager::SaveLevelUpTable()
{
	CFilestream file("LevelUpTabel.Lv", "wb");

	// 레벨의 개수를 쓴다.

	int iSize = LevelUpTable.size();

	file.Write(&iSize, 4);

	for (int i = 0; i < iSize; ++i) // 레벨의 개수만큼 쓴다.
	{
		file.Write(&LevelUpTable[i], sizeof(long long));
	}

	cout << "저장을 마쳤습니다." << endl;
	system("pause");
}

void CObjManager::LoadLevelUpTable()
{

	// 불러오기 전에 일단 데이터를 버림

	LevelUpTable.clear();
	CFilestream file("LevelUpTabel.Lv", "rb");

	// 레벨의 개수를 읽는다.

	int iSize = 0;

	file.Read(&iSize, 4);

	for (int i = 0; i < iSize; ++i) // 레벨의 개수만큼 읽어서 저장한다.
	{
		long long a;
		file.Read(&a, sizeof(long long));

		LevelUpTable.push_back(a);
	}

	cout << "불러오기를 마쳤습니다." << endl;
	system("pause");
}

void CObjManager::LevelUpTableList()
{
	system("cls");
	cout << endl;
	SHOW("레벨업 테이블 출력");

	int Max = LevelUpTable.size();

	for (int i = 0; i < Max; ++i)
	{
		SHOW2(i + 1 << " Lv");
		cout << LevelUpTable[i] << endl << endl;
	}

	cout << "출력이 끝났습니다." << endl;
	system("pause");
}

void CObjManager::DeleteLevelUpTable()
{
	LevelUpTable.clear(); // 이곳에 할당을 한 것은 아니므로 그냥 버리면 된다.

	cout << "모든 경험치 데이터를 삭제했습니다." << endl;
	system("pause");
}

bool CObjManager::InitLoad()
{
	int iSize = 0;

	CFilestream file;

	if (file.Open("Monster.mst", "rb")) // true면 파일이 있음.
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
		// 장신구 셋팅


		CItem*	pItem = (CItem*)CreateObject(OT_RUBY);

		pItem->SetItemInfo(0, 0, 0, 0, 5, 10, 0, 0.2, 0, 0, 0, 1000, 500);
		pItem->SetName("빨간 루비", OT_RUBY);
		pItem->SetDesc("빨간 색");

		m_vecRuby.push_back(pItem);


		pItem = (CItem*)CreateObject(OT_RUBY);

		pItem->SetItemInfo(100, 100, 5, 5, 5, 5, 3, 0, 0, 0, 0, 15000, 10000);
		pItem->SetName("초록 루비", OT_RUBY);
		pItem->SetDesc("초록 색");

		m_vecRuby.push_back(pItem);

		pItem = (CItem*)CreateObject(OT_RUBY);

		pItem->SetItemInfo(150, 0, 10, 20, 10, 15, 3, 1, 0, 0, 0, 70000, 35500);
		pItem->SetName("파란 루비", OT_RUBY);
		pItem->SetDesc("파란 색");

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
		pItem->SetName("목검", OT_WEAPON);
		pItem->SetDesc("나무로 만든 검");
		m_vecWeapon.push_back(pItem);


		pItem = (CItem*)CreateObject(OT_WEAPON);

		pItem->SetItemInfo(0, 0, 25, 30, 0, 0, 10, 0.5, 0, 0, 0, 5000, 2500);
		pItem->SetName("철검", OT_WEAPON);
		pItem->SetDesc("철로 만든 검");

		m_vecWeapon.push_back(pItem);


		pItem = (CItem*)CreateObject(OT_WEAPON);

		pItem->SetItemInfo(200, 0, 25, 30, 0, 0, 35, 0, 0, 0, 0, 25000, 12500);
		pItem->SetName("유령 무희", OT_WEAPON);
		pItem->SetDesc("톡톡 팡팡");

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
		// 방어구 셋팅

		CItem*	pItem = (CItem*)CreateObject(OT_ARMOR);

		pItem->SetItemInfo(0, 0, 0, 0, 5, 10, 0, 0.2, 0, 0, 0, 1000, 500);
		pItem->SetName("가죽갑옷", OT_ARMOR);
		pItem->SetDesc("동물 냄새가 난다");

		m_vecArmor.push_back(pItem);

		pItem = (CItem*)CreateObject(OT_ARMOR);

		pItem->SetItemInfo(50, 0, 0, 0, 30, 60, 0, 0, 0, 0, 0, 5000, 2500);
		pItem->SetName("빛나는 갑옷", OT_ARMOR);
		pItem->SetDesc("멋있지만, 무겁다");

		m_vecArmor.push_back(pItem);


		pItem = (CItem*)CreateObject(OT_ARMOR);

		pItem->SetItemInfo(500, 50, 5, 5, 20, 30, 0, 0, 0, 0, 0, 50000, 25000);
		pItem->SetName("빛나는 날개 갑옷", OT_ARMOR);
		pItem->SetDesc("몸이 날라다니는 듯 하다");

		m_vecArmor.push_back(pItem);

	}

	// 있으면 받고 없으면 하드코딩
	return true;
}

bool CObjManager::ItemInit()
{
	// 무기에 관한 처리를 한다.

	CItem*	pItem = (CItem*)CreateObject(OT_WEAPON);

	pItem->SetItemInfo(0, 0, 5, 10, 0, 0, 5, 0, 0, 0, 0, 1000, 500);
	pItem->SetName("목검", OT_WEAPON);
	pItem->SetDesc("나무로 만든 검");
	m_vecWeapon.push_back(pItem);


	pItem = (CItem*)CreateObject(OT_WEAPON);

	pItem->SetItemInfo(0, 0, 25, 30, 0, 0, 10, 0.5, 0, 0, 0, 5000, 2500);
	pItem->SetName("철검", OT_WEAPON);
	pItem->SetDesc("철로 만든 검");

	m_vecWeapon.push_back(pItem);


	pItem = (CItem*)CreateObject(OT_WEAPON);

	pItem->SetItemInfo(200, 0, 25, 30, 0, 0, 35, 0, 0, 0, 0, 25000, 12500);
	pItem->SetName("유령 무희", OT_WEAPON);
	pItem->SetDesc("톡톡 팡팡");

	m_vecWeapon.push_back(pItem);


	// 방어구 셋팅

	pItem = (CItem*)CreateObject(OT_ARMOR);

	pItem->SetItemInfo(0, 0, 0, 0, 5, 10, 0, 0.2, 0, 0, 0, 1000, 500);
	pItem->SetName("가죽갑옷", OT_ARMOR);
	pItem->SetDesc("동물 냄새가 난다");

	m_vecArmor.push_back(pItem);

	pItem = (CItem*)CreateObject(OT_ARMOR);

	pItem->SetItemInfo(50, 0, 0, 0, 30, 60, 0, 0, 0, 0, 0, 5000, 2500);
	pItem->SetName("빛나는 갑옷", OT_ARMOR);
	pItem->SetDesc("멋있지만, 무겁다");

	m_vecArmor.push_back(pItem);


	pItem = (CItem*)CreateObject(OT_ARMOR);

	pItem->SetItemInfo(500, 50, 5, 5, 20, 30, 0, 0, 0, 0, 0, 50000, 25000);
	pItem->SetName("빛나는 날개 갑옷", OT_ARMOR);
	pItem->SetDesc("몸이 날라다니는 듯 하다");

	m_vecArmor.push_back(pItem);

	// 장신구 셋팅


	pItem = (CItem*)CreateObject(OT_RUBY);

	pItem->SetItemInfo(0, 0, 0, 0, 5, 10, 0, 0.2, 0, 0, 0, 1000, 500);
	pItem->SetName("빨간 루비", OT_RUBY);
	pItem->SetDesc("빨간 색");

	m_vecRuby.push_back(pItem);


	pItem = (CItem*)CreateObject(OT_RUBY);

	pItem->SetItemInfo(100, 100, 5, 5, 5, 5, 3, 0, 0, 0, 0, 15000, 10000);
	pItem->SetName("초록 루비", OT_RUBY);
	pItem->SetDesc("초록 색");

	m_vecRuby.push_back(pItem);

	pItem = (CItem*)CreateObject(OT_RUBY);

	pItem->SetItemInfo(150, 0, 10, 20, 10, 15, 3, 1, 0, 0, 0, 70000, 35500);
	pItem->SetName("파란 루비", OT_RUBY);
	pItem->SetDesc("파란 색");

	m_vecRuby.push_back(pItem);
	return true;
}

bool CObjManager::MonsterInit()
{

	CMonster*	pMonster = (CMonster*)CreateObject(OT_MONSTER);

	// 각 몬스터의 정보를 부여하고 저장.
	pMonster->SetName("고블린", OT_MONSTER);
	pMonster->SetInfo(100, 10, 10, 15, 5, 7, 500, 1);
	pMonster->SetDropGold(500, 1000);
	pMonster->SetStage(0);
	pMonster->OT_type = OT_MONSTER;
	m_tMonster.push_back(pMonster);

	pMonster = (CMonster*)CreateObject(OT_MONSTER);

	pMonster->SetName("오크", OT_MONSTER);
	pMonster->SetInfo(2000, 10, 50, 70, 50, 70, 3000, 5);
	pMonster->SetDropGold(5000, 6000);
	pMonster->SetStage(1);

	m_tMonster.push_back(pMonster);

	pMonster = (CMonster*)CreateObject(OT_MONSTER);

	pMonster->SetName("드래곤", OT_MONSTER);
	pMonster->SetInfo(5500, 300, 150, 200, 100, 150, 10000, 10);
	pMonster->SetDropGold(25000, 30000);
	pMonster->SetStage(2);

	m_tMonster.push_back(pMonster);

	return true;
}

bool CObjManager::ObjectInit()
{
	// 몬스터 & 아이템 하드코딩
	ItemInit();

	MonsterInit();
	
	return true;
}

void CObjManager::LoadRubyStore()
{
	DeleteRubyStore(); // 읽어오기는 기존에 있던 것을 지워버림.
	int iSize = 0;

	CFilestream file("RubyStore.str", "rb");

	file.Read(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		CItem*   pRuby = new CItem;
		pRuby->Load(&file);
		m_vecRuby.push_back(pRuby);
	}

	cout << "장신구를 읽어왔습니다. " << endl;
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

	cout << "무기를 읽어왔습니다. " << endl;
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

	cout << "방어구를 읽어왔습니다. " << endl;
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

	cout << "장신구를 저장했습니다. " << endl;
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

	cout << "무기를 저장했습니다. " << endl;
	system("pause");
}

void CObjManager::SaveArmorStore()
{
	// 방어구의 개수를 저장한다.

	int iSize = m_vecArmor.size();

	CFilestream file("ArmorStore.str", "wb");

	file.Write(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		m_vecArmor[i]->Save(&file);
	}

	cout << "방어구를 저장했습니다. " << endl;
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
	// 몬스터의 수를 저장한다.

	int iSize = m_tMonster.size();

	CFilestream file("Monster.mst", "wb");

	file.Write(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		m_tMonster[i]->Save(&file);
	}
	// 저장을 할 때는 그냥 수에 맞춰서 저장하고 끝내면 된다. 
	// 하지만 읽어오는 경우에는 할당을 해서 데입을 해야 한다.

	cout << "몬스터를 저장했습니다. " << endl;
	system("pause");
}

void CObjManager::LoadMonster()
{
	// 기존 데이터는 없앤다.
	DeleteMonsterAll();
	// 몬스터의 수를 읽어온다.

	int iSize = 0;

	CFilestream file("Monster.mst", "rb");

	file.Read(&iSize, 4);

	for (int i = 0; i < iSize; ++i)
	{
		CMonster*   pMonster = (CMonster*)CreateObject(OT_MONSTER);
		pMonster->Load(&file);
		m_tMonster.push_back(pMonster);
	}

	cout << "몬스터를 읽어왔습니다. " << endl;
	system("pause");
}

void CObjManager::CreateMonster()
{
	CMonster*	pMonster = (CMonster*)CreateObject(OT_MONSTER);

	// 각 몬스터의 정보를 부여하고 저장.

	Ignore();
	cout << "몬스터 이름 : ";
	char* str = new char[NAME_SIZE_MAX];

	cin.getline(str, NAME_SIZE_MAX - 1, '\n');
	pMonster->SetName(str, OT_MONSTER);

	SAFE_DELETE_ARRAY(str);

	int iAttackMax, iAttackMin, iArmorMax, iArmorMin, iHP, iMP, iLevel, iStage, iGoldMin, iGoldMax;
	long long iExp;

	cout << "레벨 : "; cin >> iLevel;
	cout << "체력 : "; cin >> iHP;
	cout << "마나 : "; cin >> iMP;
	cout << "최소 공격력 : "; cin >> iAttackMin;
	cout << "최대 공격력 : "; cin >> iAttackMax;
	cout << "최소 방어력 : "; cin >> iArmorMin;
	cout << "최대 방어력 : "; cin >> iArmorMax;
	cout << "획득 경험치 : "; cin >> iExp;
	cout << "획득 최소 골드 : "; cin >> iGoldMin;
	cout << "획득 최대 골드 : "; cin >> iGoldMax;
	
	// 스테이지 정보를 출력해야 한다.
	
	int iCount = GET_SINGLE(CStageManager)->m_vecStage.size();

	cout << " ========== 스테이지 정보 =========== " << endl << endl;

	for (int i = 0; i < iCount; ++i)
	{
		cout << i + 1 << ".";
		GET_SINGLE(CStageManager)->m_vecStage[i]->DeleteVecter();
		GET_SINGLE(CStageManager)->m_vecStage[i]->iMonsterCount = 0;
		GET_SINGLE(CStageManager)->m_vecStage[i]->Init(i);
		GET_SINGLE(CStageManager)->m_vecStage[i]->Render(); 
		cout << endl;
	}
	// 벡터의 수 -1 까지 스테이지를 고를 수 있다.
	int iSize = GET_SINGLE(CStageManager)->m_vecStage.size() - 1; // 최댓값.

	while (true)
	{
		cout << endl;
		cout << "출현 스테이지 : ";
		iStage = Input<int>();

		if (iStage < 0 || iStage > iSize)
			continue;

		// 제대로 입력
		break;
	}
	pMonster->SetInfo(iHP, iMP, iAttackMin, iAttackMax, iArmorMin, iArmorMax, iExp, iLevel);
	pMonster->SetDropGold(iGoldMin, iGoldMax);
	pMonster->SetStage(iStage);
	m_tMonster.push_back(pMonster);


	cout << "몬스터가 추가 되었습니다. " << endl;
	system("pause");
}

void CObjManager::CreateItem(OBJECT_TYPE eType)
{
	Ignore();
	CItem* pItem = (CItem*)CreateObject(eType);
	
	char* str = new char[NAME_SIZE_MAX]; 
	cout << "아이템의 이름 : ";
	cin.getline(str, NAME_SIZE_MAX - 1, '\n');
	pItem->SetName(str, eType);

	SAFE_DELETE_ARRAY(str);

	char* strDesc = new char[256]; 

	cout << "아이템 설명 : ";
	cin.getline(strDesc, 255, '\n');

	pItem->SetDesc(strDesc);
	SAFE_DELETE_ARRAY(strDesc);

	// 아이템 인포 가져온 것
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

	cout << "추가 체력 : "; cin >> iHP;
	cout << "추가 마나 : "; cin >> iMP;
	cout << "최소 공격력 : "; cin >> iAttackMin;
	cout << "최대 공격력 : "; cin >> iAttackMax;
	cout << "최소 방어력 : "; cin >> iArmorMin;
	cout << "최대 방어력 : "; cin >> iArmorMax;
	cout << "크리티컬 확률 : "; cin >> iCriticalPercent;
	cout << "추가 공격력(%, double) : "; cin >> iAttackPercent;
	cout << "추가 방어력(%, double) : "; cin >> iArmorPercent;
	cout << "추가 회피력(%, double) : "; cin >> iAvoidPercent;
	cout << "추가 행운력(%, double) : "; cin >> iLuckyPercent;
	cout << "상점 판매가 : "; cin >> iPrice;
	cout << "상점 매입가 : "; cin >> iSell;

	pItem->SetItemInfo(iHP, iMP, iAttackMin, iAttackMax, iArmorMin, iArmorMax, iCriticalPercent, iAttackPercent, iArmorPercent, iAvoidPercent, iLuckyPercent, iPrice, iSell);

	switch (eType)
	{
	case OT_ARMOR:
		cout << "방어구를 추가했습니다. " << endl;
		m_vecArmor.push_back(pItem);
		break;
	case OT_RUBY:
		cout << "장신구를 추가했습니다. " << endl;
		m_vecRuby.push_back(pItem);
		break;
	case OT_WEAPON:
		cout << "무기를 추가했습니다. " << endl;
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
	// 몬스터와 플레이어를 초기화한다.
	CPlayer*	pPlayer = (CPlayer*)CreateObject(OT_PLAYER);
	
	// 직업에 따른 능력치를 부여하고 저장.
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

	case JOB_GIANT: // 특별직업은 성장 %가 없다. 당연히 초반에도 없다.
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
	// 기본 셋팅을 하고 레벨업 info를 데입해야 함.


	m_tCurPlayer = pPlayer;


	InitLoad(); // 있으면 받고 없으면 하드코딩.

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

			SHOW2(m_vecArmor.size() + 1 << ". 뒤로가기");
			cout << endl << "소지금 : " << m_tCurPlayer->GetGold() << endl;

			cout << "어떤 물건을 사시겠습니까 ? : ";

			iMenu = Input<int>();

			if (iMenu == m_vecArmor.size() + 1)
				return;

			else if (iMenu <= 0 || iMenu > m_vecArmor.size() + 1)
				continue;

			// 돈이 있는지 체크하고 있으면 m_vec에 넣어버림.

			if (m_tCurPlayer->GetGold() >= m_vecArmor[iMenu - 1]->GetiPrice())
			{
				// 물건을 구매한다.
				m_tCurPlayer->AddGold(-m_vecArmor[iMenu - 1]->GetiPrice()); // 물건 값을 지불
				m_tCurPlayer->AddItem(m_vecArmor[iMenu - 1]); // 상점 물건을 복사하여 데입.

				cout << "아이템을 구매했습니다. " << endl;
			}
			else
			{
				cout << "잔액이 부족합니다." << endl;
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

		SHOW2(m_vecWeapon.size() + 1 << ". 뒤로가기" );
		cout << endl << "소지금 : " << m_tCurPlayer->GetGold() << endl;

		cout << "어떤 물건을 사시겠습니까 ? : ";

		iMenu = Input<int>();

		if (iMenu == m_vecWeapon.size() + 1)
			return;

		else if (iMenu <= 0 || iMenu > m_vecWeapon.size() + 1)
			continue;

		// 돈이 있는지 체크하고 있으면 m_vec에 넣어버림.

		if (m_tCurPlayer->GetGold() >= m_vecWeapon[iMenu - 1]->GetiPrice())
		{
			// 물건을 구매한다.
			m_tCurPlayer->AddGold(-m_vecWeapon[iMenu - 1]->GetiPrice()); // 물건 값을 지불
			m_tCurPlayer->AddItem(m_vecWeapon[iMenu - 1]); // 상점 물건을 복사하여 데입.

			cout << "아이템을 구매했습니다. " << endl;
		}
		else
		{
			cout << "잔액이 부족합니다." << endl;
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

		SHOW2(m_vecRuby.size() + 1 << ". 뒤로가기");
		cout << endl << "소지금 : " << m_tCurPlayer->GetGold() << endl;
		cout << "어떤 물건을 사시겠습니까 ? : ";

		iMenu = Input<int>();

		if (iMenu == m_vecRuby.size() + 1)
			return;

		else if (iMenu <= 0 || iMenu > m_vecRuby.size() + 1)
			continue;

			// 돈이 있는지 체크하고 있으면 m_vec에 넣어버림.

			if (m_tCurPlayer->GetGold() >= m_vecRuby[iMenu - 1]->GetiPrice())
			{
				// 물건을 구매한다.
				m_tCurPlayer->AddGold(-m_vecRuby[iMenu - 1]->GetiPrice()); // 물건 값을 지불
				m_tCurPlayer->AddItem(m_vecRuby[iMenu - 1]); // 상점 물건을 복사하여 데입.

				cout << "아이템을 구매했습니다. " << endl;
			}
			else
			{
				cout << "잔액이 부족합니다." << endl;
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
		SHOW("무기 상점");
		iter = m_vecWeapon.begin();
		iterEnd = m_vecWeapon.end();
		break;

	case OT_ARMOR:
		SHOW("방어구 상점");
		iter = m_vecArmor.begin();
		iterEnd = m_vecArmor.end();
		break;

	case OT_RUBY:
		SHOW("장신구 상점");
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

