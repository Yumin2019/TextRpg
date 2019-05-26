#include "CMonster.h"
#include "CFilestream.h"
#include "CStageManager.h"

CMonster::CMonster(CMonster & p)	:
	CCharacter(p)
{
	iGoldMin = p.iGoldMin;
	iGoldMax = p.iGoldMax;
	iStageNumber = p.iStageNumber;
}

CMonster::CMonster()	:
	CCharacter()
{
	iGoldMin = 0;
	iGoldMax = 0;
	iStageNumber = 0;
}


CMonster::~CMonster()
{
}

void CMonster::Run()
{
}

bool CMonster::Init()
{
	return true;
}



void CMonster::Render()
{
	cout << "======================== ���� ========================" << endl;

	cout << "�̸� : " << strName << "   Lv : " << m_tInfo.iLevel << endl << endl;

	cout << "HP : " << m_tInfo.iHP << " / " << m_tInfo.iHPMax << endl;
		 cout << "MP : " << m_tInfo.iMP << " / " << m_tInfo.iMPMax << endl;

	cout << "�ּ� ���ݷ� : " << m_tInfo.iAttackMin << " ~ " << m_tInfo.iAttackMax << endl;
	cout <<	"�ּ� ���� : " << m_tInfo.iArmorMin << " ~ " << m_tInfo.iArmorMax << endl;

	cout << "ȹ�� Exp : " << m_tInfo.iExp << endl;
	cout << "������ : " << iGoldMin << " ~ " << iGoldMax << endl;
}

CMonster * CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::Save(CFilestream * file)
{
	CCharacter::Save(file);

	file->Write(&iGoldMin, 4);
	file->Write(&iGoldMax, 4);
	file->Write(&iStageNumber, 4);

}

void CMonster::Load(CFilestream * file)
{
	CCharacter::Load(file);

	file->Read(&iGoldMin, 4);
	file->Read(&iGoldMax, 4);
	file->Read(&iStageNumber, 4);


}
