#pragma once
#include "CObj.h"
#include "CFilestream.h"
#include "value.h"
class CItem :
	public CObj
{
public:
	CItem();
	CItem(const CItem& p);
	 ~CItem();


private:
	ITEMINFO	m_tItemInfo;
	char		strDesc[256];
	// object type을 통해서 equip를 구할 수 있으므로 선언하지 않는다.
public:
	
	void Save(CFilestream * file);
	void Load(CFilestream * file);

	void Run();
	bool Init();

	int GetiSell()
	{
		return m_tItemInfo.iSell;
	}

	int GetiPrice()
	{
		return m_tItemInfo.iPrice;
	}

	ITEMINFO GetItemInfo()
	{
		return m_tItemInfo;
	}


	void SetItemInfo(int HP, int MP, int iAttackMin, int iAttackMax, int iArmorMin, int iArmorMax, int iCriticalPercent, 
		double iAttackPercent, double iArmorPercent, double iAvoidPercent, double iLuckyPercent, int iPrice, int iSell);

	void SetDesc(const char* pDesc)
	{
		strcpy_s(strDesc, 255, pDesc);
	}

	void Render();
	class CItem* Clone();
};

