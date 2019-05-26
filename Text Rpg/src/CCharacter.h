#pragma once
#include "CObj.h"
class CCharacter :
	public CObj
{
public:
	CCharacter(const CCharacter& p);
	CCharacter();
	~CCharacter();

protected:
	// ���Ϳ� �÷��̾ ���� �� ����
	INFO	m_tInfo;

public:

	void CCharacter::SetInfo(int iHP, int iMP, int iAttackMin, int iAttackMax, int iArmorMin, int iArmorMax, long long iExp, int iLevel);

	int GetAttack()
	{
		return rand() % (1 + m_tInfo.iAttackMax - m_tInfo.iAttackMin) + m_tInfo.iAttackMin;
	}
	
	int GetArmor()
	{
		return rand() % (1 + m_tInfo.iArmorMax - m_tInfo.iArmorMin) + m_tInfo.iArmorMin;
	}

	long long GetExp()
	{
		return m_tInfo.iExp;
	}

	bool DieCheck()
	{
		return m_tInfo.iHP <= 0 ? true : false;
	}

	void AddHP(int k) // ������ �߰� ü���� ������� �����Ƿ� ���� ���� �Լ��� �Ǿ���. �÷��̾��� ��� Player Add HP
	{
		m_tInfo.iHP += k;

		m_tInfo.iHP = m_tInfo.iHP <= m_tInfo.iHPMax ? m_tInfo.iHP : m_tInfo.iHPMax;
	}

	void FullHP()
	{
		m_tInfo.iHP = m_tInfo.iHPMax;
		m_tInfo.iMP = m_tInfo.iMPMax;
	}

	int GetLevel()
	{
		return m_tInfo.iLevel;
	}
public:
	virtual void Run();
	virtual bool Init();
	virtual void Save(class CFilestream* file);
	virtual void Load(class CFilestream* file);

	virtual CObj* Clone();
};

