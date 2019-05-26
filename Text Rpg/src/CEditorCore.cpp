#include "CEditorCore.h"
#include "CObjManager.h"
#include "CStageManager.h"



DEFINITION_SINGLE(CEditorCore);

CEditorCore::CEditorCore()
{
}


CEditorCore::~CEditorCore()
{
}

int CEditorCore::OutPutEditMenu()
{
	while (true)
	{ 
		SHOW2("1. 추가하기");
		SHOW2("2. 저장하기");
		SHOW2("3. 출력하기");
		SHOW2("4. 불러오기");
		SHOW2("5. 모두 삭제하기");
		SHOW2("6. 뒤로가기");
		cout << "메뉴를 선택하세요 : ";
		int iMenu = Input<int>();

		if (iMenu <= ED_NONE || iMenu > ED_BACK)
			continue;

		return iMenu;

	}
}
void CEditorCore::ArmorStore()
{
	while (true)
	{
		system("cls");
		SHOW("방어구 편집");

		switch (OutPutEditMenu())
		{

		case ED_INSERT:
			GET_SINGLE(CObjManager)->CreateItem(OT_ARMOR);
			break;

		case ED_SAVE:

			GET_SINGLE(CObjManager)->SaveArmorStore();
			break;

		case ED_LIST: 
			GET_SINGLE(CObjManager)->StoreRender(OT_ARMOR, true);
			break;

		case ED_LOAD:
			GET_SINGLE(CObjManager)->LoadArmorStore();

			break;

		case ED_DELETE_ALL: 
			while (true)
			{
				system("cls");
				cout << endl;

				SHOW2("1. 그렇다");
				SHOW2("2. 아니다");
				int iMenu = Input<int>();

				if (iMenu <= 0 || iMenu > 2)
					continue;

				else if (iMenu == 2)
					break;

				GET_SINGLE(CObjManager)->DeleteArmorStore();
				cout << "모든 방어구를 삭제했습니다. " << endl;
				system("pause");

				break;
			}

			break;



		case ED_BACK:
			return;

		}
	}
}

void CEditorCore::WeaponStore()
{
	while (true)
	{
		system("cls");
		SHOW("무기 편집");

		switch (OutPutEditMenu())
		{

		case ED_INSERT:
			GET_SINGLE(CObjManager)->CreateItem(OT_WEAPON);
			break;

		case ED_SAVE:

			GET_SINGLE(CObjManager)->SaveWeaponStore();
			break;

		case ED_LIST:
			GET_SINGLE(CObjManager)->StoreRender(OT_WEAPON, true);
			break;

		case ED_LOAD:
			GET_SINGLE(CObjManager)->LoadWeaponStore();

			break;

		case ED_DELETE_ALL:
			while (true)
			{
				system("cls");
				cout << endl;

				SHOW2("1. 그렇다");
				SHOW2("2. 아니다");
				int iMenu = Input<int>();

				if (iMenu <= 0 || iMenu > 2)
					continue;

				else if (iMenu == 2)
					break;

				GET_SINGLE(CObjManager)->DeleteWeaponStore();
				cout << "모든 무기를 삭제했습니다. " << endl;
				system("pause");
				break;
			}

			break;



		case ED_BACK:
			return;

		}
	}
}

void CEditorCore::RubyStore()
{
	while (true)
	{
		system("cls");
		SHOW("장신구 편집");

		switch (OutPutEditMenu())
		{

		case ED_INSERT:
			GET_SINGLE(CObjManager)->CreateItem(OT_RUBY);
			break;

		case ED_SAVE:

			GET_SINGLE(CObjManager)->SaveRubyStore();
			break;

		case ED_LIST:
			GET_SINGLE(CObjManager)->StoreRender(OT_RUBY, true);
			break;

		case ED_LOAD:
			GET_SINGLE(CObjManager)->LoadRubyStore();

			break;

		case ED_DELETE_ALL:
			while (true)
			{
				system("cls");
				cout << endl;

				SHOW2("1. 그렇다");
				SHOW2("2. 아니다");
				int iMenu = Input<int>();

				if (iMenu <= 0 || iMenu > 2)
					continue;

				else if (iMenu == 2)
					break;

				GET_SINGLE(CObjManager)->DeleteRubyStore();
				cout << "모든 장신구를 삭제했습니다. " << endl;
				system("pause");
				break;
			}

			break;



		case ED_BACK:
			return;

		}
	}
}

int CEditorCore::OutPutStoreMenu()
{
	while (true)
	{
		system("cls");
		cout << endl;
		SHOW("아이템 편집");
		SHOW2("1. 무기 추가");
		SHOW2("2. 방어구 추가");
		SHOW2("3. 장신구 추가");
		SHOW2("4. 뒤로가기");
		cout << "어떤 아이템을 추가할까요? : ";
		int iMenu = Input<int>();

		if (iMenu <= IT_NONE || iMenu > IT_BACK)
			continue;

		return iMenu;
	}
}



void CEditorCore::Run()
{
	while (true)
	{
		system("cls");
		switch (OutPutMenu())
		{
		case MM_MONSTER:
			while (true)
			{
				system("cls");
				SHOW("몬스터 편집");
				bool ok = false;

				switch (OutPutEditMenu())
				{

				case ED_INSERT:
					if (GET_SINGLE(CStageManager)->m_vecStage.size() == 0)
					{
						// 스테이지가 하나도 없다.

						cout << "스테이지가 하나도 없습니다." << endl;
						cout << "스테이지를 먼저 추가하세요. " << endl;
						system("pause");
						break;
					}

					// 스테이지 >= 1
					GET_SINGLE(CObjManager)->CreateMonster();
					break;


				case ED_SAVE:

					GET_SINGLE(CObjManager)->SaveMonster();
					break;

				case ED_LIST: 

					MonsterList();
					GET_SINGLE(CStageManager)->StageList(); // Stage & Monster
					
					
					break;
					
				case ED_LOAD: 
					GET_SINGLE(CObjManager)->LoadMonster();

					break;

				case ED_DELETE_ALL: 
					while (true)
					{
						system("cls");
						cout << endl;
						SHOW2("1. 그렇다");
						SHOW2("2. 아니다");
						int iMenu = Input<int>();

						if (iMenu <= 0 || iMenu > 2)
							continue;

						else if (iMenu == 2)
							break;

						
						
						GET_SINGLE(CObjManager)->DeleteMonsterAll();
						cout << "모든 몬스터를 삭제했습니다. " << endl;
						system("pause");
						break;
					}

					break;



				case ED_BACK:
					ok = true;
					break;

				}

				if (ok) break;
			}
			break;

			///////////////////////////////////////
		case MM_ITEM:

			while (true)
			{
				bool ok = false;
				switch (OutPutStoreMenu())
				{
				case IT_WEAPON:
					WeaponStore();
					break;

				case IT_ARMOR:
					ArmorStore();
					break;

				case IT_RUBY:
					RubyStore();
					break;

				case IT_BACK:
					ok = true;
					break;
				}
				if (ok) break;
			}
			break;

			//////////////////////////////////////
		case MM_STAGE:
			while (true)
			{
				system("cls");
				SHOW("스테이지 편집");
				bool ok = false;

				switch (OutPutEditMenu())
				{

				case ED_INSERT:
					GET_SINGLE(CStageManager)->CreateStage();
					break;


				case ED_SAVE:
					GET_SINGLE(CStageManager)->StageSave();

					break;

				case ED_LIST: 
					GET_SINGLE(CStageManager)->StageList(); 

					break;

				case ED_LOAD:
					GET_SINGLE(CStageManager)->StageLoad();

					break;

				case ED_DELETE_ALL: 
					while (true)
					{
						system("cls");
						cout << endl;

						SHOW2("1. 그렇다");
						SHOW2("2. 아니다");
						int iMenu = Input<int>();

						if (iMenu <= 0 || iMenu > 2)
							continue;

						else if (iMenu == 2)
							break;

						GET_SINGLE(CStageManager)->DeleteVector();
						cout << "모든 스테이지를 삭제했습니다. " << endl;
						system("pause");
						break;
					}

					break;



				case ED_BACK:
					ok = true;
					break;

				}

				if (ok) break;
			}
			break;
		
			//////////////////////////////////////
		case MM_LEVEL_UP_EXP:

			while (true)
			{
				system("cls");
				SHOW("레벨 테이블 편집");
				bool ok = false;

				switch (OutPutEditMenu())
				{

				case ED_INSERT:
					GET_SINGLE(CObjManager)->CreateLevelTable();
					break;


				case ED_SAVE:
					GET_SINGLE(CObjManager)->SaveLevelUpTable();
					break;

				case ED_LIST:
					GET_SINGLE(CObjManager)->LevelUpTableList();
					break;

				case ED_LOAD:
					GET_SINGLE(CObjManager)->LoadLevelUpTable();
					break;

				case ED_DELETE_ALL:
					while (true)
					{
						system("cls");
						cout << endl;

						SHOW2("1. 그렇다");
						SHOW2("2. 아니다");
						int iMenu = Input<int>();

						if (iMenu <= 0 || iMenu > 2)
							continue;

						else if (iMenu == 2)
							break;

						GET_SINGLE(CObjManager)->DeleteLevelUpTable();
						break;
					}

					break;



				case ED_BACK:
					ok = true;
					break;

				}

				if (ok) break;
			}
			
			break;


			/////////////////////////////////////
		case MM_EXIT:
			
			return;
		}

		system("pause");

	}
}

bool CEditorCore::Init()
{
	return true;
}

int CEditorCore::OutPutMenu()
{
		system("cls");
		SHOW("편집 모드");
		SHOW2("1. 몬스터 편집");
		SHOW2("2. 아이템 편집");
		SHOW2("3. 스테이지 편집");
		SHOW2("4. 레벨업 테이블 편집");
		SHOW2("5. 뒤로가기");
	int iMenu = Input<int>();

	if (iMenu > MM_EXIT || iMenu <= MM_NONE)
		return MM_NONE;

	return iMenu;
}

void CEditorCore::MonsterList()
{
	system("cls");
	SHOW("몬스터 출력");
	int iSize = GET_SINGLE(CObjManager)->m_tMonster.size();

	for (int i = 0; i < iSize; ++i)
	{
		cout << i + 1 << ".";
		GET_SINGLE(CObjManager)->m_tMonster[i]->Render();
		cout << endl;
	}
	
	system("pause");
}

