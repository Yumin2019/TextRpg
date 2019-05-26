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
	// ���̺� ���� ����.


	CPlayer*				m_tCurPlayer; // ���� �÷��̾�
	vector<CMonster*>		m_tMonster; // ��� ���͸� �޾Ƶδ� ��
	vector<class CItem*>	m_vecWeapon; // ����� 
	vector<class CItem*>	m_vecArmor;
	vector<class CItem*>	m_vecRuby;

	vector<long long> LevelUpTable; // ~ 100 ���� ���̺�.


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

	// �� ������ ����Ʈ�� ����ϴ� �Լ� ~

	void ArmorStoreList();
	void WeaponStoreList();
	void RubyStoreList();

	void StoreRender(OBJECT_TYPE eType, bool ok);
};

