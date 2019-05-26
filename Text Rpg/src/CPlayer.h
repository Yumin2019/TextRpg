#pragma once
#include "CCharacter.h"


class CPlayer :
	public CCharacter
{
public:
	CPlayer(const CPlayer& p);
	CPlayer();
	~CPlayer();



private:
	PLAYER				m_tPlayer;        // �÷��̾� ����ü ���� �ۼ�Ʈ & ����Ʈ , ��� ...
	LEVELUPINFO			m_tLevelUpInfo;   // ������ ����
	JOB					eJob; 
	class CItem*		m_tItem[EQ_RUBY]; // �÷��̾��� ���� ��� �迭.
	ITEMINFO			m_tAllItem;		  // �÷��̾ ������ �������� ������ �����Ͽ� Render�� ������ ���������� �̿��Ѵ�. 
	vector<CItem*>		m_vecInventory; 
	vector<long long>   LevelUpTable; // ������Ʈ �Ŵ����� �ִ� ���� �����پ��� ������ ���̺�


	friend class CStage;

public:

	void LoadLevelUpTable();
	void ShowEquipRender();
	void JobPlusInfo();
	void AddItem(CItem* p);
	void InventoryItemList();
	void ShowEquipItem();
	bool SetItem(CItem* & p);
	void ReturnItem(CItem* & p);
	void ItemSell(vector<CItem*>::iterator iter);

	friend class CObjManager;
	friend class CStageManager;

public:


	void SellStore();
	void Hospital();
	void LevelInfoRender(LEVELUPINFO Info);
	void StartInfoRender();
	void AllItemInit();
	bool CheckEquip();

	void SetPlayer(double iAttack, double iArmor, double iAvoid, double iLuck, int iCritical, int iMoney);
	
	bool LvCheck()
	{
		if (m_tInfo.iLevel != MAX_LEVEL)
		{
			return m_tInfo.iExp >= LevelUpTable[m_tInfo.iLevel-1] ? true : false;
		}
		else
		{
			return false;
		}
	}
	void LvUp()
	{
		++m_tInfo.iLevel;
		++m_tPlayer.iPoint;
		// �� ����Ʈ�� �̿��Ͽ� �߰� ������ ���� �� ����.
	}

	
	void OutPutPointMenu();

	void SetGold(int K)
	{
		m_tPlayer.iGold = K;
	}

	void AddLevelUpInfo();
	void AddLevelUpPrint();

	long long GetLevelUpExp()
	{
		return	LevelUpTable[m_tInfo.iLevel - 1];

	}
	PLAYER GetPlayer()
	{
		return m_tPlayer;
	}

	long long GetExp()
	{
		return m_tInfo.iExp;
	}

	int GetGold()
	{
		return m_tPlayer.iGold;
	}
public:
	virtual void Run();
	virtual bool Init();
	void Magic();


	int OutPutMenu();
	LEVELUPINFO GetLevelUpInfo()
	{
		return m_tLevelUpInfo;
	}

	JOB GetJob()
	{
		return eJob;
	}

	void SetLevelUpInfo();

	// �� 4���� ���� �� ����� �ǹ��Ѵ�. 100(�⺻) % + (�÷��̾� �ɷ�ġ) 1% + (������) 3 % -> 104% �̷� ��.
	// �������� ������.... �����ڿ� ���ؼ� �⺻ ���� 0���� ���õǾ� �׳� ����ϸ� �ȴ�.

	int GetCriticalPercent()
	{
		return (m_tPlayer.iCriticalPercent + m_tAllItem.iCriticalPercent) > 100 ? 100 : (m_tPlayer.iCriticalPercent + m_tAllItem.iCriticalPercent);
	}
	double GetAttackPercent()
	{
		return (m_tPlayer.iAttackPercent + m_tAllItem.iAttackPercent +  100)/100;
	}

	double GetArmorPercent()
	{
		return (m_tPlayer.iArmorPercent + m_tAllItem.iArmorPercent + 100) / 100;
	}

	double GetLuckyPercent() 
	{
		return (m_tPlayer.iLuckyPercent + m_tAllItem.iLuckyPercent + 100) / 100;
	}

	int GetAvoidPercent()  // �̰� ��ǻ� ��ġ�� Ȯ�� �� ��ü
	{
		return (m_tPlayer.iAvoidPercent + m_tAllItem.iAvoidPercent) > 100 ? 100 : (m_tPlayer.iAvoidPercent + m_tAllItem.iAvoidPercent);
	}
	
	bool Stop()
	{
		return (m_tInfo.iHP == m_tInfo.iHPMax && m_tInfo.iMP == m_tInfo.iMPMax);
	}
	void AddGold(int k)
	{
		m_tPlayer.iGold += k;
		m_tPlayer.iGold = m_tPlayer.iGold <= 0 ? 0 : m_tPlayer.iGold;

	}

	void AddExp(long long k)
	{
		m_tInfo.iExp += k;
		m_tInfo.iExp = m_tInfo.iExp <= 0 ? 0 : m_tInfo.iExp;
	}
	
	void PlayerAddMP(int k)
	{
		if (k > 0) // k�� ���
		{
			if (m_tInfo.iMP + k <= m_tInfo.iMPMax)
				m_tInfo.iMP += k;

			else
			{
				int x = m_tInfo.iMPMax - m_tInfo.iMP;
				m_tInfo.iMP += x; // �÷��̾� mp = max;
				k -= x;

				m_tAllItem.iMP += k;

				m_tAllItem.iMP = m_tAllItem.iMP <= m_tAllItem.iMPMax ? m_tAllItem.iMP : m_tAllItem.iMPMax;
			}
		}

				else // k �� ���� ü�� ������ ��쿡�� ������ ������ �ÿ� �̸� ���Ƽ� ó�� �ؾ��Ѵ�.
				{

					if (m_tAllItem.iMP + k >= 0)
					{
						m_tAllItem.iMP += k;
					}

					else 
					{
						int x = m_tAllItem.iMP + k;

						m_tAllItem.iMP = 0;

						m_tInfo.iMP += x;
					}

				}
	}
	void Render();

	CPlayer* Clone();
	virtual void Save(class CFilestream* file);
	virtual void Load(class CFilestream* file);


	int GetPlayerAttack() // �����κп��� % ������ ���ֵ��� �Ѵ�.
	{

		return 		rand() % (1 + m_tInfo.iAttackMax + m_tAllItem.iAttackMax - m_tInfo.iAttackMin - m_tAllItem.iAttackMin) + (m_tInfo.iAttackMin + m_tAllItem.iAttackMin);
	}


	int GetPlayerArmor()
	{
		return 		rand() % (1 + m_tInfo.iArmorMax + m_tAllItem.iArmorMax - m_tInfo.iArmorMin - m_tAllItem.iArmorMin) + (m_tInfo.iArmorMin + m_tAllItem.iArmorMin);
	}

	bool PlayerDieCheck()
	{

		return (m_tInfo.iHP + m_tAllItem.iHP) <= 0 ? true : false;
	}

	void PlayerAddHP(int k) 
	{

		if (k > 0)
			// k ��ŭ�� ȸ���ϴµ� �������� ���� �߰� ü�µ� ������ �ؾ��Ѵ�.
		{
			if (m_tInfo.iHP + k <= m_tInfo.iHPMax) // hp������ �ذ� ����.
				m_tInfo.iHP += k;

			else  // hp�� ������ ���� ��� ó���ؾ� �Ǵ� ���.
			{
				int x = m_tInfo.iHPMax - m_tInfo.iHP; // �ϴ� ä������ �ϴ� ����.
				m_tInfo.iHP += x;
				k -= x; // ���� ü���� �����ۿ� �ξ���� ( �̷��� �ذ��� �� �Ǹ� ������ ��. )

				m_tAllItem.iHP += k;

				m_tAllItem.iHP = m_tAllItem.iHP <= m_tAllItem.iHPMax ? m_tAllItem.iHP : m_tAllItem.iHPMax;
			}
		}
		// k�� ���� ���� ó���� ���� ������ k���� ���� �����ִµ� -10 + 50 �̷� ������ �������� ��...
		else
		{
			// �ϴ��� ���� ���� ����� HP���� ���� ��Ų��.

			if (m_tAllItem.iHP + k >= 0)
			{
				m_tAllItem.iHP += k;
			}

			else // �������� ü���� ���ҽ�Ű�� ������ ���
			{
				int x = m_tAllItem.iHP + k;

				m_tAllItem.iHP = 0;
				// ���� ���� ü�¿��� ����.

				m_tInfo.iHP += x;
				// ���Ŀ� ü���� ��� ���� �� �޾Ƶ鿩�� ���� ���� �ǰų� 0�� �Ǹ� ���ó���� ��.
			}
		}
	}

	void PlayerFullHP()
	{
		m_tInfo.iHP = m_tInfo.iHPMax;
		m_tInfo.iMP = m_tInfo.iMPMax;

		m_tAllItem.iHP = m_tAllItem.iHPMax;
		m_tAllItem.iMP = m_tAllItem.iMPMax;
	}

	bool ItemUse()
	{

		if (m_tItem[EQ_ARMOR - 1])
			return true;

			if (m_tItem[EQ_WEAPON - 1])
				return true;

				if (m_tItem[EQ_RUBY - 1])
					return true;

				return false;
	}
};

