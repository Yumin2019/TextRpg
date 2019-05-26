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

		case MM_LOAD: // m_tCurPlayer�� �ε�.

		
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
	// ������Ʈ �Ŵ��� �ʱ�ȭ.
	if (!GET_SINGLE(CObjManager)->Init())
		return false;

	GET_SINGLE(CObjManager)->LoadLevelUpTable(); // ������ ���̺� ����

	// ���� �÷��̾��� ������ ���̺� �ʱ�ȭ.
	GET_SINGLE(CObjManager)->m_tCurPlayer->LoadLevelUpTable();

	// �������� �Ŵ��� �ʱ�ȭ
	if (!GET_SINGLE(CStageManager)->Init())
		return false;


	return true;
}

bool CCore::Load()
{

	GET_SINGLE(CObjManager)->InitLoad(); // �ϵ��ڵ� or Not

	GET_SINGLE(CObjManager)->m_tCurPlayer = new CPlayer; // �÷��̾� ����

	GET_SINGLE(CObjManager)->LoadLevelUpTable(); // ������ ���̺� ����

	GET_SINGLE(CObjManager)->m_tCurPlayer->LoadLevelUpTable(); // ������ ���̺� �������

	// �������� �Ŵ��� �ʱ�ȭ
	if (!GET_SINGLE(CStageManager)->Init()) // �ϵ��ڵ� or Not 
		return false;

	return true;
}

int CCore::OutPutMenu()
{
	system("cls");
	cout << endl;
	SHOW("�κ�");
	SHOW2("1. ��");
	SHOW2("2. ����");
	SHOW2("3. �κ��丮");
	SHOW2("4. �� ����");
	SHOW2("5. ����");
	SHOW2("6. �����ϱ�");
	SHOW2("7. �ҷ�����");
	SHOW2("8. �ڷΰ���");
	cout << "�޴��� �����ϼ��� : ";
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
		SHOW2("�����ϱ�");
		// ��� �����͸� �������� ���Ѵ�.
		GET_SINGLE(CObjManager)->PlayerSaveVecFree();
		GET_SINGLE(CObjManager)->InitSavePlayer();
		// 3 �迭�� �� ������ �޾ƿ´�. 1 ~ 3����. 
		cout << endl;
		// �ٸ� �޾ƿ��� � �����Ͱ� ��������� Ȯ���ϱ� ���ؼ� ��� �޾ƿ�.
		for (int i = 0; i < 3; ++i)
		{
			SHOW2(i + 1);

			if ((GET_SINGLE(CObjManager)->m_tPlayer[i]->OT_type) == OT_NONE) // ���� �޸𸮰� ������ �Ǿ� ���� ������
			{
				cout << "=====================" << endl;
				cout << "  �����Ͱ� �����ϴ�" << endl;
				cout << "=====================" << endl << endl;
			}
			else
			{
				GET_SINGLE(CObjManager)->m_tPlayer[i]->LoadLevelUpTable();
				GET_SINGLE(CObjManager)->m_tPlayer[i]->Render();
			}
		}


		SHOW2("4. �ڷΰ���");
			cout << "������ �����͸� �����ϼ��� : ";

			int iMenu = Input<int>();

			if (iMenu <= 0 || iMenu > 4)
				continue;

			else if (iMenu == 4)
				return;

			if ((GET_SINGLE(CObjManager)->m_tPlayer[iMenu - 1]->OT_type) == OT_NONE) // ���� �����͸� �����Ϸ��� �Ѵ�.
			{
				cout << endl;
				cout << "  �����Ͱ� �����ϴ�" << endl;
				cout << endl;
			}
			
			else // ����� �ִ� �����͸� �����Ѵ�.
			{
				// �װ����ٰ� �ƹ� ���̳� ���� ������ �ȴ�. �׷��� ������ ���� �� �����ڿ� ���ؼ� �⺻���� �ְ� ������ �������� ����.
				const char* arr[3] = { "Player1.ply", "Player2.ply" , "Player3.ply" };

				CFilestream file;
				file.Open(arr[iMenu-1], "wb");
				file.Read("", 1);
				file.Close();

				system("cls");
				cout << "=======================" << endl;
				cout << "�����Ͱ� �����Ǿ����ϴ�." << endl;
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
	// 3�迭�� �� ������ �޾ƿ´�. 1 ~ 3����.

	while (true)
	{

		system("cls");
		cout << endl;

		for (int i = 0; i < 3; ++i)
		{
			SHOW2(i + 1);
			if ((GET_SINGLE(CObjManager)->m_tPlayer[i]->OT_type) == OT_NONE) // ���� �޸𸮰� ������ �Ǿ� ���� ������
			{
				cout << "=====================" << endl;
				cout << "  �����Ͱ� �����ϴ�" << endl;
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

		SHOW2("4. �ڷΰ���");
		SHOW2("5. ������ ����");

		cout << endl << "���� : �����ϰ� �ִ� �������� �κ��丮�� ����˴ϴ�. " << endl << endl;
		cout << "��� ���� ������ �Ͻðڽ��ϱ� ? : ";

		int iMenu = Input<int>();

		if (iMenu <= SC_NONE || iMenu > SC_DELETE)
			continue;

		return iMenu;

	}


}

int CCore::LoadOutPutMenu()
{
	system("cls");
	// ��� ���̽����� �о�� ���� ���ؾ� �Ѵ�.

	GET_SINGLE(CObjManager)->PlayerSaveVecFree(); // �Ҵ��� �Ǿ������� �ٽ� �Ҵ��� �ؾ� �ϴϱ� ����
	GET_SINGLE(CObjManager)->InitSavePlayer();
	// 3�迭�� �� ������ �޾ƿ´�. 1 ~ 3����.


	while (true)
	{

		system("cls");
		cout << endl;

		for (int i = 0; i < 3; ++i)
		{
			SHOW2(i + 1);
			if ((GET_SINGLE(CObjManager)->m_tPlayer[i]->OT_type) == OT_NONE) // ���� �޸𸮰� ������ �Ǿ� ���� ������
			{
				cout << "=====================" << endl;
				cout << "  �����Ͱ� �����ϴ�" << endl;
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
		SHOW2("4. �ڷΰ���");
		SHOW2("5. ������ ����");

		cout << "��� ������ �ҷ����⸦ �Ͻðڽ��ϱ� ? : ";

		int iMenu = Input<int>();

		if (iMenu <= SC_NONE || iMenu > SC_DELETE)
			continue;

		else if (iMenu == SC_DELETE || iMenu == SC_BACK) // 4���� 5���� ������ ��쿡�� �Ʒ��� �޼����� ��µǴ� ���� ����
			return iMenu;

		if ((GET_SINGLE(CObjManager)->m_tPlayer[iMenu - 1]->OT_type) == OT_NONE)
		{
			// ���� �����͸� ���ٸ�...
			system("cls");
			cout << "=====================" << endl;
			cout << "�����Ͱ� �״�� ������ �˴ϴ�." << endl;
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
	// ���̺��� ��쿡�� ���� �����͸� �ű�� �׸���.

	cout << "������ �Ϸ�Ǿ����ϴ�." << endl;
	system("pause");
}

void CCore::LoadPlayer(const char * pName)
{
	system("cls");

	CFilestream file(pName, "rb");

	GET_SINGLE(CObjManager)->m_tCurPlayer->Load(&file);
	// ���ó� ���� �����Ϳ��� pName�� �ִ� ������ �о���� �׸���.

	cout << "�ҷ����Ⱑ �Ϸ�Ǿ����ϴ�." << endl;

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

		case ST_SELL: // �κ��丮 �ٿ��ְ� � ��ǰ�� ���� ����.
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
	SHOW("����");
	SHOW2("1. ���� ����");
	SHOW2("2. �� ����");
	SHOW2("3. ��ű� ����");
	SHOW2("4. �Ǹ� ����");
	SHOW2("5. �ڷΰ���");
	cout << "������ �����ϼ��� : ";
	int iMenu = Input<int>();

	if (iMenu <= ST_NONE || iMenu > ST_BACK)
		return 0;

	return iMenu;
}



