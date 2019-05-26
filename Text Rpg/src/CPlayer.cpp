#include "CPlayer.h"
#include "Flag.h"
#include "CFilestream.h"
#include "CItem.h"
#include "CObjManager.h"

CPlayer::CPlayer(const CPlayer & p) :
	CCharacter(p)
{
	eJob = p.eJob;
	m_tPlayer = p.m_tPlayer;
	
	// 불러오기를 해도 lvinfo는 같음.

	m_tItem[EQ_ARMOR-1] = p.m_tItem[EQ_ARMOR-1];
	m_tItem[EQ_WEAPON-1] = p.m_tItem[EQ_WEAPON-1];
	m_tItem[EQ_RUBY-1] = p.m_tItem[EQ_RUBY-1];

}

CPlayer::CPlayer()	:
	CCharacter()
{
	m_tLevelUpInfo = {};
	m_tItem[EQ_ARMOR - 1] = NULL;
	m_tItem[EQ_WEAPON - 1] = NULL;
	m_tItem[EQ_RUBY - 1] = NULL;
	m_tAllItem = {};
	eJob = JOB_NONE;
	m_tPlayer = {};
}


CPlayer::~CPlayer()
{
	SAFE_DELETE(m_tItem[EQ_ARMOR - 1]);
	SAFE_DELETE(m_tItem[EQ_WEAPON - 1]);
	SAFE_DELETE(m_tItem[EQ_RUBY - 1]);
	LevelUpTable.clear();

	Safe_Delete_VecList(m_vecInventory);
}

void CPlayer::SellStore()
{
	// 가지고 있는 아이템 정보를 출력한다.
	// 장착하고 있는 아이템은 말고 / 인벤토리에 있는 것만 판매가능.
	int iMenu;
	while (true)
	{
		int iSize = m_vecInventory.size();

		system("cls");

		cout << endl;
		SHOW2("나의 인벤토리");

		for (int i = 0; i < iSize; ++i)
		{
			// 각 아이템의 Render기능을 사용한다.
			SHOW2(i + 1);
			m_vecInventory[i]->Render();
			cout << endl;
		}
		SHOW2(iSize + 1 << ". 뒤로가기");

		cout << "소지금 : " << GetGold() << " Gold" << endl;

		cout << "판매할 장비를 선택하세요 : ";

		iMenu = Input<int>();

		if (iMenu <= 0 || iMenu > iSize + 1)
			continue;

		else if (iMenu == iSize + 1)
			return;

			int idx = iMenu - 1;

			vector<CItem*>::iterator iter = m_vecInventory.begin() + idx;
			ItemSell(iter);
		
	}
}

void CPlayer::Hospital()
{
	system("cls");
	cout << endl;
	SHOW("+ 병원 +");
	int iGold = (700 * m_tInfo.iLevel);

	cout << "병원비 : " << iGold << endl;

	if (Stop())
	{
		cout << "아프지 않은데 병원을 찾아갔다. - 0 Gold" << endl;
	}

	else if (m_tPlayer.iGold < (700 * m_tInfo.iLevel)) // 병원비 700 * 레벨
	{
		cout << endl;
		iGold = (700 * m_tInfo.iLevel) - m_tPlayer.iGold;
		cout << iGold << " Gold 가 부족합니다. " << endl;

	}

	else
	{
		// 돈이 더 많음.
		cout << endl;
		cout << "HP와 MP가 회복되었습니다." << endl;
		cout << iGold << " Gold 를 빼앗겼습니다." << endl;
		AddGold(-iGold);
		PlayerFullHP();
	}

	cout << "현재 금액 : " << m_tPlayer.iGold << endl << endl;

	system("pause");
}

void CPlayer::LevelInfoRender(LEVELUPINFO Info)
{

	cout << "<< 레벨당 성장 능력치 >> " << endl << endl;
	cout << "HP : " << Info.iHP << endl;
	cout << "MP : " << Info.iMP << endl;
	cout << "공격력 : " << Info.iAttackMin << " ~ " << Info.iAttackMax << endl;
	cout << "방어력 : " << Info.iArmorMin << " ~ " << Info.iArmorMax << endl;
	cout << "추가 공격력 : " << Info.iAttackPercent << "%" << endl;
	cout << "추가 방어력 : " << Info.iArmorPercent << "%" << endl;
	cout << "추가 회피력 : " << Info.iAvoidPercent << "%" << endl;
	cout << "추가 행운력 : " << Info.iLuckyPercent << "%" << endl;
	cout << "추가 크리티컬 확률 : " << Info.iCriticalPercent << "%" << endl << endl;
}

void CPlayer::StartInfoRender() 
{
	cout << "<< 초기 능력치 >> " << endl << endl;
	cout << "HP : " << m_tInfo.iHP << endl;
	cout << "MP : " << m_tInfo.iMP << endl;
	cout << "공격력 : " << m_tInfo.iAttackMin << " ~ " << m_tInfo.iAttackMax << endl;
	cout << "방어력 : " << m_tInfo.iArmorMin << " ~ " << m_tInfo.iArmorMax << endl;
	cout << "기본 공격력 : " << m_tPlayer.iAttackPercent << "%" << endl;
	cout << "기본 방어력 : " << m_tPlayer.iArmorPercent << "%" << endl;
	cout << "기본 회피력 : " << m_tPlayer.iAvoidPercent << "%" << endl;
	cout << "기본 행운력 : " << m_tPlayer.iLuckyPercent << "%" << endl;
	cout << "기본 크리티컬 확률 : " << m_tPlayer.iCriticalPercent << "%" << endl << endl;
}

void CPlayer::AllItemInit() // 우리는 인벤토리 메뉴에서 장비를 장착할 때 마다 이 함수를 이용해서 들고 있는 아이템의 총 정보를 
								   // 다시 셋팅해야 한다.
{
	ITEMINFO  p = {};
	// 있는 장비들만 계산하여 리턴.

	if (m_tItem[EQ_ARMOR - 1])
		p += m_tItem[EQ_ARMOR - 1]->GetItemInfo();

	if (m_tItem[EQ_WEAPON - 1])
		p += m_tItem[EQ_WEAPON - 1]->GetItemInfo();

	if (m_tItem[EQ_RUBY - 1])
		p += m_tItem[EQ_RUBY - 1]->GetItemInfo();

	m_tAllItem = p;
}


void CPlayer::LoadLevelUpTable()
{
	 // 레벨업 테이블 데이터 복사 오브젝트 -> 플레이어 데이터

	int iSize = GET_SINGLE(CObjManager)->LevelUpTable.size();


	for (int i = 0; i < iSize; ++i)
	{
		LevelUpTable.push_back(GET_SINGLE(CObjManager)->LevelUpTable[i]);
	}
}

void CPlayer::ShowEquipRender()
{
	SHOW2("무기");
	if (m_tItem[EQ_WEAPON - 1])
	{
		cout << endl;
		m_tItem[EQ_WEAPON - 1]->Render();
		cout << endl;

	}
	else
		cout << "X" << endl;

	SHOW2("방어구");
	if (m_tItem[EQ_ARMOR - 1])
	{
		cout << endl;
		m_tItem[EQ_ARMOR - 1]->Render();
		cout << endl;

	}
	else
		cout << "X" << endl;

	SHOW2("장신구");
	if (m_tItem[EQ_RUBY - 1])
	{
		cout << endl;
		m_tItem[EQ_RUBY - 1]->Render();
		cout << endl;

	}
	else
		cout << "X" << endl;

	cout << endl;
}

void CPlayer::JobPlusInfo()
{
	system("cls");
	
	LEVELUPINFO		m_tInfo = {};

	for (int i = 1; i <= 9; ++i)
	{
		switch ((JOB)i)
		{
		case JOB_KNIGHT:
			cout << endl;
			SHOW("전사");
			SetInfo(200, 50, 10, 15, 15, 25, 0, 1);
			SetPlayer(0, 0.5, 0, 0, 0, 500);
			m_tInfo = { 170, 50, 10, 15, 15, 15, 0, 0.7, 0, 0, 0 };

			break;
		case JOB_ARCHER:
			SHOW("궁수");
			SetInfo(150, 100, 15, 20, 10, 15, 0, 1);
			SetPlayer(0, 0, 2, 0, 0, 500);
			m_tInfo = { 150, 100, 15, 20, 10, 12, 0, 0, 1, 0.2 ,0 };
			cout << "<< Save Gold & Exp >> 사망 패널티 감소" << endl << endl;
			break;

		case JOB_WIZARD:
			SHOW("마법사");
			SetInfo(120, 200, 20, 25, 5, 7, 0, 1);
			SetPlayer(0.5, 0, 0, 0, 0, 500);

			m_tInfo = { 100, 150, 20, 25, 5, 7, 0.7, 0, 0, 0 ,0 };

			break;
		case JOB_THIEF:
			SHOW("도적");
			SetInfo(90, 150, 27, 30, 3, 5, 0, 1);
			SetPlayer(0, 0, 0, 2, 0, 500);
			m_tInfo = { 90, 100, 25, 30, 3, 5, 0, 0, 0, 1.2, 0 };
			cout << "<< 패시브 약탈 >> 추가 경험치, 골드 획득 " << endl << endl;
			break;


		case JOB_SWORD:
			SetInfo(120, 70, 20, 25, 5, 10, 0, 1);
			SetPlayer(0, 0, 0, 0, 2, 500);
			m_tInfo = { 90, 40, 20, 25, 5, 10, 0, 0, 0, 0.2, 1 };
			SHOW("검객");
			cout << "<< 패시브 강한 치명타 >> 치명타 추가데미지 50 %" << endl << endl;
			break;


		case JOB_ELF:
			SHOW("엘프");
			SetInfo(100, 130, 15, 20, 7, 8, 0, 1);
			SetPlayer(0, 0, 0, 0, 0, 500);
			m_tInfo = { 100, 150, 15, 20, 5, 7, 0, 0, 0, 0, 0 };
			cout << "<< 패시브 지략 >> 레벨당 마나 + 40" << endl << endl;

			break;

		case JOB_DEMON:
			SHOW("악마");
			SetInfo(80, 100, 20, 25, 6, 7, 0, 1);
			SetPlayer(0, 0, 0, 0, 0, 500);
			m_tInfo = { 110, 30, 22, 25, 3, 5, 0, 0, 0, 0, 0 };

			cout << "<< 패시브 악마 >> 레벨당 공격력 + 4" << endl << endl;
			break;

		case JOB_ORC:
			SHOW("오크");

			m_tInfo = { 170, 10, 5, 10, 20, 25, 0, 0, 0, 0, 0 };
			SetInfo(140, 70, 10, 17, 17, 23, 0, 1);
			SetPlayer(0, 0, 0, 0, 0, 500);
			cout << "<< 패시브 두꺼운 피부 >> 레벨당 방어력 + 4" << endl << endl;

			break;

		case JOB_DRAGON:
			SHOW("드래곤");
			m_tInfo = { 150, 50, 15, 18, 15, 20, 0, 0, 0, 0, 0 };
			SetInfo(90, 120, 15, 16, 18, 20, 0, 1);
			SetPlayer(0, 0, 0, 0, 0, 500);
			cout << "<< 패시브 Dragon >> 레벨당 공격력, 방어력 + 2" << endl << endl;

			break;

		case JOB_GIANT:
			SHOW("자이언트");
			m_tInfo = { 200, 10, 5, 10, 15, 20, 0, 0, 0, 0, 0 };
			SetInfo(210, 50, 10, 14, 15, 17, 0, 1);
			SetPlayer(0, 0, 0, 0, 0, 500);
			cout << "<< 패시브 거인 >> 레벨당 체력 + 40" << endl << endl;

			break;

		}
		StartInfoRender();
		LevelInfoRender(m_tInfo);

	}

	system("pause");
}

void CPlayer::AddItem(CItem * p)
{
	// clone을 써서 가져온 것을 데입
	m_vecInventory.push_back(p->Clone());
}

void CPlayer::InventoryItemList()
{

	int iMenu;
	while (true)
	{
		int iSize = m_vecInventory.size();

		AllItemInit();

		system("cls");

		cout << endl;
		SHOW2("인벤토리");
		Render(); // 플레이어 정보 출력.

		for (int i = 0; i < iSize; ++i)
		{
			// 각 아이템의 Render기능을 사용한다.
			SHOW2(i + 1);
			m_vecInventory[i]->Render();
			cout << endl;
		}

		SHOW2(iSize + 1 << ". 아이템 장착 해제");
		SHOW2(iSize + 2 << ". 뒤로가기");
	
		cout << "장착할 장비를 선택하세요 : ";

		iMenu = Input<int>();

		if (iMenu <= 0 || iMenu > iSize + 2)
			continue;

		else if (iMenu == iSize + 2)
			return;

		else if (iMenu == iSize + 1 && !ItemUse() ) // 아이템 해제를 할 건데... 3 항목에 아이템이 존재하지 않음.
		{
			cout << endl;
			cout << "장착한 아이템이 없습니다." << endl;

			system("pause");
		}


		else if (iMenu == iSize + 1) // 어떤 아이템을 해제할지 정하고 그걸 인벤토리에 넣는 과정.
			{
					while (true)
					{
						bool ok = false;
								system("cls");
								cout << endl;


								// 장착한 아이템을 보여준다.
								SHOW("장착한 아이템");

								ShowEquipRender();

								SHOW2("1. 무기 해제");
								SHOW2("2. 방어구 해제");
								SHOW2("3. 장신구 해제");
								SHOW2("4. 뒤로가기");
								cout << "해제할 장비를 선택하세요: " << endl;
								iMenu = Input<int>();

								if (iMenu <= EQ_NONE || iMenu > 4)
									continue;

								else if (iMenu == 4)
									break;

								switch (iMenu)
								{
								case EQ_WEAPON:
									if (m_tItem[EQ_WEAPON - 1])
									{
										ReturnItem(m_tItem[EQ_WEAPON - 1]);
									}
									else
									{
										cout << "장착한 아이템이 없습니다." << endl;
										system("pause");

									}
									break;

								case EQ_ARMOR:
									if (m_tItem[EQ_ARMOR - 1])
									{
										ReturnItem(m_tItem[EQ_ARMOR - 1]);
									}
									else
									{
										cout << "장착한 아이템이 없습니다." << endl;
										system("pause");


									}
									break;

								case EQ_RUBY:
									if (m_tItem[EQ_RUBY - 1])
									{
										ReturnItem(m_tItem[EQ_RUBY - 1]);
									}
									else
									{
										cout << "장착한 아이템이 없습니다." << endl;
										system("pause");

									}
									break;
								}


								AllItemInit();


					}
				}
		// 선택한 장비를 착용한다. 장비를 착용하고 있는지, 없는지에 따라서 변경됨.
		else
		{
						int idx = iMenu - 1;

						// 선택한 장비의 EQUIP을 가져온다.
						if (SetItem(m_vecInventory[idx]))
						{
							// 그냥 교체면 넘어가고 착용하지 않은 상태일 시에는 여기서 걸림.

							vector<CItem*>::iterator iter = m_vecInventory.begin() + idx;

							m_vecInventory.erase(iter); // 해당 요소만 삭제하고 떙긴다.
						}

						AllItemInit();
		}


	}


}


void CPlayer::ShowEquipItem()
{
	cout << "무기 : ";
	if (m_tItem[EQ_WEAPON - 1])
	{
		cout << m_tItem[EQ_WEAPON - 1]->strName << endl;
	}
	else
		cout << "X" << endl;

	cout << "방어구 : ";
	if (m_tItem[EQ_ARMOR - 1])
	{
		cout << m_tItem[EQ_ARMOR - 1]->strName << endl;
	}
	else
		cout << "X" << endl;

	cout << "장신구 : ";
	if (m_tItem[EQ_RUBY - 1])
	{
		cout << m_tItem[EQ_RUBY - 1]->strName << endl;
	}
	else
		cout << "X" << endl;

	cout << endl;
}

bool CPlayer::SetItem(CItem* & p)
{
	// 장비와 오브젝트 타입을 받아서 이것을 EQUIP로 호환하여 데입을 하려고 한다.

	EQUIP eq = EQ_NONE;

	switch (p->OT_type)
	{
	case OT_WEAPON:
		eq = EQ_WEAPON;
		break;
	case OT_ARMOR:
		eq = EQ_ARMOR;
		break;
	case OT_RUBY:
		eq = EQ_RUBY;
		break;

	}

	// 해당 칸에 장비를 끼고 있는지 없는지로 나뉜다. 

	if (m_tItem[eq-1]) // 장비가 있으면 지금 끼고 있던 장비와 착용하려는 장비를 바꿔야한다.
	{
		CItem*	temp = m_tItem[eq-1];
		m_tItem[eq-1] = p;
		p = temp; 
		// temp가 가지고 있던 장비를 인벤토리에..
		return false; // 그냥 가도 된다.
	}

	else
	{
		// 장비가 없으면 땡겨주는 과정이 필요함. 벡터의 erase이용.
		m_tItem[eq - 1] = p->Clone();
		return true;
	}
}

void CPlayer::ReturnItem(CItem *& p)
{
	// 고른 장비를 인벤토리에 넣는다.

	m_vecInventory.push_back(p->Clone());
	SAFE_DELETE(p);

	cout << "아이템을 해제했습니다." << endl;
	system("pause");
}

void CPlayer::ItemSell(vector<CItem*>::iterator iter)
{
	// 고른 아이템을 판매한다.

	// 장착하고 있는 아이템은 못팔고 인벤토리에 있는 것 중에서 선택할 수 있도록 한다.
	while (true)
	{
		cout << endl;
		cout << "정말로 << " << (*iter)->strName << " >>  을(를) 판매하시겠습니까 ? " << endl << endl;
		SHOW2("1. 그렇다");
		SHOW2("2. 아니다");
		
		int iMenu = Input<int>();

		if (iMenu <= 0 || iMenu > 2)
			continue;


		else if (iMenu == 2)
			return;

		break;
	}
	
	// 고른 장비를 판매한다.
	int iGold = (*iter)->GetiSell();
	AddGold(iGold);

	m_vecInventory.erase(iter);
	cout << "장비를 판매했습니다" << endl;
	cout << iGold << " Gold 를 얻었습니다. " << endl;
	
	system("pause");
}

bool CPlayer::CheckEquip()
{
	if(m_tItem[EQ_RUBY-1] || m_tItem[EQ_ARMOR-1] || m_tItem[EQ_WEAPON-1])
		return false;

	return true;
}

void CPlayer::SetPlayer(double iAttack, double iArmor, double iAvoid, double iLuck, int iCritical, int iMoney)
{
	m_tPlayer.iAttackPercent = iAttack;
	m_tPlayer.iArmorPercent = iArmor;
	m_tPlayer.iLuckyPercent = iLuck;
	m_tPlayer.iAvoidPercent = iAvoid;
	m_tPlayer.iGold = iMoney;
	m_tPlayer.iPoint = 0;
	m_tPlayer.iCriticalPercent = iCritical;
}


void CPlayer::OutPutPointMenu() // 레벨업 시에 바로 찍도록 한다.
{
	while (m_tPlayer.iPoint!= 0)
	{
		system("cls");
		cout << endl << endl;
		SHOW("스탯 포인트");
		cout << "1. 공격력 1% (공격력이 증가합니다.)" << endl << endl;
		cout << "2. 방어력 1% (방어력이 증가합니다.)" << endl << endl;
		cout << "3. 회피력 2% (회피 확률이 증가합니다.)" << endl << endl;
		cout << "4. 행운력 2% (레벨업 보상으로 골드를 얻습니다. / 도적 : 패시브 효과 상승)" << endl << endl;
		cout << "5. 치명타 확률 2% (치명타 확률이 증가합니다.)" << endl << endl;

		cout << "추가 공격력 : " << (m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent) << "%" << endl;
		cout << "추가 방어력 : " << (m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent) << "%" << endl;
		cout << "추가 회피력 : " << m_tPlayer.iAvoidPercent + m_tAllItem.iAvoidPercent << "%" << endl;
		cout << "추가 행운력 : " << m_tPlayer.iLuckyPercent + m_tAllItem.iLuckyPercent << "%" << endl;
		cout << "크리티컬 확률 : " << m_tAllItem.iCriticalPercent + m_tPlayer.iCriticalPercent << "%" << endl << endl;
		
		cout << "스탯을 선택하세요 : ";
		int iMenu = Input<int>();

		if (iMenu <= POINT_NONE || iMenu > POINT_CRITICAL)
			continue;

		// 고른 것이 최대 범위인지 확인하고 증가시킨다.

		switch (iMenu)
		{
		case POINT_ATTACK:
			m_tPlayer.iAttackPercent += 1;
			--m_tPlayer.iPoint;
			cout << "추가 공격력 + 1% " << endl;

			break;
		case POINT_ARMOR:
			m_tPlayer.iArmorPercent += 1;
			--m_tPlayer.iPoint;

			cout << "추가 방어력 + 1% " << endl;

			break;
		case POINT_AVOID:
			
			if (m_tPlayer.iAvoidPercent + 2 > AVOID_POINT_MAX)
			{
				cout << "이미 최댓값입니다." << endl;
				break;
			}

			m_tPlayer.iAvoidPercent += 2;
			cout << "추가 회피력 + 2%" << endl;
			--m_tPlayer.iPoint;

			break;
		case POINT_LUCK:
			m_tPlayer.iLuckyPercent += 2;
			--m_tPlayer.iPoint;

			cout << "추가 행운력 + 2%" << endl;
			break;

		case POINT_CRITICAL:

			if (m_tPlayer.iCriticalPercent + 2 > CRITICAL_POINT_MAX)
			{
				cout << "이미 최댓값입니다." << endl;
				break;
			}

			m_tPlayer.iCriticalPercent += 2;
			--m_tPlayer.iPoint;

			cout << "추가 치명타 확률 + 2%" << endl;
			
			break;
		}

		system("pause");

	}
	
}

void CPlayer::AddLevelUpInfo()
{
	//레벨업 능력치를 올린다.    특수 퍼센트 체력 마나 공격력 방어력 다 같이.


	// 체력 마나
	m_tInfo.iHPMax += m_tLevelUpInfo.iHP;
	m_tInfo.iMPMax += m_tLevelUpInfo.iMP;
	// 함수로 마나와 체력은 채워줌.

	// 공격력
	m_tInfo.iAttackMin += m_tLevelUpInfo.iAttackMin;
	m_tInfo.iAttackMax += m_tLevelUpInfo.iAttackMax;

	// 방어력
	m_tInfo.iArmorMin += m_tLevelUpInfo.iArmorMin;
	m_tInfo.iArmorMax += m_tLevelUpInfo.iArmorMax;

	// 공격 방어 회피 행운 치명타 확률 중에서 치명타와 회피는 상황을 보고 올려야 함.
	m_tPlayer.iAttackPercent += m_tLevelUpInfo.iAttackPercent;
	m_tPlayer.iArmorPercent += m_tLevelUpInfo.iArmorPercent;
	m_tPlayer.iLuckyPercent += m_tLevelUpInfo.iLuckyPercent;

	if ((m_tPlayer.iAvoidPercent + m_tLevelUpInfo.iAvoidPercent) > AVOID_POINT_MAX)
	{
		++m_tPlayer.iPoint;
		cout << "회피력이 LV_MAX값에 도달하여 추가 스탯을 얻습니다." << endl;
		system("pause");
	}
	// 어찌보면 빨리 맥스값을 채우고 부스탯을 찍는 상황에서는 기본에 올라가는 %보다 더 높아서 좋을 것 같다.

	else
		m_tPlayer.iAvoidPercent += m_tLevelUpInfo.iAvoidPercent;

	if ((m_tPlayer.iCriticalPercent + m_tLevelUpInfo.iCriticalPercent) > CRITICAL_POINT_MAX)
	{
		++m_tPlayer.iPoint;
		cout << "치명타 확률이 LV_MAX값에 도달하여 추가 스탯을 얻습니다." << endl;
		system("pause");
	}
	else
		m_tPlayer.iCriticalPercent += m_tLevelUpInfo.iCriticalPercent;


	

	switch (eJob)
	{
	case JOB_KNIGHT:
	case JOB_ARCHER:
	case JOB_WIZARD:
	case JOB_THIEF:
	case JOB_SWORD:
	
		return;			
		
		// 인간 외의 종족은 성장 스탯이 없다. -> 대신에 추가 체력 / 마나 등이 주어진다.


	case JOB_GIANT:
		// 추가 체력 40
		cout << "<< 패시브 거인 >> " << endl;
		cout << "체력이 40 만큼 증가합니다. " << endl;
		m_tInfo.iHPMax += 40;
		break;


	case JOB_ELF:
		// 추가 마나 40
		cout << "<< 패시브 지략 >> " << endl;
		cout << "마나가 40 만큼 증가합니다. " << endl;
		m_tInfo.iMPMax += 40;

		break;

	case JOB_DRAGON: // 공격력 2 방어력 2
		cout << "<< 패시브 Dragon >> " << endl;
		cout << "공격력과 방어력이 2 만큼 증가합니다. " << endl;
		m_tInfo.iArmorMax += 2;
		m_tInfo.iArmorMin += 2;
		m_tInfo.iAttackMax += 2;
		m_tInfo.iAttackMin += 2;
		break;

	case JOB_ORC: // 방어력 4
		cout << "<< 패시브 두꺼운 피부 >> " << endl;
		cout << "방어력이 4 만큼 증가합니다. " << endl;
		m_tInfo.iArmorMax += 4;
		m_tInfo.iArmorMin += 4;

		break;

	case JOB_DEMON: // 공격력 4

		cout << "<< 패시브 악마 >> " << endl;
		cout << "공격력이 4 만큼 증가합니다. " << endl;
		m_tInfo.iAttackMax +=4;
		m_tInfo.iAttackMin +=4;
		break;
	}

	system("pause");
}

void CPlayer::AddLevelUpPrint()
{
	// 우리는 플레이어에게 어느 정도의 능력치가 상승했는지 보여주려고 한다.

	// 체력 마나
	cout << "HP : " << m_tLevelUpInfo.iHP << "+" << endl;
	cout << "MP : " << m_tLevelUpInfo.iMP << "+" << endl;
	// 함수로 마나와 체력은 채워줌.

	// 공격력
	cout << "최소 공격력 : " << m_tLevelUpInfo.iAttackMin << "+" << endl;
	cout << "최대 공격력 : " << m_tLevelUpInfo.iAttackMax << "+" << endl;

	// 방어력
	cout << "최소 방어력 : " << m_tLevelUpInfo.iArmorMin << "+" << endl;
	cout << "최대 방어력 : " << m_tLevelUpInfo.iArmorMax << "+" << endl;

	// 공격 방어 회피 럭
		cout << "NOW 방어력 : " << m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent<< "%" << endl;
		cout << "NOW 회피력 : " << m_tPlayer.iAvoidPercent + m_tAllItem.iAvoidPercent << "%" << endl;
		cout << "NOW 공격력 : " << m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent << "%" << endl;
		cout << "NOW 행운력 : " << m_tPlayer.iLuckyPercent + m_tAllItem.iLuckyPercent << "%" << endl;
		cout << "NOW 치명타 확률 : " << m_tPlayer.iCriticalPercent + m_tAllItem.iCriticalPercent << "%" << endl;

		system("pause");
}

void CPlayer::Run()
{

}

bool CPlayer::Init()
{

	AllItemInit(); // 일단은 0으로 셋팅

	Ignore();
	cout << "당신의 이름을 입력하세요 : ";
	char* strname = new char[NAME_SIZE_MAX];
	cin.getline(strname, NAME_SIZE_MAX - 1, '\n');

	int iMenu = 2;

	while (iMenu == 2)
	{
		cout << "당신의 이름이 맞습니까 ? " << strname << endl;
		SHOW2("1. 그렇다");
		SHOW2("2. 아니다");
		iMenu = Input<int>();

		if (iMenu < 1 || iMenu > 2)
			continue;

		else if (iMenu == 2)
		{
			system("cls");
			cout << "다시 입력하세요 : ";
			cin.ignore(1024, '\n');
			cin.getline(strname, NAME_SIZE_MAX - 1, '\n');

			system("pause");
			continue;
		}

		break;
	}
	strcpy_s(strName, strname);
	SAFE_DELETE_ARRAY(strname);

	// 직업 관련해서는 출력할 때 처리.
	int iJob;
	while (true)
	{
		iJob = OutPutMenu();


		 if (iJob == JOB_PLUS)
		{
			JobPlusInfo(); // 상세 정보
		}

		else if (iJob != JOB_NONE)
		{
			eJob = (JOB)iJob;
			break;
		}
	}

	// 각 직업별 레벨업 능력치를 셋팅한다.
	SetLevelUpInfo();


	return true;
}

void CPlayer::Magic()
{
	// 마나를 소모하여 체력을 회복한다.  마나를 체력ㅇ로.
	int iMP = m_tInfo.iLevel * 50;

	int iMenu;

	while (true)
	{
		cout << "정말로 체력 포션(+" << iMP << ")을 먹으시겠습니까 ? (" << iMP << " MP 소모)" << endl;
		cout << "1. 그렇다. " << endl;
		cout << "2. 아니다." << endl;
		iMenu = Input<int>();

		if (iMenu < 1 || iMenu > 2)
			continue;

		break;
	}

	
	switch (iMenu)
	{
	case 1:
		if ( (m_tInfo.iMP + m_tAllItem.iMP ) < iMP)
		{
			cout << "MP 가 부족합니다. " << endl;
			break;
		}

		cout << "체력을 " << iMP << " 회복했습니다. " << endl;
		PlayerAddHP(iMP);
		PlayerAddMP(-iMP);

		break;
	case 2:
		break;
	}
	
	system("pause");
}

int CPlayer::OutPutMenu()
{
	system("cls");
	SHOW("인간");			 
	SHOW2("1. 전사");
	SHOW2("2. 궁수");
	SHOW2("3. 마법사");
	SHOW2("4. 도적");
	SHOW2("5. 검객");

	SHOW("몬스터");

	SHOW2("6. 자이언트");
	SHOW2("7. 엘프");
	SHOW2("8. 악마");
	SHOW2("9. 드래곤");
	SHOW2("10. 오크");
	SHOW2("11. 모든 직업 상세 정보 출력");
	cout << "직업을 선택하세요 : ";
	int iJob = Input<int>();

	if (iJob <= JOB_NONE || iJob > JOB_PLUS)
		return JOB_NONE;

	return iJob;

}

void CPlayer::SetLevelUpInfo()
{

	LEVELUPINFO		m_tInfo = {};

	switch (eJob)
	{
	case JOB_KNIGHT:
		m_tInfo = { 170, 50, 10, 15, 15, 15, 0, 0.7, 0, 0, 0 };
		
		break;
		
	case JOB_ARCHER:
		m_tInfo = { 150, 100, 15, 20, 10, 12, 0, 0, 1, 0.2 ,0 };

		break;

	case JOB_WIZARD:
		m_tInfo = { 100, 150, 20, 25, 5, 7, 0.7, 0, 0, 0 ,0 };
		break;

	case JOB_THIEF:
		m_tInfo = { 90, 100, 25, 30, 3, 5, 0, 0, 0, 1.2, 0 };
		break;

	case JOB_SWORD:
		m_tInfo = { 90, 40, 20, 25, 5, 10, 0, 0, 0, 0.2, 1 };

		break;

		// 인간 외의 종족은 성장 스탯이 없다. -> 대신에 추가 체력 / 마나 등이 주어진다.
	case JOB_GIANT:
		m_tInfo = { 200, 10, 5, 10, 15, 20, 0, 0, 0, 0, 0 };
		break;


	case JOB_ELF:
		m_tInfo = { 100, 150, 15, 20, 5, 7, 0, 0, 0, 0, 0 };

		break;

	case JOB_DRAGON:
		m_tInfo = { 150, 50, 15, 18, 15, 20, 0, 0, 0, 0, 0 };

		break;

	case JOB_ORC:
		m_tInfo = { 170, 10, 5, 10, 20, 25, 0, 0, 0, 0, 0 };

		break;

	case JOB_DEMON:
		m_tInfo = { 110, 30, 22, 25, 3, 5, 0, 0, 0, 0, 0 };

		break;
	}

	/*
	int		iHP;
	int		iMP;
	int		iAttackMin;
	int		iAttackMax;
	int		iArmorMin;
	int		iArmorMax;

	double	iAttackPercent;
	double	iArmorPercent;
	double	iAvoidPercent;
	double	iLuckyPercent;
	int			iCriticalPercent;

	*/

	m_tLevelUpInfo = m_tInfo;

}

void CPlayer::Render()
{
	cout << "======================== 플레이어 ========================" << endl;
	cout << "이름 : " << strName << "   Lv : " << m_tInfo.iLevel;
		
		
	cout << "\t직업 : ";

	switch (eJob)
	{
	case JOB_KNIGHT:
		cout << "전사" << endl;
		break;
	case JOB_ARCHER:
		cout << "궁수" << endl;
		break;
	case JOB_WIZARD:
		cout << "마법사" << endl;
		break;
	case JOB_THIEF:
		cout << "도적" << endl;
		break;
	case JOB_SWORD:
		cout << "검객" << endl;
		break;
	case JOB_ELF:
		cout << "엘프" << endl;
		break;

	case JOB_DEMON:
		cout << "악마" << endl;
		break;

	case JOB_ORC:
		cout << "오크" << endl;
		break;

	case JOB_DRAGON:
		cout << "드래곤" << endl;
		break;

	case JOB_GIANT:
		cout << "거인" << endl;
		break;

	}

	cout << endl;
		cout << "HP : " << m_tInfo.iHP << " + " << m_tAllItem.iHP;
			
			
		cout << " / " << m_tInfo.iHPMax << " + " << m_tAllItem.iHPMax << endl;

		cout << "MP : " << m_tInfo.iMP << " + " << m_tAllItem.iMP <<
			" / " << m_tInfo.iMPMax << " + " << m_tAllItem.iMPMax << endl << endl;

		cout << "최소 공격력 : " << (int)((m_tInfo.iAttackMin + m_tAllItem.iAttackMin) * GetAttackPercent())

			<< " ~ " << (int)((m_tInfo.iAttackMax + m_tAllItem.iAttackMax)  * GetAttackPercent()) << endl;

		cout << "최소 방어력 : " << (int)((m_tInfo.iArmorMin + m_tAllItem.iArmorMin) * GetArmorPercent()) << " ~ "
			<< (int)((m_tInfo.iArmorMax + m_tAllItem.iArmorMax)  * GetArmorPercent()) << endl << endl;

		cout << "추가 공격력 : " << (m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent) << "%  (" <<
			((m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent) / 100) * (m_tInfo.iAttackMin + m_tAllItem.iAttackMin) << " ~ "

			<< ((m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent) / 100) * (m_tInfo.iAttackMax + m_tAllItem.iAttackMax) << ")" << endl;

		cout << "추가 방어력 : " << (m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent) << "%  ("

			<< ((m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent) / 100) * (m_tInfo.iArmorMin + m_tAllItem.iArmorMin) << " ~ "
			<< ((m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent) / 100) * (m_tInfo.iArmorMax + m_tAllItem.iArmorMax) << ")" << endl;

		cout << "추가 회피력 : " << GetAvoidPercent() << "%" << endl;

		

		cout << "추가 행운력 : " << m_tPlayer.iLuckyPercent + m_tAllItem.iLuckyPercent << "%" << endl << endl;

		cout << "크리티컬 확률 : " << GetCriticalPercent() << "%" << endl << endl;
		
		if (m_tInfo.iLevel != MAX_LEVEL)
		{
			cout << "Exp : " << m_tInfo.iExp << " / " << LevelUpTable[m_tInfo.iLevel - 1] << endl;
		}
		else
		{
			cout << "Exp : 99.9 % " << endl;
		}


		cout << "소지금 : " << m_tPlayer.iGold << endl << endl;

	ShowEquipItem();

	cout << endl;

}

CPlayer * CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::Save(CFilestream * file)
{
	CCharacter::Save(file);

	file->Write(&m_tPlayer, sizeof(m_tPlayer));
	file->Write(&eJob, 4);

	file->Write(&m_tLevelUpInfo, sizeof(m_tLevelUpInfo));


	// 장착중인 장비를 인벤토리에 넣어서 함께 저장하도록 한다.

	if (m_tItem[EQ_ARMOR - 1])
	{
		m_vecInventory.push_back(m_tItem[EQ_ARMOR - 1]->Clone());
		SAFE_DELETE(m_tItem[EQ_ARMOR - 1]);

	}

	if (m_tItem[EQ_WEAPON - 1])
	{
		m_vecInventory.push_back(m_tItem[EQ_WEAPON - 1]->Clone());
		SAFE_DELETE(m_tItem[EQ_WEAPON - 1]);

	}

	if (m_tItem[EQ_RUBY - 1])
	{
		m_vecInventory.push_back(m_tItem[EQ_RUBY - 1]->Clone());
		SAFE_DELETE(m_tItem[EQ_RUBY - 1]);
	}


	// 인벤토리 파일을 저장한다.

	int iSize = m_vecInventory.size();

	// 장비 개수를 저장한다.
	file->Write(&iSize, 4);

	// 장비를 저장한다.
	for (int i = 0; i < iSize; ++i)
	{
		m_vecInventory[i]->Save(file);
	}
}

void CPlayer::Load(CFilestream * file)
{
	CCharacter::Load(file);
	file->Read(&m_tPlayer, sizeof(m_tPlayer));
	file->Read(&eJob, 4);
	

	file->Read(&m_tLevelUpInfo, sizeof(m_tLevelUpInfo));

	int iSize = 0;
	// 장비 개수를 읽는다.

	// 장비를 읽을 때 현재 데이터는 날려야 한다. 인벤토리도 마찬가지

	Safe_Delete_VecList(m_vecInventory);
	SAFE_DELETE(m_tItem[EQ_RUBY - 1]);
	SAFE_DELETE(m_tItem[EQ_WEAPON - 1]);
	SAFE_DELETE(m_tItem[EQ_ARMOR - 1]);

	file->Read(&iSize, 4);


	// 장비를 읽는다.
	for (int i = 0; i < iSize; ++i)
	{
		CItem*   pItem = new CItem;
		pItem->Load(file);
		m_vecInventory.push_back(pItem);
	}
}

