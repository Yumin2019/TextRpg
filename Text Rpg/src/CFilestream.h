#pragma once

#include "value.h"
#include "Flag.h"

class CFilestream
{
public:
	CFilestream();
	CFilestream(const char* pName, const char* pMode);

	~CFilestream();

private:
	bool		m_bOpen;
	FILE_MODE	m_FileMode;
	FILE*		pFile;


public:

	FILE*	GetpFile()
	{
		return pFile;
	}
	bool GetOpen()
	{
		return m_bOpen;
	}
	bool Open(const char* pName, const char* pMode);
	bool Close();

	FILE_MODE GetFileMode()
	{
		return m_FileMode;
	}
	bool Read(void* pName, int iSize);
	bool Write(void* pName, int iSize);
};

