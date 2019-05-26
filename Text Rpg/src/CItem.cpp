#include "CItem.h"


CItem::CItem() :
		CObj()
{
	memset(strDesc, 0, 256);
	m_tItemInfo = {}; 
}

CItem::CItem(const CItem & p)	:
		CObj(p)
{
	strcpy_s(strDesc, 255, p.strDesc);
	m_tItemInfo = p.m_tItemInfo;
}


CItem::~CItem()
{
}

void CItem::Save(CFilestream * file) 
{
	CObj::Save(file);


	file->Write(&m_tItemInfo, sizeof(m_tItemInfo));

	int iSize = strlen(strDesc);
	// 길이를 저장한다.

	file->Write(&iSize, 4);


	// 길이 만큼 적는다.
	file->Write(strDesc, iSize);


}

void CItem::Load(CFilestream * file)
{
	CObj::Load(file);

	file->Read(&m_tItemInfo, sizeof(m_tItemInfo));
	memset(strDesc, 0, 256);

	int iSize;
	// 길이를 읽어온다.

	file->Read(&iSize, 4);


	// 길이 만큼 읽는다
	file->Read(strDesc, iSize);

}

void CItem::Run()
{
}

bool CItem::Init()
{
	return true;
}

void CItem::SetItemInfo(int HP, int MP, int iAttackMin, int iAttackMax, int iArmorMin, int iArmorMax,
	int iCriticalPercent, double iAttackPercent, double iArmorPercent, double iAvoidPercent, double iLuckyPercent, int iPrice, int iSell)
{
	m_tItemInfo.iHPMax = HP;
	m_tItemInfo.iMPMax = MP;
	m_tItemInfo.iHP = HP;
	m_tItemInfo.iMP = MP;

	m_tItemInfo.iAttackMin = iAttackMin;
	m_tItemInfo.iAttackMax = iAttackMax;
	m_tItemInfo.iArmorMin = iArmorMin;
	m_tItemInfo.iArmorMax = iArmorMax;

	m_tItemInfo.iCriticalPercent = iCriticalPercent;
	m_tItemInfo.iAttackPercent = iAttackPercent;
	m_tItemInfo.iArmorPercent = iArmorPercent;
	m_tItemInfo.iAvoidPercent = iAvoidPercent;
	m_tItemInfo.iLuckyPercent = iLuckyPercent;

	m_tItemInfo.iPrice = iPrice;
	m_tItemInfo.iSell = iSell;

}

void CItem::Render()
{

	cout << strName;
	// 아이템 타입.

	switch (OT_type)
	{
	case OT_ARMOR:
		cout << "(방어구)\t";
		break;

	case OT_WEAPON:
		cout << "(무기)\t";
		break;

	case OT_RUBY:
		cout << "(장신구)\t"; 
		break;
	}
	
	cout << endl << endl <<  "설명 : " << strDesc << endl;

	cout << "HP : " << m_tItemInfo.iHPMax << "+" << "                MP: " << m_tItemInfo.iMPMax << " + " << endl;
	cout << "공격력 : " << m_tItemInfo.iAttackMin << " ~ " << m_tItemInfo.iAttackMax << "        방어력 : " << m_tItemInfo.iArmorMin << " ~ " << m_tItemInfo.iArmorMax << endl;
	cout << "크리티컬 확률 : " << m_tItemInfo.iCriticalPercent << "%\t추가 공격력 : " << m_tItemInfo.iAttackPercent << "%" << endl;
	cout << "추가 방어력 : " << m_tItemInfo.iArmorPercent << "%\t추가 회피력 : " << m_tItemInfo.iAvoidPercent << "%" << endl;
	cout << "추가 행운력 : " << m_tItemInfo.iLuckyPercent << "%"<< endl;
	cout << "가격 : " << m_tItemInfo.iPrice << "\t\t상점매입가 : " << m_tItemInfo.iSell << endl;
}

CItem * CItem::Clone()
{
	return new CItem(*this);
}
