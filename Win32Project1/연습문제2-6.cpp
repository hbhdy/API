#include <Windows.h>
#include <string.h>

HINSTANCE g_hlnst;
LPCTSTR lpszClass = "window program 1 - 2";

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM IParam);

int WINAPI WinMain(HINSTANCE hlnstance, HINSTANCE hPrevlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hlnst = hlnstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hlnstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL,
		hlnstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static char text[10][100];
	static char Cpytext[10][100];
	static SIZE size;
	static int yul, xPos, yPos, hyang = 0;
	static bool re = true;

	switch (iMessage)
	{
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		yul = 0;
		return 0;
	case WM_CHAR:
		//if (text[hyang][yul - 1] >= 48 && text[hyang][yul - 1] < 58)  //숫자 인지 판별
		//{
		//	hyang++;
		//	yPos = yPos + 20;
		//	yul = 0;
		//}
		/*text[hyang][yul++] = wParam;*/
		
		if (wParam >= 48 && wParam < 58 && yul >= 1)
		{
			yul = 0;
			hyang ++ ;
			yPos = yPos + 20;
			//text[hyang][yul++] = wParam;
		}
		else {
			text[hyang][yul++] = wParam;
			if (text[hyang][yul - 1] >= 48 && text[hyang][yul - 1] < 58)
			{
				yul = 0;
				hyang++;
				yPos += 20;
				//text[hyang][yul++] = wParam;
			}
		}

		if (wParam == VK_BACK) // 벡스페이스
		{
			if (yul == 0 && hyang == 0)
			{
				break;
			}
			yul--;
			if (yul <= 0)
			{
				hyang--;
				yul = strlen(text[hyang]);
				yPos = yPos - 20;
			}
		}
		else if (wParam == VK_RETURN)  // 엔터
		{
			if (hyang < 9) {
				yul = 0;
				yPos = yPos + 20;
				hyang++;
			}
		}
		else if (yul >= 10)  // 10칸 입력시 다음행으로
		{
			hyang++;
			yPos = yPos + 20;
			yul = 0;
		}
		else if (wParam == 'r' || wParam == 'R')   // 반대로 출력
		{

			for (int i = 0; i <= hyang; ++i) {
				for (int j = 0; j < 100; ++j) {
					Cpytext[i][j] = text[i][j];
				}
			}
			int temp = hyang - 1;

			for (int i = 0; i <= hyang; ++i) {
				for (int j = 0; j < 100; ++j) {
					text[i][j] = Cpytext[temp][j];
				}
				temp--;
			}
		}
		else if (text[hyang][yul - 1] == 'q' || text[hyang][yul - 1] == 'Q')  // 종료
		{
			PostQuitMessage(0);
		}
		text[hyang][yul] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		for (int i = 0; i <= hyang; ++i) {
			GetTextExtentPoint(hDC, text[hyang], strlen(text[hyang]), &size);
			TextOut(hDC, 0, (i * 20), text[i], strlen(text[i]));
			SetCaretPos(size.cx, yPos);
		}

		for (int i = 0; i <= hyang; ++i) {
			for (int j = 0; j < 100; ++j) {
				GetTextExtentPoint(hDC, text[i], strlen(text[i]), &size);
				if (text[i][j] >= 48 && text[i][j] < 58) {
					SetTextColor(hDC, RGB(255, 0, 0));
					TextOut(hDC, 0, (i * 20), text[i], strlen(text[i]));
				}
				SetCaretPos(size.cx, yPos);
			}
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

//아스키 48 ~ 57 
