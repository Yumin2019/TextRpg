
#include "value.h"
#include "CCore.h"
#include "CEditorCore.h"

enum MAIN_MENU
{
	MM_NONE,
	MM_START,
	MM_LOAD,
	MM_EDIT,
	MM_EXIT
};
int main()
{
	system("title TEXT RPG - made by Yumin 2019.1 ");
	srand((unsigned int)time(NULL));

	// 편집 모드 / 인게임으로 나눠서 처리를 한다.

	while (true)
	{

		system("cls");
		cout << endl;
		SHOW(" TEXT RPG - made by Yumin 2019.1 ");
		SHOW2("1. 새로운 시작");
		SHOW2("2. 불러오기");
		SHOW2("3. 편집 모드");
		SHOW2("4. 종료");
		int iMenu = Input<int>();

		if (iMenu <= MM_NONE || iMenu > MM_EXIT)
			iMenu = MM_NONE;
		// 코어를 초기화 한다.

		switch (iMenu)
		{
		case MM_START:
			if (!GET_SINGLE(CCore)->Init())
			{
				DESTROY_SINGLE(CCore);
				return 0;
			}

			GET_SINGLE(CCore)->Run();
			DESTROY_SINGLE(CCore);

			break;
		case MM_LOAD: // 불러오기

			if (!GET_SINGLE(CCore)->Load())
			{
				DESTROY_SINGLE(CCore);
				return 0;
			}

			while (true) // 코어에 있는 것과 동일.
			{
				bool ok = false;
				switch (GET_SINGLE(CCore)->LoadOutPutMenu())
				{
				case SC_ONE:
					GET_SINGLE(CCore)->LoadPlayer("Player1.ply");
					break;

				case SC_TWO:
					GET_SINGLE(CCore)->LoadPlayer("Player2.ply");

					break;

				case SC_THREE:
					GET_SINGLE(CCore)->LoadPlayer("Player3.ply");

					break;

				case SC_BACK:
					ok = true;
					break;


				case SC_DELETE:
					GET_SINGLE(CCore)->SaveDelete();
					break;
				}

				if (ok) break;
			}
			// 불러오기 완료 만약에 위에서 없는 데이터를 누르고 와서 Run이 된다면 데이터는 NULL로... (진행이 불가함) 

			GET_SINGLE(CCore)->Run();


			DESTROY_SINGLE(CCore);

			break;


		case MM_EDIT:

			if (!GET_SINGLE(CEditorCore)->Init())
			{
				DESTROY_SINGLE(CEditorCore);
				return 0;
			}

			GET_SINGLE(CEditorCore)->Run();
			DESTROY_SINGLE(CEditorCore);

			break;
		case MM_EXIT:
			return 0;
		}
		
	}
	return 0;
	system("pause");
}