
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

	// ���� ��� / �ΰ������� ������ ó���� �Ѵ�.

	while (true)
	{

		system("cls");
		cout << endl;
		SHOW(" TEXT RPG - made by Yumin 2019.1 ");
		SHOW2("1. ���ο� ����");
		SHOW2("2. �ҷ�����");
		SHOW2("3. ���� ���");
		SHOW2("4. ����");
		int iMenu = Input<int>();

		if (iMenu <= MM_NONE || iMenu > MM_EXIT)
			iMenu = MM_NONE;
		// �ھ �ʱ�ȭ �Ѵ�.

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
		case MM_LOAD: // �ҷ�����

			if (!GET_SINGLE(CCore)->Load())
			{
				DESTROY_SINGLE(CCore);
				return 0;
			}

			while (true) // �ھ �ִ� �Ͱ� ����.
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
			// �ҷ����� �Ϸ� ���࿡ ������ ���� �����͸� ������ �ͼ� Run�� �ȴٸ� �����ʹ� NULL��... (������ �Ұ���) 

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