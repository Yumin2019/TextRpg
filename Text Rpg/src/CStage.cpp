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
	
	// �÷��̾��� ������ �޾ƿ´�.
	CPlayer*	pPlayer = GET_SINGLE(CObjManager)->m_tCurPlayer;

	// ���� ������ �޾ƿ´�.

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
				int iAttack = pPlayer->GetPlayerAttack() *pPlayer->GetAttackPercent(); // �÷��̾� ���ݷ� : ��� �ɷ�ġ + �ڽ��� �ɷ�ġ ~ �߿� ���� �ϳ� 
																					   // * 1.xx �߰� ���ݷ� 

				int iCritical = rand() % 100;

			

			
				if (iCritical < pPlayer->GetCriticalPercent())
				{
					if (pPlayer->GetJob() == JOB_SWORD)
					{
						cout << "<< �нú� ���� ġ��Ÿ >> " << endl;
						iAttack *= 2.5;
					}
					else
					{
						cout << " << ġ��Ÿ >> " << endl;
						iAttack *= 2;
					}
				}
				iAttack -= pMonster->GetArmor();

				iAttack = iAttack < 1 ? 1 : iAttack;

				cout << pPlayer->GetName() << " ��(��) " << pMonster->GetName() << " ��(��) " << "�����Ͽ� " << iAttack << " �� ���ظ� ������." << endl << endl;


				pMonster->AddHP(-iAttack);

				if (pMonster->DieCheck())
				{
					int iGold = pMonster->GetDropGold();
					long long iExp = pMonster->GetExp();
					// ���� ���
					cout << pMonster->GetName() << " ��(��) ����߽��ϴ�. " << endl;
					

					if (pPlayer->GetJob() == JOB_THIEF)
					{
						cout << " << �нú� : ��Ż >> " << endl;
						cout << iGold << " + " << iGold * (pPlayer->GetLuckyPercent() - 1) << " Gold �� ������ϴ�. " << endl;
						cout << iExp << " + " << iExp * (pPlayer->GetLuckyPercent() - 1) << " Exp �� ������ϴ�. " << endl;

						iGold += iGold * (pPlayer->GetLuckyPercent() - 1);
						iExp += iExp * (pPlayer->GetLuckyPercent() - 1);
					}

					else
					{
						cout << pPlayer->GetName() << " ��(��) " << iGold << " Gold �� ������ϴ�. " << endl;
						cout << pPlayer->GetName() << " ��(��) " << iExp << " Exp �� ������ϴ�. " << endl;
					}
					pPlayer->AddExp(iExp);
					pPlayer->AddGold(iGold);

				

					// LV �˻�
					if (pPlayer->LvCheck())
					{
						// �������� �ߴٸ�...
						system("cls");

						long long iExp = pPlayer->GetLevelUpExp(); // �ٿ��� �ϴ� EXP
						pPlayer->AddExp(-iExp);
						pPlayer->LvUp(); // ���� �⺻ 1 ����.
						pPlayer->AddLevelUpInfo();
						// ����ġ ���̰� ���� 1 �ø���, �ɷ�ġ �ְ� ü�� Ǯ 
						pPlayer->PlayerFullHP();
						iGold = pPlayer->m_tPlayer.iGold * (pPlayer->GetLuckyPercent() - 1) + (pPlayer->GetLevel() * 500); 
						// ���¿� ���� ��� ��带 ���Ѵ�. �� ��� * percent + 500 * ����

						cout << "============ Lv UP ============" << endl;
						cout << pPlayer->strName << " ��(��) " << pPlayer->GetLevel() << " Lv �� �Ǿ����ϴ�. " << endl;

						cout << "���� �߰� ��� : " << iGold << " Gold+" << endl;

						pPlayer->AddGold(iGold); // �߰� ��带 ����.
						// ���� ���. 

						pPlayer->OutPutPointMenu();

						pPlayer->AddLevelUpPrint();
						// ����

					}

					// ���� �����
					system("pause");

					idx = rand() % iMonsterCount;

					SAFE_DELETE(pMonster);

					pMonster = m_vecMonster[idx]->Clone();

					break;
				}

				// ���Ͱ� �������
				
				iAttack = pMonster->GetAttack();
				iAttack -= (pPlayer->GetPlayerArmor() * pPlayer->GetArmorPercent() );

				iAttack = iAttack < 1 ? 1 : iAttack;

				int K = rand() % 100;


				if (K < pPlayer->GetAvoidPercent()) // ������ + �ɷ�ġ
				{
					cout << " << �нú� : ���� >>" << endl;
					cout << pMonster->strName << " �� ������ ���Ҵ�." << endl;
					
					system("pause");

				}
				
				else
				{
						cout << pMonster->GetName() << " ��(��) " << pPlayer->GetName() << " ��(��) " << "�����Ͽ� " 
							<< iAttack << " �� ���ظ� ������." << endl << endl;

						pPlayer->PlayerAddHP(-iAttack);
				}

				if (pPlayer->PlayerDieCheck())
				{
					// �÷��̾ �׾��ٸ�

					int iGold = (pPlayer->GetGold()) * 0.3;
					long long iExp = (pPlayer->GetExp()) * 0.3;
					
					if (pPlayer->GetJob() == JOB_ARCHER)
					{
						cout << " << �нú� : Save Gold & Exp >>" << endl;
						cout << pPlayer->strName << " ��(��) ������� Exp�� Gold�� �Ϻ� �����մϴ�." << endl;
						iGold -= pPlayer->GetAvoidPercent() * 150;
						iExp -= pPlayer->GetAvoidPercent() * 150;
						
						iGold = iGold <= 0 ? 1 : iGold;
						iExp = iExp <= 0 ? 1 : iExp;
					}


					cout << pPlayer->GetName() << " ��(��) ����߽��ϴ�. " << endl;
					cout << pPlayer->GetName() << " ��(��) " << iGold << " Gold �� ���Ѱ���ϴ�.. " << endl;
					cout << pPlayer->GetName() << " ��(��) " << iExp << " Exp �� ���Ѱ���ϴ�. " << endl;

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


		case CHOICE_POTION: // ������ �Դ´�.
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
		cout << "1. ���� " << endl;
		cout << "2. ���� �Ա�" << endl;
		cout << "3. �������� " << endl;

		cout << "�޴��� �����ϼ��� : ";
		int iMenu = Input<int>();

		if (iMenu < 1 || iMenu > 3)
			continue;

		cout << endl;
		return iMenu;
	}
}

void CStage::Render() // �������� ��������� �ΰ��ӿ��� ������� ����.
{
	
	cout << endl;
	cout << "�������� �̸� : " << strStageName << endl;
	cout << "���� ���� : " << iMonsterCount << " ����" << endl;
	cout << "�������� �ѹ� : " << iStageNumber << endl;
	for (int i = 0; i < iMonsterCount; ++i)
	{
		cout << i + 1 << ". < " << m_vecMonster[i]->strName << " > " << endl;
	}
}

void CStage::Save(CFilestream * file)
{
	int iSize = strlen(strStageName);

	// �̸��� ũ�⸦ �����Ѵ�.

	file->Write(&iSize, 4);

	// ũ�� ��ŭ �̸��� �����Ѵ�.

	file->Write(strStageName, iSize);


	// �������� �ѹ��� �����Ѵ�.

	file->Write(&iStageNumber, 4);

	// ������ �� and ���ͺκ��� Init���� ó����.

}

void CStage::Load(CFilestream * file)
{
	int iSize = 0;

	// �̸��� ũ�⸦ �д´�.

	file->Read(&iSize, 4);

	// ũ�� ��ŭ �̸��� �д´�.

	file->Read(strStageName, iSize);


	// �������� �ѹ��� �д´�.

	int iNumber = 0;
	file->Read(&iNumber, 4);

	iStageNumber = iNumber;
	
	// ������ �� and ���ͺκ��� Init���� ó����.
}

bool CStage::Init(int iNumber)
{
	// ���������� ������ �ִ� ���� ������ �����Ѵ�.

	// ���� ���Ϳ��� ���ڸ� �����Ѵ�.

	// �� �������� �� ���͸� ������ �ͼ� ó���Ѵ�.

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




