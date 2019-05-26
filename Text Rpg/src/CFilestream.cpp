#include "CFilestream.h"



CFilestream::CFilestream() :
	m_bOpen(false),
	m_FileMode(FM_NONE),
	pFile(NULL)
{
}

CFilestream::CFilestream(const char * pName, const char * pMode)
{
	Open(pName, pMode);
}


CFilestream::~CFilestream()
{
	Close();
}

bool CFilestream::Open(const char* pName,const char* pMode)
{

	if (m_bOpen)
		return false;

	fopen_s(&pFile, pName, pMode);

	if (!pFile)
		return false;


	switch (pMode[0])
	{
	case 'r':
	case 'R':
		m_FileMode = FM_READ;
		break;
	case 'w':
	case 'W':
		m_FileMode = FM_WRITE;
		break;
	}

	m_bOpen = true;

	return true;
}

bool CFilestream::Close()
{
	if (!m_bOpen)
		return false;

	fclose(pFile);
	pFile = NULL;
	m_bOpen = false;
	m_FileMode = FM_NONE;

	return true;
}

bool CFilestream::Read(void * pName, int iSize)
{
	if (m_FileMode != FM_READ)
		return false;

	if (!m_bOpen)
		return false;

	fread(pName, iSize, 1, pFile);

	return true;
}

bool CFilestream::Write(void * pName, int iSize)
{
	if (m_FileMode != FM_WRITE)
		return false;

	if (!m_bOpen)
		return false;

	fwrite(pName, iSize, 1, pFile);

	return true;
}
