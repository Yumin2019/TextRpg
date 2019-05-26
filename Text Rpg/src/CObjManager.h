#pragma once


#include "value.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CItem.h"


class CObjManager
{
public:
	CObjManager();
	~CObjManager();

	DECLARE_SINGLE(CObjManager);


	CPlayer*			m_tPlayer[PLAYER_SAVE_SIZE];
	// 세이브 전용 변수.


	CPlayer*				m_tCurPlayer; // 현재 플레이어
	vector<CMonster*>		m_tMonster; // 모든 몬스터를 받아두는 곳
	vector<class CItem*>	m_vecWeapon; // 스토어 
	vector<class CItem*>	m_vecArmor;
	vector<class CItem*>	m_vecRuby;

	vector<long long> LevelUpTable; // ~ 100 레벨 테이블.


public:
	void CObjManager::LoadLevelUpTable();

	void CreateLevelTable();
	void SaveLevelUpTable();
	void LevelUpTableList();
	void DeleteLevelUpTable();

	bool InitLoad();
	bool ItemInit();
	bool MonsterInit();
	bool ObjectInit();
	void LoadRubyStore();
	void LoadWeaponStore();
	void LoadArmorStore();

	void SaveRubyStore();
	void SaveWeaponStore();
	void SaveArmorStore();

	void DeleteMonsterAll();
	void DeleteRubyStore();
	void DeleteArmorStore();
	void DeleteWeaponStore();

	void SaveMonster();
	void LoadMonster();

	void CreateMonster();
	void CreateItem(OBJECT_TYPE eType);

	void PlayerSaveVecFree();

	class CObj* CreateObject(OBJECT_TYPE eType);
	void Run();
	bool Init();

	bool InitSavePlayer();

	// 각 상점의 리스트를 출력하는 함수 ~

	void ArmorStoreList();
	void WeaponStoreList();
	void RubyStoreList();

	void StoreRender(OBJECT_TYPE eType, bool ok);
};

