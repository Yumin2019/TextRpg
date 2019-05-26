#pragma once

#include <iostream>
#include <time.h>
#include <vector>


using namespace std;

// 레벨 업으로 증가할 수 있는 최대값

#define AVOID_POINT_MAX 55
#define CRITICAL_POINT_MAX 55

#define NAME_SIZE_MAX 32
#define DESC_SIZE_MAX 256
#define PLAYER_SAVE_SIZE 3


#define Ignore()  cin.ignore(1024,'\n')
#define SHOW(p) cout << "====================== " << p << " ======================" << endl << endl;
#define SHOW2(p) cout << " << " << p << " >> " << endl << endl;

#define DEFINITION_SINGLE(Type) Type* Type::m_pInst = NULL
#define GET_SINGLE(Type) Type::GetInst()
#define DESTROY_SINGLE(Type) Type::DeleteInst();

#define SAFE_DELETE(p) if(p) {delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p) {delete[] p; p = NULL;}
#define DECLARE_SINGLE(Type) \
private : \
static Type* m_pInst; \
public: \
static Type* GetInst()\
{\
if(!m_pInst) \
m_pInst = new Type; \
return m_pInst;\
}\
static void DeleteInst()\
{\
SAFE_DELETE(m_pInst); \
}\
 // 생성자 소멸자는 안 만듬

#define MAX_LEVEL LevelUpTable.size() + 1


template<typename T>
void Safe_Delete_VecList(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}

	p.clear();
}

template<typename T>
T Input()
{
	T input;
	cin >> input;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(1024, '\n');
		return 0;
	}

	return input;
}

typedef struct _tagItmeInfo
{
	int		iHP;
	int		iMP;
	int		iHPMax;
	int		iMPMax;
	int		iAttackMin;
	int		iAttackMax;
	int		iArmorMin;
	int		iArmorMax;
	int		iCriticalPercent;
	double	iAttackPercent;
	double	iArmorPercent;
	double	iAvoidPercent;
	double	iLuckyPercent;

	int		iPrice;
	int		iSell;



	// 구조체 연산 오버로딩 +

	_tagItmeInfo operator+=(const _tagItmeInfo& p)
	{
		iHP += p.iHP;
		iMP += p.iMP;
		iHPMax += p.iHPMax;
		iMPMax += p.iMPMax;
		iAttackMin += p.iAttackMin;
		iAttackMax += p.iAttackMax;
		iArmorMin += p.iArmorMin;
		iArmorMax += p.iArmorMax;
		iCriticalPercent += p.iCriticalPercent;
		iAttackPercent += p.iAttackPercent;
		iArmorPercent += p.iArmorPercent;
		iAvoidPercent += p.iAvoidPercent;
		iLuckyPercent += p.iLuckyPercent;
		// 전투에서 쓰려는데 가격은 쓸데가 없겠군요.

		return (*this);
	}
}ITEMINFO, *PITEMINFO;

typedef struct LevelUpInfo
{
	int		iHP;
	int		iMP;
	int		iAttackMin;
	int		iAttackMax;
	int		iArmorMin;
	int		iArmorMax;

	double	iAttackPercent;
	double	iArmorPercent;
	double	iAvoidPercent;
	double	iLuckyPercent;
	int			iCriticalPercent;


}LEVELUPINFO, *PLEVELUPINFO;

typedef struct _tagInfo
{
	int		iHP;
	int		iHPMax;
	int		iMP;
	int		iMPMax;
	int		iAttackMin;
	int		iAttackMax;
	int		iArmorMin;
	int		iArmorMax;
	int		iLevel;
	long long	 iExp;
}INFO, *PINFO;

typedef struct _tagPlayer
{
	double		iAttackPercent;
	double		iArmorPercent;
	double		iAvoidPercent;
	double		iLuckyPercent;
	int			iCriticalPercent;
	int			iPoint;
	int			iGold;
}PLAYER, *PPLAYER;
