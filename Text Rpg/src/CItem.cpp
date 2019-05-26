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
	// ���̸� �����Ѵ�.

	file->Write(&iSize, 4);


	// ���� ��ŭ ���´�.
	file->Write(strDesc, iSize);


}

void CItem::Load(CFilestream * file)
{
	CObj::Load(file);

	file->Read(&m_tItemInfo, sizeof(m_tItemInfo));
	memset(strDesc, 0, 256);

	int iSize;
	// ���̸� �о�´�.

	file->Read(&iSize, 4);


	// ���� ��ŭ �д´�
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
	// ������ Ÿ��.

	switch (OT_type)
	{
	case OT_ARMOR:
		cout << "(��)\t";
		break;

	case OT_WEAPON:
		cout << "(����)\t";
		break;

	case OT_RUBY:
		cout << "(��ű�)\t"; 
		break;
	}
	
	cout << endl << endl <<  "���� : " << strDesc << endl;

	cout << "HP : " << m_tItemInfo.iHPMax << "+" << "                MP: " << m_tItemInfo.iMPMax << " + " << endl;
	cout << "���ݷ� : " << m_tItemInfo.iAttackMin << " ~ " << m_tItemInfo.iAttackMax << "        ���� : " << m_tItemInfo.iArmorMin << " ~ " << m_tItemInfo.iArmorMax << endl;
	cout << "ũ��Ƽ�� Ȯ�� : " << m_tItemInfo.iCriticalPercent << "%\t�߰� ���ݷ� : " << m_tItemInfo.iAttackPercent << "%" << endl;
	cout << "�߰� ���� : " << m_tItemInfo.iArmorPercent << "%\t�߰� ȸ�Ƿ� : " << m_tItemInfo.iAvoidPercent << "%" << endl;
	cout << "�߰� ���� : " << m_tItemInfo.iLuckyPercent << "%"<< endl;
	cout << "���� : " << m_tItemInfo.iPrice << "\t\t�������԰� : " << m_tItemInfo.iSell << endl;
}

CItem * CItem::Clone()
{
	return new CItem(*this);
}
