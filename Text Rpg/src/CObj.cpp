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

	// 이름의 사이즈를 기록한다.

	file->Write(&iSize, 4);

	// 이름을 기록한다.

	file->Write(strName, iSize);

	// 오브젝트 타입을 기록한다.

	file->Write(&OT_type, 4);
	
}

void CObj::Load(CFilestream * file)
{
	if (!file->GetOpen())
		return;

	if (file->GetFileMode() != FM_READ)
		return;

	int iSize;

	// 이름의 사이즈를 읽어온다.

	file->Read(&iSize, 4);

	// 이름을 읽는다.

	// 처음에 셋팅한 이름이 길면, 뒤에까지 이름이 생김... NULL
	memset(strName, 0, NAME_SIZE_MAX);
	file->Read(strName, iSize);

	// 오브젝트 타입을 읽는다.

	file->Read(&OT_type, 4);

}



