#include "CCharacter.h"
#include "CFilestream.h"

CCharacter::CCharacter(const CCharacter & p)	:
	CObj(p)
{
	m_tInfo = p.m_tInfo;
}

CCharacter::CCharacter() :
	CObj()
{
	SetInfo(0, 0, 0, 0, 0, 0, 0, 1);
}


CCharacter::~CCharacter()
{
}

void CCharacter::SetInfo(int iHP, int iMP, int iAttackMin, int iAttackMax, int iArmorMin, int iArmorMax, long long iExp, int iLevel)
{
	m_tInfo.iHP = iHP;
	m_tInfo.iHPMax = iHP;
	m_tInfo.iMP = iMP;
	m_tInfo.iMPMax = iMP;
	m_tInfo.iAttackMax = iAttackMax;
	m_tInfo.iAttackMin = iAttackMin;
	m_tInfo.iArmorMin = iArmorMin;
	m_tInfo.iArmorMax = iArmorMax;
	m_tInfo.iExp = iExp;
	m_tInfo.iLevel = iLevel;
}

void CCharacter::Run()
{
}

bool CCharacter::Init()
{
	return true;
}

void CCharacter::Save(CFilestream * file)
{
	CObj::Save(file);

	file->Write(&m_tInfo, sizeof(m_tInfo));
	
}


void CCharacter::Load(CFilestream * file)
{
	CObj::Load(file);

	file->Read(&m_tInfo, sizeof(m_tInfo));

}

CObj * CCharacter::Clone()
{
	return NULL;
}
