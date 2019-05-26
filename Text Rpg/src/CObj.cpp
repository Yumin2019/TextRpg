#include "CObj.h"
#include "CFilestream.h"


CObj::CObj(const CObj & p)
{
	strcpy_s(strName, NAME_SIZE_MAX - 1, p.strName);
	OT_type = p.OT_type;
}

CObj::CObj()
{
	memset(strName, 0, NAME_SIZE_MAX);
	OT_type = OT_NONE;
}


CObj::~CObj()
{
}

void CObj::Save(CFilestream * file)
{
	if (!file->GetOpen())
		return;

	if (file->GetFileMode() != FM_WRITE)
		return;

	int iSize = strlen(strName);

	// �̸��� ����� ����Ѵ�.

	file->Write(&iSize, 4);

	// �̸��� ����Ѵ�.

	file->Write(strName, iSize);

	// ������Ʈ Ÿ���� ����Ѵ�.

	file->Write(&OT_type, 4);
	
}

void CObj::Load(CFilestream * file)
{
	if (!file->GetOpen())
		return;

	if (file->GetFileMode() != FM_READ)
		return;

	int iSize;

	// �̸��� ����� �о�´�.

	file->Read(&iSize, 4);

	// �̸��� �д´�.

	// ó���� ������ �̸��� ���, �ڿ����� �̸��� ����... NULL
	memset(strName, 0, NAME_SIZE_MAX);
	file->Read(strName, iSize);

	// ������Ʈ Ÿ���� �д´�.

	file->Read(&OT_type, 4);

}



