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
	
	// �ҷ����⸦ �ص� lvinfo�� ����.

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
	// ������ �ִ� ������ ������ ����Ѵ�.
	// �����ϰ� �ִ� �������� ���� / �κ��丮�� �ִ� �͸� �ǸŰ���.
	int iMenu;
	while (true)
	{
		int iSize = m_vecInventory.size();

		system("cls");

		cout << endl;
		SHOW2("���� �κ��丮");

		for (int i = 0; i < iSize; ++i)
		{
			// �� �������� Render����� ����Ѵ�.
			SHOW2(i + 1);
			m_vecInventory[i]->Render();
			cout << endl;
		}
		SHOW2(iSize + 1 << ". �ڷΰ���");

		cout << "������ : " << GetGold() << " Gold" << endl;

		cout << "�Ǹ��� ��� �����ϼ��� : ";

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
	SHOW("+ ���� +");
	int iGold = (700 * m_tInfo.iLevel);

	cout << "������ : " << iGold << endl;

	if (Stop())
	{
		cout << "������ ������ ������ ã�ư���. - 0 Gold" << endl;
	}

	else if (m_tPlayer.iGold < (700 * m_tInfo.iLevel)) // ������ 700 * ����
	{
		cout << endl;
		iGold = (700 * m_tInfo.iLevel) - m_tPlayer.iGold;
		cout << iGold << " Gold �� �����մϴ�. " << endl;

	}

	else
	{
		// ���� �� ����.
		cout << endl;
		cout << "HP�� MP�� ȸ���Ǿ����ϴ�." << endl;
		cout << iGold << " Gold �� ���Ѱ���ϴ�." << endl;
		AddGold(-iGold);
		PlayerFullHP();
	}

	cout << "���� �ݾ� : " << m_tPlayer.iGold << endl << endl;

	system("pause");
}

void CPlayer::LevelInfoRender(LEVELUPINFO Info)
{

	cout << "<< ������ ���� �ɷ�ġ >> " << endl << endl;
	cout << "HP : " << Info.iHP << endl;
	cout << "MP : " << Info.iMP << endl;
	cout << "���ݷ� : " << Info.iAttackMin << " ~ " << Info.iAttackMax << endl;
	cout << "���� : " << Info.iArmorMin << " ~ " << Info.iArmorMax << endl;
	cout << "�߰� ���ݷ� : " << Info.iAttackPercent << "%" << endl;
	cout << "�߰� ���� : " << Info.iArmorPercent << "%" << endl;
	cout << "�߰� ȸ�Ƿ� : " << Info.iAvoidPercent << "%" << endl;
	cout << "�߰� ���� : " << Info.iLuckyPercent << "%" << endl;
	cout << "�߰� ũ��Ƽ�� Ȯ�� : " << Info.iCriticalPercent << "%" << endl << endl;
}

void CPlayer::StartInfoRender() 
{
	cout << "<< �ʱ� �ɷ�ġ >> " << endl << endl;
	cout << "HP : " << m_tInfo.iHP << endl;
	cout << "MP : " << m_tInfo.iMP << endl;
	cout << "���ݷ� : " << m_tInfo.iAttackMin << " ~ " << m_tInfo.iAttackMax << endl;
	cout << "���� : " << m_tInfo.iArmorMin << " ~ " << m_tInfo.iArmorMax << endl;
	cout << "�⺻ ���ݷ� : " << m_tPlayer.iAttackPercent << "%" << endl;
	cout << "�⺻ ���� : " << m_tPlayer.iArmorPercent << "%" << endl;
	cout << "�⺻ ȸ�Ƿ� : " << m_tPlayer.iAvoidPercent << "%" << endl;
	cout << "�⺻ ���� : " << m_tPlayer.iLuckyPercent << "%" << endl;
	cout << "�⺻ ũ��Ƽ�� Ȯ�� : " << m_tPlayer.iCriticalPercent << "%" << endl << endl;
}

void CPlayer::AllItemInit() // �츮�� �κ��丮 �޴����� ��� ������ �� ���� �� �Լ��� �̿��ؼ� ��� �ִ� �������� �� ������ 
								   // �ٽ� �����ؾ� �Ѵ�.
{
	ITEMINFO  p = {};
	// �ִ� ���鸸 ����Ͽ� ����.

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
	 // ������ ���̺� ������ ���� ������Ʈ -> �÷��̾� ������

	int iSize = GET_SINGLE(CObjManager)->LevelUpTable.size();


	for (int i = 0; i < iSize; ++i)
	{
		LevelUpTable.push_back(GET_SINGLE(CObjManager)->LevelUpTable[i]);
	}
}

void CPlayer::ShowEquipRender()
{
	SHOW2("����");
	if (m_tItem[EQ_WEAPON - 1])
	{
		cout << endl;
		m_tItem[EQ_WEAPON - 1]->Render();
		cout << endl;

	}
	else
		cout << "X" << endl;

	SHOW2("��");
	if (m_tItem[EQ_ARMOR - 1])
	{
		cout << endl;
		m_tItem[EQ_ARMOR - 1]->Render();
		cout << endl;

	}
	else
		cout << "X" << endl;

	SHOW2("��ű�");
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
			SHOW("����");
			SetInfo(200, 50, 10, 15, 15, 25, 0, 1);
			SetPlayer(0, 0.5, 0, 0, 0, 500);
			m_tInfo = { 170, 50, 10, 15, 15, 15, 0, 0.7, 0, 0, 0 };

			break;
		case JOB_ARCHER:
			SHOW("�ü�");
			SetInfo(150, 100, 15, 20, 10, 15, 0, 1);
			SetPlayer(0, 0, 2, 0, 0, 500);
			m_tInfo = { 150, 100, 15, 20, 10, 12, 0, 0, 1, 0.2 ,0 };
			cout << "<< Save Gold & Exp >> ��� �г�Ƽ ����" << endl << endl;
			break;

		case JOB_WIZARD:
			SHOW("������");
			SetInfo(120, 200, 20, 25, 5, 7, 0, 1);
			SetPlayer(0.5, 0, 0, 0, 0, 500);

			m_tInfo = { 100, 150, 20, 25, 5, 7, 0.7, 0, 0, 0 ,0 };

			break;
		case JOB_THIEF:
			SHOW("����");
			SetInfo(90, 150, 27, 30, 3, 5, 0, 1);
			SetPlayer(0, 0, 0, 2, 0, 500);
			m_tInfo = { 90, 100, 25, 30, 3, 5, 0, 0, 0, 1.2, 0 };
			cout << "<< �нú� ��Ż >> �߰� ����ġ, ��� ȹ�� " << endl << endl;
			break;


		case JOB_SWORD:
			SetInfo(120, 70, 20, 25, 5, 10, 0, 1);
			SetPlayer(0, 0, 0, 0, 2, 500);
			m_tInfo = { 90, 40, 20, 25, 5, 10, 0, 0, 0, 0.2, 1 };
			SHOW("�˰�");
			cout << "<< �нú� ���� ġ��Ÿ >> ġ��Ÿ �߰������� 50 %" << endl << endl;
			break;


		case JOB_ELF:
			SHOW("����");
			SetInfo(100, 130, 15, 20, 7, 8, 0, 1);
			SetPlayer(0, 0, 0, 0, 0, 500);
			m_tInfo = { 100, 150, 15, 20, 5, 7, 0, 0, 0, 0, 0 };
			cout << "<< �нú� ���� >> ������ ���� + 40" << endl << endl;

			break;

		case JOB_DEMON:
			SHOW("�Ǹ�");
			SetInfo(80, 100, 20, 25, 6, 7, 0, 1);
			SetPlayer(0, 0, 0, 0, 0, 500);
			m_tInfo = { 110, 30, 22, 25, 3, 5, 0, 0, 0, 0, 0 };

			cout << "<< �нú� �Ǹ� >> ������ ���ݷ� + 4" << endl << endl;
			break;

		case JOB_ORC:
			SHOW("��ũ");

			m_tInfo = { 170, 10, 5, 10, 20, 25, 0, 0, 0, 0, 0 };
			SetInfo(140, 70, 10, 17, 17, 23, 0, 1);
			SetPlayer(0, 0, 0, 0, 0, 500);
			cout << "<< �нú� �β��� �Ǻ� >> ������ ���� + 4" << endl << endl;

			break;

		case JOB_DRAGON:
			SHOW("�巡��");
			m_tInfo = { 150, 50, 15, 18, 15, 20, 0, 0, 0, 0, 0 };
			SetInfo(90, 120, 15, 16, 18, 20, 0, 1);
			SetPlayer(0, 0, 0, 0, 0, 500);
			cout << "<< �нú� Dragon >> ������ ���ݷ�, ���� + 2" << endl << endl;

			break;

		case JOB_GIANT:
			SHOW("���̾�Ʈ");
			m_tInfo = { 200, 10, 5, 10, 15, 20, 0, 0, 0, 0, 0 };
			SetInfo(210, 50, 10, 14, 15, 17, 0, 1);
			SetPlayer(0, 0, 0, 0, 0, 500);
			cout << "<< �нú� ���� >> ������ ü�� + 40" << endl << endl;

			break;

		}
		StartInfoRender();
		LevelInfoRender(m_tInfo);

	}

	system("pause");
}

void CPlayer::AddItem(CItem * p)
{
	// clone�� �Ἥ ������ ���� ����
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
		SHOW2("�κ��丮");
		Render(); // �÷��̾� ���� ���.

		for (int i = 0; i < iSize; ++i)
		{
			// �� �������� Render����� ����Ѵ�.
			SHOW2(i + 1);
			m_vecInventory[i]->Render();
			cout << endl;
		}

		SHOW2(iSize + 1 << ". ������ ���� ����");
		SHOW2(iSize + 2 << ". �ڷΰ���");
	
		cout << "������ ��� �����ϼ��� : ";

		iMenu = Input<int>();

		if (iMenu <= 0 || iMenu > iSize + 2)
			continue;

		else if (iMenu == iSize + 2)
			return;

		else if (iMenu == iSize + 1 && !ItemUse() ) // ������ ������ �� �ǵ�... 3 �׸� �������� �������� ����.
		{
			cout << endl;
			cout << "������ �������� �����ϴ�." << endl;

			system("pause");
		}


		else if (iMenu == iSize + 1) // � �������� �������� ���ϰ� �װ� �κ��丮�� �ִ� ����.
			{
					while (true)
					{
						bool ok = false;
								system("cls");
								cout << endl;


								// ������ �������� �����ش�.
								SHOW("������ ������");

								ShowEquipRender();

								SHOW2("1. ���� ����");
								SHOW2("2. �� ����");
								SHOW2("3. ��ű� ����");
								SHOW2("4. �ڷΰ���");
								cout << "������ ��� �����ϼ���: " << endl;
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
										cout << "������ �������� �����ϴ�." << endl;
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
										cout << "������ �������� �����ϴ�." << endl;
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
										cout << "������ �������� �����ϴ�." << endl;
										system("pause");

									}
									break;
								}


								AllItemInit();


					}
				}
		// ������ ��� �����Ѵ�. ��� �����ϰ� �ִ���, �������� ���� �����.
		else
		{
						int idx = iMenu - 1;

						// ������ ����� EQUIP�� �����´�.
						if (SetItem(m_vecInventory[idx]))
						{
							// �׳� ��ü�� �Ѿ�� �������� ���� ������ �ÿ��� ���⼭ �ɸ�.

							vector<CItem*>::iterator iter = m_vecInventory.begin() + idx;

							m_vecInventory.erase(iter); // �ش� ��Ҹ� �����ϰ� �����.
						}

						AllItemInit();
		}


	}


}


void CPlayer::ShowEquipItem()
{
	cout << "���� : ";
	if (m_tItem[EQ_WEAPON - 1])
	{
		cout << m_tItem[EQ_WEAPON - 1]->strName << endl;
	}
	else
		cout << "X" << endl;

	cout << "�� : ";
	if (m_tItem[EQ_ARMOR - 1])
	{
		cout << m_tItem[EQ_ARMOR - 1]->strName << endl;
	}
	else
		cout << "X" << endl;

	cout << "��ű� : ";
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
	// ���� ������Ʈ Ÿ���� �޾Ƽ� �̰��� EQUIP�� ȣȯ�Ͽ� ������ �Ϸ��� �Ѵ�.

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

	// �ش� ĭ�� ��� ���� �ִ��� �������� ������. 

	if (m_tItem[eq-1]) // ��� ������ ���� ���� �ִ� ���� �����Ϸ��� ��� �ٲ���Ѵ�.
	{
		CItem*	temp = m_tItem[eq-1];
		m_tItem[eq-1] = p;
		p = temp; 
		// temp�� ������ �ִ� ��� �κ��丮��..
		return false; // �׳� ���� �ȴ�.
	}

	else
	{
		// ��� ������ �����ִ� ������ �ʿ���. ������ erase�̿�.
		m_tItem[eq - 1] = p->Clone();
		return true;
	}
}

void CPlayer::ReturnItem(CItem *& p)
{
	// �� ��� �κ��丮�� �ִ´�.

	m_vecInventory.push_back(p->Clone());
	SAFE_DELETE(p);

	cout << "�������� �����߽��ϴ�." << endl;
	system("pause");
}

void CPlayer::ItemSell(vector<CItem*>::iterator iter)
{
	// �� �������� �Ǹ��Ѵ�.

	// �����ϰ� �ִ� �������� ���Ȱ� �κ��丮�� �ִ� �� �߿��� ������ �� �ֵ��� �Ѵ�.
	while (true)
	{
		cout << endl;
		cout << "������ << " << (*iter)->strName << " >>  ��(��) �Ǹ��Ͻðڽ��ϱ� ? " << endl << endl;
		SHOW2("1. �׷���");
		SHOW2("2. �ƴϴ�");
		
		int iMenu = Input<int>();

		if (iMenu <= 0 || iMenu > 2)
			continue;


		else if (iMenu == 2)
			return;

		break;
	}
	
	// �� ��� �Ǹ��Ѵ�.
	int iGold = (*iter)->GetiSell();
	AddGold(iGold);

	m_vecInventory.erase(iter);
	cout << "��� �Ǹ��߽��ϴ�" << endl;
	cout << iGold << " Gold �� ������ϴ�. " << endl;
	
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


void CPlayer::OutPutPointMenu() // ������ �ÿ� �ٷ� �ﵵ�� �Ѵ�.
{
	while (m_tPlayer.iPoint!= 0)
	{
		system("cls");
		cout << endl << endl;
		SHOW("���� ����Ʈ");
		cout << "1. ���ݷ� 1% (���ݷ��� �����մϴ�.)" << endl << endl;
		cout << "2. ���� 1% (������ �����մϴ�.)" << endl << endl;
		cout << "3. ȸ�Ƿ� 2% (ȸ�� Ȯ���� �����մϴ�.)" << endl << endl;
		cout << "4. ���� 2% (������ �������� ��带 ����ϴ�. / ���� : �нú� ȿ�� ���)" << endl << endl;
		cout << "5. ġ��Ÿ Ȯ�� 2% (ġ��Ÿ Ȯ���� �����մϴ�.)" << endl << endl;

		cout << "�߰� ���ݷ� : " << (m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent) << "%" << endl;
		cout << "�߰� ���� : " << (m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent) << "%" << endl;
		cout << "�߰� ȸ�Ƿ� : " << m_tPlayer.iAvoidPercent + m_tAllItem.iAvoidPercent << "%" << endl;
		cout << "�߰� ���� : " << m_tPlayer.iLuckyPercent + m_tAllItem.iLuckyPercent << "%" << endl;
		cout << "ũ��Ƽ�� Ȯ�� : " << m_tAllItem.iCriticalPercent + m_tPlayer.iCriticalPercent << "%" << endl << endl;
		
		cout << "������ �����ϼ��� : ";
		int iMenu = Input<int>();

		if (iMenu <= POINT_NONE || iMenu > POINT_CRITICAL)
			continue;

		// �� ���� �ִ� �������� Ȯ���ϰ� ������Ų��.

		switch (iMenu)
		{
		case POINT_ATTACK:
			m_tPlayer.iAttackPercent += 1;
			--m_tPlayer.iPoint;
			cout << "�߰� ���ݷ� + 1% " << endl;

			break;
		case POINT_ARMOR:
			m_tPlayer.iArmorPercent += 1;
			--m_tPlayer.iPoint;

			cout << "�߰� ���� + 1% " << endl;

			break;
		case POINT_AVOID:
			
			if (m_tPlayer.iAvoidPercent + 2 > AVOID_POINT_MAX)
			{
				cout << "�̹� �ִ��Դϴ�." << endl;
				break;
			}

			m_tPlayer.iAvoidPercent += 2;
			cout << "�߰� ȸ�Ƿ� + 2%" << endl;
			--m_tPlayer.iPoint;

			break;
		case POINT_LUCK:
			m_tPlayer.iLuckyPercent += 2;
			--m_tPlayer.iPoint;

			cout << "�߰� ���� + 2%" << endl;
			break;

		case POINT_CRITICAL:

			if (m_tPlayer.iCriticalPercent + 2 > CRITICAL_POINT_MAX)
			{
				cout << "�̹� �ִ��Դϴ�." << endl;
				break;
			}

			m_tPlayer.iCriticalPercent += 2;
			--m_tPlayer.iPoint;

			cout << "�߰� ġ��Ÿ Ȯ�� + 2%" << endl;
			
			break;
		}

		system("pause");

	}
	
}

void CPlayer::AddLevelUpInfo()
{
	//������ �ɷ�ġ�� �ø���.    Ư�� �ۼ�Ʈ ü�� ���� ���ݷ� ���� �� ����.


	// ü�� ����
	m_tInfo.iHPMax += m_tLevelUpInfo.iHP;
	m_tInfo.iMPMax += m_tLevelUpInfo.iMP;
	// �Լ��� ������ ü���� ä����.

	// ���ݷ�
	m_tInfo.iAttackMin += m_tLevelUpInfo.iAttackMin;
	m_tInfo.iAttackMax += m_tLevelUpInfo.iAttackMax;

	// ����
	m_tInfo.iArmorMin += m_tLevelUpInfo.iArmorMin;
	m_tInfo.iArmorMax += m_tLevelUpInfo.iArmorMax;

	// ���� ��� ȸ�� ��� ġ��Ÿ Ȯ�� �߿��� ġ��Ÿ�� ȸ�Ǵ� ��Ȳ�� ���� �÷��� ��.
	m_tPlayer.iAttackPercent += m_tLevelUpInfo.iAttackPercent;
	m_tPlayer.iArmorPercent += m_tLevelUpInfo.iArmorPercent;
	m_tPlayer.iLuckyPercent += m_tLevelUpInfo.iLuckyPercent;

	if ((m_tPlayer.iAvoidPercent + m_tLevelUpInfo.iAvoidPercent) > AVOID_POINT_MAX)
	{
		++m_tPlayer.iPoint;
		cout << "ȸ�Ƿ��� LV_MAX���� �����Ͽ� �߰� ������ ����ϴ�." << endl;
		system("pause");
	}
	// ����� ���� �ƽ����� ä��� �ν����� ��� ��Ȳ������ �⺻�� �ö󰡴� %���� �� ���Ƽ� ���� �� ����.

	else
		m_tPlayer.iAvoidPercent += m_tLevelUpInfo.iAvoidPercent;

	if ((m_tPlayer.iCriticalPercent + m_tLevelUpInfo.iCriticalPercent) > CRITICAL_POINT_MAX)
	{
		++m_tPlayer.iPoint;
		cout << "ġ��Ÿ Ȯ���� LV_MAX���� �����Ͽ� �߰� ������ ����ϴ�." << endl;
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
		
		// �ΰ� ���� ������ ���� ������ ����. -> ��ſ� �߰� ü�� / ���� ���� �־�����.


	case JOB_GIANT:
		// �߰� ü�� 40
		cout << "<< �нú� ���� >> " << endl;
		cout << "ü���� 40 ��ŭ �����մϴ�. " << endl;
		m_tInfo.iHPMax += 40;
		break;


	case JOB_ELF:
		// �߰� ���� 40
		cout << "<< �нú� ���� >> " << endl;
		cout << "������ 40 ��ŭ �����մϴ�. " << endl;
		m_tInfo.iMPMax += 40;

		break;

	case JOB_DRAGON: // ���ݷ� 2 ���� 2
		cout << "<< �нú� Dragon >> " << endl;
		cout << "���ݷ°� ������ 2 ��ŭ �����մϴ�. " << endl;
		m_tInfo.iArmorMax += 2;
		m_tInfo.iArmorMin += 2;
		m_tInfo.iAttackMax += 2;
		m_tInfo.iAttackMin += 2;
		break;

	case JOB_ORC: // ���� 4
		cout << "<< �нú� �β��� �Ǻ� >> " << endl;
		cout << "������ 4 ��ŭ �����մϴ�. " << endl;
		m_tInfo.iArmorMax += 4;
		m_tInfo.iArmorMin += 4;

		break;

	case JOB_DEMON: // ���ݷ� 4

		cout << "<< �нú� �Ǹ� >> " << endl;
		cout << "���ݷ��� 4 ��ŭ �����մϴ�. " << endl;
		m_tInfo.iAttackMax +=4;
		m_tInfo.iAttackMin +=4;
		break;
	}

	system("pause");
}

void CPlayer::AddLevelUpPrint()
{
	// �츮�� �÷��̾�� ��� ������ �ɷ�ġ�� ����ߴ��� �����ַ��� �Ѵ�.

	// ü�� ����
	cout << "HP : " << m_tLevelUpInfo.iHP << "+" << endl;
	cout << "MP : " << m_tLevelUpInfo.iMP << "+" << endl;
	// �Լ��� ������ ü���� ä����.

	// ���ݷ�
	cout << "�ּ� ���ݷ� : " << m_tLevelUpInfo.iAttackMin << "+" << endl;
	cout << "�ִ� ���ݷ� : " << m_tLevelUpInfo.iAttackMax << "+" << endl;

	// ����
	cout << "�ּ� ���� : " << m_tLevelUpInfo.iArmorMin << "+" << endl;
	cout << "�ִ� ���� : " << m_tLevelUpInfo.iArmorMax << "+" << endl;

	// ���� ��� ȸ�� ��
		cout << "NOW ���� : " << m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent<< "%" << endl;
		cout << "NOW ȸ�Ƿ� : " << m_tPlayer.iAvoidPercent + m_tAllItem.iAvoidPercent << "%" << endl;
		cout << "NOW ���ݷ� : " << m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent << "%" << endl;
		cout << "NOW ���� : " << m_tPlayer.iLuckyPercent + m_tAllItem.iLuckyPercent << "%" << endl;
		cout << "NOW ġ��Ÿ Ȯ�� : " << m_tPlayer.iCriticalPercent + m_tAllItem.iCriticalPercent << "%" << endl;

		system("pause");
}

void CPlayer::Run()
{

}

bool CPlayer::Init()
{

	AllItemInit(); // �ϴ��� 0���� ����

	Ignore();
	cout << "����� �̸��� �Է��ϼ��� : ";
	char* strname = new char[NAME_SIZE_MAX];
	cin.getline(strname, NAME_SIZE_MAX - 1, '\n');

	int iMenu = 2;

	while (iMenu == 2)
	{
		cout << "����� �̸��� �½��ϱ� ? " << strname << endl;
		SHOW2("1. �׷���");
		SHOW2("2. �ƴϴ�");
		iMenu = Input<int>();

		if (iMenu < 1 || iMenu > 2)
			continue;

		else if (iMenu == 2)
		{
			system("cls");
			cout << "�ٽ� �Է��ϼ��� : ";
			cin.ignore(1024, '\n');
			cin.getline(strname, NAME_SIZE_MAX - 1, '\n');

			system("pause");
			continue;
		}

		break;
	}
	strcpy_s(strName, strname);
	SAFE_DELETE_ARRAY(strname);

	// ���� �����ؼ��� ����� �� ó��.
	int iJob;
	while (true)
	{
		iJob = OutPutMenu();


		 if (iJob == JOB_PLUS)
		{
			JobPlusInfo(); // �� ����
		}

		else if (iJob != JOB_NONE)
		{
			eJob = (JOB)iJob;
			break;
		}
	}

	// �� ������ ������ �ɷ�ġ�� �����Ѵ�.
	SetLevelUpInfo();


	return true;
}

void CPlayer::Magic()
{
	// ������ �Ҹ��Ͽ� ü���� ȸ���Ѵ�.  ������ ü�¤���.
	int iMP = m_tInfo.iLevel * 50;

	int iMenu;

	while (true)
	{
		cout << "������ ü�� ����(+" << iMP << ")�� �����ðڽ��ϱ� ? (" << iMP << " MP �Ҹ�)" << endl;
		cout << "1. �׷���. " << endl;
		cout << "2. �ƴϴ�." << endl;
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
			cout << "MP �� �����մϴ�. " << endl;
			break;
		}

		cout << "ü���� " << iMP << " ȸ���߽��ϴ�. " << endl;
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
	SHOW("�ΰ�");			 
	SHOW2("1. ����");
	SHOW2("2. �ü�");
	SHOW2("3. ������");
	SHOW2("4. ����");
	SHOW2("5. �˰�");

	SHOW("����");

	SHOW2("6. ���̾�Ʈ");
	SHOW2("7. ����");
	SHOW2("8. �Ǹ�");
	SHOW2("9. �巡��");
	SHOW2("10. ��ũ");
	SHOW2("11. ��� ���� �� ���� ���");
	cout << "������ �����ϼ��� : ";
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

		// �ΰ� ���� ������ ���� ������ ����. -> ��ſ� �߰� ü�� / ���� ���� �־�����.
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
	cout << "======================== �÷��̾� ========================" << endl;
	cout << "�̸� : " << strName << "   Lv : " << m_tInfo.iLevel;
		
		
	cout << "\t���� : ";

	switch (eJob)
	{
	case JOB_KNIGHT:
		cout << "����" << endl;
		break;
	case JOB_ARCHER:
		cout << "�ü�" << endl;
		break;
	case JOB_WIZARD:
		cout << "������" << endl;
		break;
	case JOB_THIEF:
		cout << "����" << endl;
		break;
	case JOB_SWORD:
		cout << "�˰�" << endl;
		break;
	case JOB_ELF:
		cout << "����" << endl;
		break;

	case JOB_DEMON:
		cout << "�Ǹ�" << endl;
		break;

	case JOB_ORC:
		cout << "��ũ" << endl;
		break;

	case JOB_DRAGON:
		cout << "�巡��" << endl;
		break;

	case JOB_GIANT:
		cout << "����" << endl;
		break;

	}

	cout << endl;
		cout << "HP : " << m_tInfo.iHP << " + " << m_tAllItem.iHP;
			
			
		cout << " / " << m_tInfo.iHPMax << " + " << m_tAllItem.iHPMax << endl;

		cout << "MP : " << m_tInfo.iMP << " + " << m_tAllItem.iMP <<
			" / " << m_tInfo.iMPMax << " + " << m_tAllItem.iMPMax << endl << endl;

		cout << "�ּ� ���ݷ� : " << (int)((m_tInfo.iAttackMin + m_tAllItem.iAttackMin) * GetAttackPercent())

			<< " ~ " << (int)((m_tInfo.iAttackMax + m_tAllItem.iAttackMax)  * GetAttackPercent()) << endl;

		cout << "�ּ� ���� : " << (int)((m_tInfo.iArmorMin + m_tAllItem.iArmorMin) * GetArmorPercent()) << " ~ "
			<< (int)((m_tInfo.iArmorMax + m_tAllItem.iArmorMax)  * GetArmorPercent()) << endl << endl;

		cout << "�߰� ���ݷ� : " << (m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent) << "%  (" <<
			((m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent) / 100) * (m_tInfo.iAttackMin + m_tAllItem.iAttackMin) << " ~ "

			<< ((m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent) / 100) * (m_tInfo.iAttackMax + m_tAllItem.iAttackMax) << ")" << endl;

		cout << "�߰� ���� : " << (m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent) << "%  ("

			<< ((m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent) / 100) * (m_tInfo.iArmorMin + m_tAllItem.iArmorMin) << " ~ "
			<< ((m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent) / 100) * (m_tInfo.iArmorMax + m_tAllItem.iArmorMax) << ")" << endl;

		cout << "�߰� ȸ�Ƿ� : " << GetAvoidPercent() << "%" << endl;

		

		cout << "�߰� ���� : " << m_tPlayer.iLuckyPercent + m_tAllItem.iLuckyPercent << "%" << endl << endl;

		cout << "ũ��Ƽ�� Ȯ�� : " << GetCriticalPercent() << "%" << endl << endl;
		
		if (m_tInfo.iLevel != MAX_LEVEL)
		{
			cout << "Exp : " << m_tInfo.iExp << " / " << LevelUpTable[m_tInfo.iLevel - 1] << endl;
		}
		else
		{
			cout << "Exp : 99.9 % " << endl;
		}


		cout << "������ : " << m_tPlayer.iGold << endl << endl;

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


	// �������� ��� �κ��丮�� �־ �Բ� �����ϵ��� �Ѵ�.

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


	// �κ��丮 ������ �����Ѵ�.

	int iSize = m_vecInventory.size();

	// ��� ������ �����Ѵ�.
	file->Write(&iSize, 4);

	// ��� �����Ѵ�.
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
	// ��� ������ �д´�.

	// ��� ���� �� ���� �����ʹ� ������ �Ѵ�. �κ��丮�� ��������

	Safe_Delete_VecList(m_vecInventory);
	SAFE_DELETE(m_tItem[EQ_RUBY - 1]);
	SAFE_DELETE(m_tItem[EQ_WEAPON - 1]);
	SAFE_DELETE(m_tItem[EQ_ARMOR - 1]);

	file->Read(&iSize, 4);


	// ��� �д´�.
	for (int i = 0; i < iSize; ++i)
	{
		CItem*   pItem = new CItem;
		pItem->Load(file);
		m_vecInventory.push_back(pItem);
	}
}

