#include "CCore.h"
#include "CObjManager.h"
#include "CStageManager.h"
#include "CFilestream.h"

DEFINITION_SINGLE(CCore);

CCore::CCore()
{
}


CCore::~CCore()
{
	DESTROY_SINGLE(CObjManager);
	DESTROY_SINGLE(CStageManager);
}

void CCore::Run()
{
	while (true)
	{
		switch (OutPutMenu())
		{
		case MM_MAP:
			
			GET_SINGLE(CStageManager)->Run();
			break;


		case MM_STORE:
			Store();
			break;


		case MM_INVENTORY:

			GET_SINGLE(CObjManager)->m_tCurPlayer->InventoryItemList();
			break;


		case MM_INFORMATION:
			GET_SINGLE(CObjManager)->m_tCurPlayer->Render();

			system("pause");
			break;

		case MM_HOSPITAL:

			GET_SINGLE(CObjManager)->m_tCurPlayer->Hospital();
			break;

		case MM_SAVE: 
			
			while (true)
			{
				bool ok = false;
				switch (SaveOutPutMenu())
				{
				case SC_ONE:
					SavePlayer("Player1.ply");
					break;

				case SC_TWO:
					SavePlayer("Player2.ply");

					break;

				case SC_THREE:
					SavePlayer("Player3.ply");

					break;

				case SC_BACK:
					ok = true;
					break;

				case SC_DELETE:
					SaveDelete();
					break;
				}

				if (ok) break;
			}
			break;

		case MM_LOAD: // m_tCurPlayer에 로드.

		
			while (true)
			{
				bool ok = false;
				switch (LoadOutPutMenu())
				{
				case SC_ONE:
					LoadPlayer("Player1.ply");
					break;

				case SC_TWO:
					LoadPlayer("Player2.ply");

					break;

				case SC_THREE:
					LoadPlayer("Player3.ply");

					break;

				case SC_BACK:
					ok = true;
					break;


				case SC_DELETE:
					SaveDelete();
					break;
				}

				if (ok) break;
			}

			break;

		case MM_BACK:
			return;
		}
	}
}

bool CCore::Init()
{
	// 오브젝트 매니저 초기화.
	if (!GET_SINGLE(CObjManager)->Init())
		return false;

	GET_SINGLE(CObjManager)->LoadLevelUpTable(); // 레벨업 테이블 생성

	// 현재 플레이어의 레벨업 데이블 초기화.
	GET_SINGLE(CObjManager)->m_tCurPlayer->LoadLevelUpTable();

	// 스테이지 매니저 초기화
	if (!GET_SINGLE(CStageManager)->Init())
		return false;


	return true;
}

bool CCore::Load()
{

	GET_SINGLE(CObjManager)->InitLoad(); // 하드코딩 or Not

	GET_SINGLE(CObjManager)->m_tCurPlayer = new CPlayer; // 플레이어 생성

	GET_SINGLE(CObjManager)->LoadLevelUpTable(); // 레벨업 테이블 생성

	GET_SINGLE(CObjManager)->m_tCurPlayer->LoadLevelUpTable(); // 레벨업 데이블 복사과정

	// 스테이지 매니저 초기화
	if (!GET_SINGLE(CStageManager)->Init()) // 하드코딩 or Not 
		return false;

	return true;
}

int CCore::OutPutMenu()
{
	system("cls");
	cout << endl;
	SHOW("로비");
	SHOW2("1. 맵");
	SHOW2("2. 상점");
	SHOW2("3. 인벤토리");
	SHOW2("4. 내 정보");
	SHOW2("5. 병원");
	SHOW2("6. 저장하기");
	SHOW2("7. 불러오기");
	SHOW2("8. 뒤로가기");
	cout << "메뉴를 선택하세요 : ";
	int iMenu = Input<int>();

	if (iMenu <= MM_NONE || iMenu > MM_BACK)
		return MM_NONE;

	return iMenu;
}


void CCore::SaveDelete()
{
	while (true)
	{
		system("cls");
		SHOW2("삭제하기");
		// 어느 데이터를 삭제할지 정한다.
		GET_SINGLE(CObjManager)->PlayerSaveVecFree();
		GET_SINGLE(CObjManager)->InitSavePlayer();
		// 3 배열에 각 정보를 받아온다. 1 ~ 3까지. 
		cout << endl;
		// 다만 받아오고서 어떤 데이터가 사라졌는지 확인하기 위해서 계속 받아옴.
		for (int i = 0; i < 3; ++i)
		{
			SHOW2(i + 1);

			if ((GET_SINGLE(CObjManager)->m_tPlayer[i]->OT_type) == OT_NONE) // 잡힌 메모리가 저장이 되어 있지 않으면
			{
				cout << "=====================" << endl;
				cout << "  데이터가 없습니다" << endl;
				cout << "=====================" << endl << endl;
			}
			else
			{
				GET_SINGLE(CObjManager)->m_tPlayer[i]->LoadLevelUpTable();
				GET_SINGLE(CObjManager)->m_tPlayer[i]->Render();
			}
		}


		SHOW2("4. 뒤로가기");
			cout << "삭제할 데이터를 선택하세요 : ";

			int iMenu = Input<int>();

			if (iMenu <= 0 || iMenu > 4)
				continue;

			else if (iMenu == 4)
				return;

			if ((GET_SINGLE(CObjManager)->m_tPlayer[iMenu - 1]->OT_type) == OT_NONE) // 없는 데이터를 삭제하려고 한다.
			{
				cout << endl;
				cout << "  데이터가 없습니다" << endl;
				cout << endl;
			}
			
			else // 제대로 있는 데이터를 삭제한다.
			{
				// 그곳에다가 아무 값이나 쓰고서 닫으면 된다. 그러면 다음에 읽을 때 생성자에 의해서 기본값을 주고 데이터 없음으로 셋팅.
				const char* arr[3] = { "Player1.ply", "Player2.ply" , "Player3.ply" };

				CFilestream file;
				file.Open(arr[iMenu-1], "wb");
				file.Read("", 1);
				file.Close();

				system("cls");
				cout << "=======================" << endl;
				cout << "데이터가 삭제되었습니다." << endl;
				cout << "=======================" << endl << endl;
			}

			system("pause");

	}
}


int CCore::SaveOutPutMenu()
{
	system("cls");

	GET_SINGLE(CObjManager)->PlayerSaveVecFree();
	GET_SINGLE(CObjManager)->InitSavePlayer();
	// 3배열에 각 정보를 받아온다. 1 ~ 3까지.

	while (true)
	{

		system("cls");
		cout << endl;

		for (int i = 0; i < 3; ++i)
		{
			SHOW2(i + 1);
			if ((GET_SINGLE(CObjManager)->m_tPlayer[i]->OT_type) == OT_NONE) // 잡힌 메모리가 저장이 되어 있지 않으면
			{
				cout << "=====================" << endl;
				cout << "  데이터가 없습니다" << endl;
				cout << "=====================" << endl << endl;
			}
			else 
			{
				GET_SINGLE(CObjManager)->m_tPlayer[i]->LoadLevelUpTable();
				GET_SINGLE(CObjManager)->m_tPlayer[i]->Render();
			}


		}
		cout << "You";
		GET_SINGLE(CObjManager)->m_tCurPlayer->Render();

		SHOW2("4. 뒤로가기");
		SHOW2("5. 데이터 삭제");

		cout << endl << "참고 : 착용하고 있던 아이템은 인벤토리에 저장됩니다. " << endl << endl;
		cout << "어느 곳에 저장을 하시겠습니까 ? : ";

		int iMenu = Input<int>();

		if (iMenu <= SC_NONE || iMenu > SC_DELETE)
			continue;

		return iMenu;

	}


}

int CCore::LoadOutPutMenu()
{
	system("cls");
	// 어느 케이스에서 읽어야 할지 정해야 한다.

	GET_SINGLE(CObjManager)->PlayerSaveVecFree(); // 할당이 되어있으면 다시 할당을 해야 하니까 해제
	GET_SINGLE(CObjManager)->InitSavePlayer();
	// 3배열에 각 정보를 받아온다. 1 ~ 3까지.


	while (true)
	{

		system("cls");
		cout << endl;

		for (int i = 0; i < 3; ++i)
		{
			SHOW2(i + 1);
			if ((GET_SINGLE(CObjManager)->m_tPlayer[i]->OT_type) == OT_NONE) // 잡힌 메모리가 저장이 되어 있지 않으면
			{
				cout << "=====================" << endl;
				cout << "  데이터가 없습니다" << endl;
				cout << "=====================" << endl << endl;
			}
			else
			{
				GET_SINGLE(CObjManager)->m_tPlayer[i]->LoadLevelUpTable();
				GET_SINGLE(CObjManager)->m_tPlayer[i]->Render();
			}

		}
		cout << "You";
		GET_SINGLE(CObjManager)->m_tCurPlayer->Render();
		SHOW2("4. 뒤로가기");
		SHOW2("5. 데이터 삭제");

		cout << "어느 곳에서 불러오기를 하시겠습니까 ? : ";

		int iMenu = Input<int>();

		if (iMenu <= SC_NONE || iMenu > SC_DELETE)
			continue;

		else if (iMenu == SC_DELETE || iMenu == SC_BACK) // 4번과 5번을 눌렀을 경우에는 아래에 메세지가 출력되는 것을 막음
			return iMenu;

		if ((GET_SINGLE(CObjManager)->m_tPlayer[iMenu - 1]->OT_type) == OT_NONE)
		{
			// 없는 데이터를 고른다면...
			system("cls");
			cout << "=====================" << endl;
			cout << "데이터가 그대로 유지가 됩니다." << endl;
			cout << "=====================" << endl << endl;
			system("pause");
		}


		return iMenu;

	}

}

void CCore::SavePlayer(const char* pName)
{
	system("cls");
	CFilestream file(pName, "wb");

	GET_SINGLE(CObjManager)->m_tCurPlayer->Save(&file);
	// 세이브의 경우에는 현재 데이터를 옮기면 그만임.

	cout << "저장이 완료되었습니다." << endl;
	system("pause");
}

void CCore::LoadPlayer(const char * pName)
{
	system("cls");

	CFilestream file(pName, "rb");

	GET_SINGLE(CObjManager)->m_tCurPlayer->Load(&file);
	// 역시나 현재 데이터에서 pName에 있는 정보를 읽어오면 그만임.

	cout << "불러오기가 완료되었습니다." << endl;

	system("pause");

}



void CCore::Store()
{
	while (true)
	{
		switch (StoreOutPutMenu())
		{
		case ST_WEAPON:
			GET_SINGLE(CObjManager)->WeaponStoreList();
			break;

		case ST_ARMOR:
			GET_SINGLE(CObjManager)->ArmorStoreList();
			break;

		case ST_RUBY:
			GET_SINGLE(CObjManager)->RubyStoreList();
			break;

		case ST_SELL: // 인벤토리 뛰워주고 어떤 물품을 팔지 선택.
			GET_SINGLE(CObjManager)->m_tCurPlayer->SellStore();

		case ST_BACK:
			return;
		}
	}
}

int CCore::StoreOutPutMenu()
{
	system("cls");
	cout << endl;
	SHOW("상점");
	SHOW2("1. 무기 상점");
	SHOW2("2. 방어구 상점");
	SHOW2("3. 장신구 상점");
	SHOW2("4. 판매 상점");
	SHOW2("5. 뒤로가기");
	cout << "상점을 선택하세요 : ";
	int iMenu = Input<int>();

	if (iMenu <= ST_NONE || iMenu > ST_BACK)
		return 0;

	return iMenu;
}



