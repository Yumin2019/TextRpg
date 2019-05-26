#include "CStage.h"
#include "CObjManager.h"
#include "CMonster.h"
#include "CFilestream.h"
#include "CStageManager.h"

CStage::CStage()
{
	memset(strStageName, 0, NAME_SIZE_MAX);
	iMonsterCount = 0;
}


CStage::~CStage()
{
	Safe_Delete_VecList(m_vecMonster);
}

void CStage::Run()
{
	
	// 플레이어의 정보를 받아온다.
	CPlayer*	pPlayer = GET_SINGLE(CObjManager)->m_tCurPlayer;

	// 몬스터 정보를 받아온다.

	int idx = rand() % iMonsterCount;

	CMonster*	pMonster = m_vecMonster[idx]->Clone();

	while (true)
	{
		system("cls");
		cout << endl << " << " << strStageName << " >>" << endl;

		pPlayer->Render();
		pMonster->Render();

		int iMenu = OutPutMenu();

		switch (iMenu)
		{
		case CHOICE_ATTACK:
			

			while (true)
			{
				int iAttack = pPlayer->GetPlayerAttack() *pPlayer->GetAttackPercent(); // 플레이어 공격력 : 장비 능력치 + 자신의 능력치 ~ 중에 랜덤 하나 
																					   // * 1.xx 추가 공격력 

				int iCritical = rand() % 100;

			

			
				if (iCritical < pPlayer->GetCriticalPercent())
				{
					if (pPlayer->GetJob() == JOB_SWORD)
					{
						cout << "<< 패시브 강한 치명타 >> " << endl;
						iAttack *= 2.5;
					}
					else
					{
						cout << " << 치명타 >> " << endl;
						iAttack *= 2;
					}
				}
				iAttack -= pMonster->GetArmor();

				iAttack = iAttack < 1 ? 1 : iAttack;

				cout << pPlayer->GetName() << " 이(가) " << pMonster->GetName() << " 을(를) " << "공격하여 " << iAttack << " 의 피해를 입혔다." << endl << endl;


				pMonster->AddHP(-iAttack);

				if (pMonster->DieCheck())
				{
					int iGold = pMonster->GetDropGold();
					long long iExp = pMonster->GetExp();
					// 몬스터 사망
					cout << pMonster->GetName() << " 이(가) 사망했습니다. " << endl;
					

					if (pPlayer->GetJob() == JOB_THIEF)
					{
						cout << " << 패시브 : 약탈 >> " << endl;
						cout << iGold << " + " << iGold * (pPlayer->GetLuckyPercent() - 1) << " Gold 를 얻었습니다. " << endl;
						cout << iExp << " + " << iExp * (pPlayer->GetLuckyPercent() - 1) << " Exp 를 얻었습니다. " << endl;

						iGold += iGold * (pPlayer->GetLuckyPercent() - 1);
						iExp += iExp * (pPlayer->GetLuckyPercent() - 1);
					}

					else
					{
						cout << pPlayer->GetName() << " 은(는) " << iGold << " Gold 를 얻었습니다. " << endl;
						cout << pPlayer->GetName() << " 은(는) " << iExp << " Exp 를 얻었습니다. " << endl;
					}
					pPlayer->AddExp(iExp);
					pPlayer->AddGold(iGold);

				

					// LV 검사
					if (pPlayer->LvCheck())
					{
						// 레벨업을 했다면...
						system("cls");

						long long iExp = pPlayer->GetLevelUpExp(); // 줄여야 하는 EXP
						pPlayer->AddExp(-iExp);
						pPlayer->LvUp(); // 스탯 기본 1 받음.
						pPlayer->AddLevelUpInfo();
						// 경험치 줄이고 레벨 1 올리고, 능력치 주고 체력 풀 
						pPlayer->PlayerFullHP();
						iGold = pPlayer->m_tPlayer.iGold * (pPlayer->GetLuckyPercent() - 1) + (pPlayer->GetLevel() * 500); 
						// 행운력에 따라 얻는 골드를 구한다. 내 골드 * percent + 500 * 레벨

						cout << "============ Lv UP ============" << endl;
						cout << pPlayer->strName << " 은(는) " << pPlayer->GetLevel() << " Lv 이 되었습니다. " << endl;

						cout << "행운력 추가 골드 : " << iGold << " Gold+" << endl;

						pPlayer->AddGold(iGold); // 추가 골드를 저장.
						// 스탯 찍기. 

						pPlayer->OutPutPointMenu();

						pPlayer->AddLevelUpPrint();
						// 퍼즈

					}

					// 몬스터 재생성
					system("pause");

					idx = rand() % iMonsterCount;

					SAFE_DELETE(pMonster);

					pMonster = m_vecMonster[idx]->Clone();

					break;
				}

				// 몬스터가 살았으면
				
				iAttack = pMonster->GetAttack();
				iAttack -= (pPlayer->GetPlayerArmor() * pPlayer->GetArmorPercent() );

				iAttack = iAttack < 1 ? 1 : iAttack;

				int K = rand() % 100;


				if (K < pPlayer->GetAvoidPercent()) // 아이템 + 능력치
				{
					cout << " << 패시브 : 막기 >>" << endl;
					cout << pMonster->strName << " 의 공격을 막았다." << endl;
					
					system("pause");

				}
				
				else
				{
						cout << pMonster->GetName() << " 이(가) " << pPlayer->GetName() << " 을(를) " << "공격하여 " 
							<< iAttack << " 의 피해를 입혔다." << endl << endl;

						pPlayer->PlayerAddHP(-iAttack);
				}

				if (pPlayer->PlayerDieCheck())
				{
					// 플레이어가 죽었다면

					int iGold = (pPlayer->GetGold()) * 0.3;
					long long iExp = (pPlayer->GetExp()) * 0.3;
					
					if (pPlayer->GetJob() == JOB_ARCHER)
					{
						cout << " << 패시브 : Save Gold & Exp >>" << endl;
						cout << pPlayer->strName << " 은(는) 재빠르게 Exp와 Gold를 일부 저장합니다." << endl;
						iGold -= pPlayer->GetAvoidPercent() * 150;
						iExp -= pPlayer->GetAvoidPercent() * 150;
						
						iGold = iGold <= 0 ? 1 : iGold;
						iExp = iExp <= 0 ? 1 : iExp;
					}


					cout << pPlayer->GetName() << " 이(가) 사망했습니다. " << endl;
					cout << pPlayer->GetName() << " 은(는) " << iGold << " Gold 를 빼앗겼습니다.. " << endl;
					cout << pPlayer->GetName() << " 은(는) " << iExp << " Exp 를 빼앗겼습니다. " << endl;

					pPlayer->AddExp(-iExp);
					pPlayer->AddGold(-iGold);
					// 100 - > 70

					pPlayer->PlayerFullHP();

					system("pause");

					break;
				}

				system("pause");

				break;
			}


			break;


		case CHOICE_POTION: // 포션을 먹는다.
			pPlayer->Magic();

			break;

		case CHOICE_BACK:
			SAFE_DELETE(pMonster);

			return;
		}
	}
}

void CStage::SetStage(int StageNumber)
{
	iMonsterCount = 0;
	iStageNumber = StageNumber;
}

int CStage::OutPutMenu()
{
	while (true)
	{
		cout << endl << endl;
		cout << "1. 공격 " << endl;
		cout << "2. 포션 먹기" << endl;
		cout << "3. 도망가기 " << endl;

		cout << "메뉴를 선택하세요 : ";
		int iMenu = Input<int>();

		if (iMenu < 1 || iMenu > 3)
			continue;

		cout << endl;
		return iMenu;
	}
}

void CStage::Render() // 스테이지 렌더기능은 인게임에서 사용하지 않음.
{
	
	cout << endl;
	cout << "스테이지 이름 : " << strStageName << endl;
	cout << "가진 몬스터 : " << iMonsterCount << " 마리" << endl;
	cout << "스테이지 넘버 : " << iStageNumber << endl;
	for (int i = 0; i < iMonsterCount; ++i)
	{
		cout << i + 1 << ". < " << m_vecMonster[i]->strName << " > " << endl;
	}
}

void CStage::Save(CFilestream * file)
{
	int iSize = strlen(strStageName);

	// 이름의 크기를 저장한다.

	file->Write(&iSize, 4);

	// 크기 만큼 이름을 저장한다.

	file->Write(strStageName, iSize);


	// 스테이지 넘버를 저장한다.

	file->Write(&iStageNumber, 4);

	// 몬스터의 수 and 벡터부분은 Init에서 처리함.

}

void CStage::Load(CFilestream * file)
{
	int iSize = 0;

	// 이름의 크기를 읽는다.

	file->Read(&iSize, 4);

	// 크기 만큼 이름을 읽는다.

	file->Read(strStageName, iSize);


	// 스테이지 넘버를 읽는다.

	int iNumber = 0;
	file->Read(&iNumber, 4);

	iStageNumber = iNumber;
	
	// 몬스터의 수 and 벡터부분은 Init에서 처리함.
}

bool CStage::Init(int iNumber)
{
	// 스테이지가 가지고 있는 몬스터 정보를 수집한다.

	// 몬스터 벡터에서 숫자를 봐야한다.

	// 각 스테이지 별 몬스터를 가지고 와서 처리한다.

	vector<CMonster*>::iterator iter =
		GET_SINGLE(CObjManager)->m_tMonster.begin();

	vector<CMonster*>::iterator iterEnd =
		GET_SINGLE(CObjManager)->m_tMonster.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->iStageNumber == iNumber)
		{
			m_vecMonster.push_back((*iter)->Clone());
			++iMonsterCount;
		}
	}


	return true;
}




