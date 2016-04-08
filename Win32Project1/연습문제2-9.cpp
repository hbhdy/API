#include <Windows.h>
#include <stdlib.h>
#include <time.h>

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
	RECT rect;
	SIZE size;
	HPEN hPen, oldPen;
	static char str[10][100];
	static int pos[10];
	static int col, row, yPos;
	static int pw = 0; // 펜의 굵기
	static int rand1, rand2, rand3 = 0;
	srand((unsigned)time(NULL));
	rand1=rand() % 255;
	rand2 = rand() % 255;
	rand3 = rand() % 255;   //색상의 랜덤값
	static bool wide1 = true; // +의 bool값
	static bool wide2 = true; // -의 bool값
	static bool start = true;

	switch (iMessage)
	{
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		col = 0;
		row = 0;
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);
		if (wParam == VK_SPACE) {
			pos[row] = atoi(str[row]);
			row++;
			col = 0;
		}
		break;

	case WM_CHAR:
		hDC = GetDC(hWnd);
		if (wParam == 'p') // 플러스
		{
			wide1 = false;
			pw++;
			str[row][col++] = '\0';
		}
		else if (wParam == 'm') //마이너스
		{
			wide2 = false;
			pw--;
			if (pw < 1)
				pw = 1;
			str[row][col++] = '\0';
		}
		if (((wParam>=48 && wParam<58)) || wParam==' '||wParam=='p'||wParam=='m')
		{
		start = true;
		}
		else if((wParam>=65 &&wParam<91) || (wParam>=97&&wParam<123))
		{
		start = false;
		}

		str[row][col++] = wParam;
		InvalidateRect(hWnd, NULL, true);
		
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		rect.left = 0;
		rect.top = 600;
		rect.right = 500;
		rect.bottom = 630;
		Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		for (int i = 0; i <= row; i++)
		{
			GetTextExtentPoint(hDC, str[i], strlen(str[i]), &size);
			TextOut(hDC, i*30, 610, str[i], strlen(str[i]));
			SetCaretPos(i*30+size.cx, 610);
		}
		hPen = CreatePen(PS_SOLID, pw, (RGB(rand1, rand2, rand3)));
		oldPen = (HPEN)SelectObject(hDC, hPen);
		if (pos[0] == 1)  // 직선 그리기
		{
			if (wide1 == false || wide2 ==false)
			{
				hPen = CreatePen(PS_SOLID, pw, (RGB(rand1, rand2, rand3)));
				oldPen = (HPEN)SelectObject(hDC, hPen);
				MoveToEx(hDC, pos[1], pos[2], NULL);
				LineTo(hDC, pos[3], pos[4]);
			}
			MoveToEx(hDC, pos[1], pos[2], NULL);
			LineTo(hDC, pos[3], pos[4]);
		}
		else if (pos[0] == 2) // 원 그리기
		{
			if (wide1 == false || wide2 == false)
			{
				hPen = CreatePen(PS_SOLID, pw, (RGB(rand1, rand2, rand3)));
				oldPen = (HPEN)SelectObject(hDC, hPen);
				Ellipse(hDC, pos[1], pos[2], pos[3], pos[4]);
			}
			Ellipse(hDC, pos[1], pos[2], pos[3], pos[4]);
		}
		else if (pos[0] == 3) // 사각형 그리기
		{
			if (wide1 == false || wide2 == false)
			{
				hPen = CreatePen(PS_SOLID, pw, (RGB(rand1, rand2, rand3)));
				oldPen = (HPEN)SelectObject(hDC, hPen);
				Rectangle(hDC, pos[1], pos[2], pos[3], pos[4]);
			}
			Rectangle(hDC, pos[1], pos[2], pos[3], pos[4]);
		}
		if (start == false)
		{
			TextOut(hDC, 400, 400, "에러입니다. 꺄르륵~!", strlen("에러입니다. 꺄르륵~!"));
		}
		SelectObject(hDC, oldPen);
		DeleteObject(hPen);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}