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
	PLAYER				m_tPlayer;        // 플레이어 구조체 정보 퍼센트 & 포인트 , 골드 ...
	LEVELUPINFO			m_tLevelUpInfo;   // 레벨업 정보
	JOB					eJob; 
	class CItem*		m_tItem[EQ_RUBY]; // 플레이어의 장착 장비 배열.
	ITEMINFO			m_tAllItem;		  // 플레이어가 장착한 아이템의 정보를 통합하여 Render와 전투에 직접적으로 이용한다. 
	vector<CItem*>		m_vecInventory; 
	vector<long long>   LevelUpTable; // 오브젝트 매니저에 있는 것을 가져다쓰는 데이터 테이블


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
		// 이 포인트를 이용하여 추가 스탯을 찍을 수 있음.
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

	// 이 4가지 값은 총 계산을 의미한다. 100(기본) % + (플레이어 능력치) 1% + (아이템) 3 % -> 104% 이런 식.
	// 아이템이 없으면.... 생성자에 의해서 기본 값이 0으로 셋팅되어 그냥 계산하면 된다.

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

	int GetAvoidPercent()  // 이건 사실상 수치가 확률 그 자체
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
		if (k > 0) // k는 양수
		{
			if (m_tInfo.iMP + k <= m_tInfo.iMPMax)
				m_tInfo.iMP += k;

			else
			{
				int x = m_tInfo.iMPMax - m_tInfo.iMP;
				m_tInfo.iMP += x; // 플레이어 mp = max;
				k -= x;

				m_tAllItem.iMP += k;

				m_tAllItem.iMP = m_tAllItem.iMP <= m_tAllItem.iMPMax ? m_tAllItem.iMP : m_tAllItem.iMPMax;
			}
		}

				else // k 는 음수 체력 포션의 경우에는 마나가 부족할 시에 미리 막아서 처리 해야한다.
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


	int GetPlayerAttack() // 전투부분에서 % 연산을 해주도록 한다.
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
			// k 만큼을 회복하는데 아이템을 통한 추가 체력도 생각을 해야한다.
		{
			if (m_tInfo.iHP + k <= m_tInfo.iHPMax) // hp통으만 해결 가능.
				m_tInfo.iHP += k;

			else  // hp통 아이템 까지 모두 처리해야 되는 경우.
			{
				int x = m_tInfo.iHPMax - m_tInfo.iHP; // 일단 채워야하 하는 피통.
				m_tInfo.iHP += x;
				k -= x; // 남은 체력은 아이템에 부어넣음 ( 이러고도 해결이 안 되면 버리는 값. )

				m_tAllItem.iHP += k;

				m_tAllItem.iHP = m_tAllItem.iHP <= m_tAllItem.iHPMax ? m_tAllItem.iHP : m_tAllItem.iHPMax;
			}
		}
		// k는 음수 따로 처리를 하지 않으면 k값을 뺴고서 보여주는데 -10 + 50 이런 식으로 보여지게 됨...
		else
		{
			// 일단은 보기 좋게 장비의 HP부터 감소 시킨다.

			if (m_tAllItem.iHP + k >= 0)
			{
				m_tAllItem.iHP += k;
			}

			else // 아이템의 체력을 감소시키고도 부족한 경우
			{
				int x = m_tAllItem.iHP + k;

				m_tAllItem.iHP = 0;
				// 남은 값은 체력에서 까줌.

				m_tInfo.iHP += x;
				// 이후에 체력이 모든 것을 다 받아들여서 음수 값이 되거나 0이 되면 사망처리가 됨.
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

