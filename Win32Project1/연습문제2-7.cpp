#include <windows.h>
#include <time.h>
HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Nam";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, 1280, 800, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDC;
	static char memo[100];
	static char copy_memo[100];
	static SIZE size;
	static int  col;
	static int countTemp = 0;
	static int Check;
	static int xPos = 0, yPos = 0;
	static bool F1 = false;  // F1키 용
	static bool P_D = false; // Page_down

	srand(unsigned(time(NULL)));


	switch (iMessage)
	{
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);

		memset(memo, NULL, sizeof(char) * 100);  // 메모리 함수
		memset(copy_memo, NULL, sizeof(char) * 100);

		break;
	case WM_CHAR:
		hDC = GetDC(hWnd);

		if (wParam == '1')   //F1 안되서 1번으로 함
		{
			if (F1)
			{
				strcpy(&memo[Check], copy_memo);

				Check = 0;
				countTemp = 0;
				memset(&copy_memo, NULL, sizeof(char) * 100);
				F1 = false;
			}
			else
			{
				Check = col;
				F1 = true;
			}
		}
		else if (wParam == '2')   // HOME 키를 2로 대체
		{
			xPos = rand() % 200;
			yPos = rand() % 200;
		}
		else if (wParam == VK_BACK)
		{
			if (col == 0)
				break;

			memo[0] = ' ';
			strcpy(memo, &memo[1]);
			--col;
		}
		else if (wParam == '3') // 페이지 다운키는 키를 누르면 10자씩 잘라서 보이게 하기
		{
			if (P_D)
				P_D = false;
			else
				P_D = true;
		}
		else
		{
			if (col == 100)
				break;
			else if (col < 100)
			{
				if (F1)
				{
					memo[col] = '*';
					++col;
					copy_memo[countTemp] = wParam;
					++countTemp;
				}
				else
				{
					memo[col] = wParam;
					++col;
				}
			}
		}
		memo[col] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if (P_D) //페이지 다운 키 입력시 분할
		{
			char split_memo[10][10];
			int splitnum = 0;

			for (int i = 0; i < 10; ++i)
				memset(split_memo[i], NULL, sizeof(char) * 10);

			for (int i = 0; i < 100; ++i)
			{
				if (memo[i] == NULL)
				{
					splitnum = i;
					break;
				}
				strcpy(&split_memo[i / 10][i % 10], &memo[i]);
			}
			for (int i = 0; i < splitnum / 10 + 1; ++i)
			{
				if (strlen(split_memo[i]) > 10)
					TextOut(hDC, xPos, yPos + i * 20, split_memo[i], 10);
				else
					TextOut(hDC, xPos, yPos + i * 20, split_memo[i], strlen(split_memo[i]));
			}
			GetTextExtentPoint(hDC, split_memo [splitnum / 10], strlen(split_memo[splitnum / 10]), &size);
			SetCaretPos(xPos + size.cx, yPos + (splitnum / 10) * 20);
		}
		else
		{
			TextOut(hDC, xPos, yPos,memo, strlen(memo));
			GetTextExtentPoint(hDC, memo, strlen(memo), &size);
			SetCaretPos(xPos + size.cx, yPos);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}