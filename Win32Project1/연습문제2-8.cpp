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
	HBRUSH hBrush, oldBrush;
	srand((unsigned)time(NULL));
	int high, wide = 0;
	static int keynum;
	static int random = rand() % 4;
	static int l = random;
	static int t = random;
	static int r = random + 1;
	static int b = random + 1;
	static bool logic = true;

	switch (iMessage)
	{
	case WM_CHAR:
		if (wParam == 'r' || wParam == 'R')  // 도형의 변환
		{
			if (logic)
				logic = false;
			else
				logic = true;
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP) {
			keynum = 1;
			t -= 1;
			b -= 1;
			if (t < 0 && b < 1)
			{
				t = 3;
				b = 4;
			}
		}
		else if (wParam == VK_LEFT) {
			keynum = 2;
			l -= 1;
			r -= 1;
			if (l < 0 && r < 1)
			{
				l = 3;
				r = 4;
			}
		}
		else if (wParam == VK_RIGHT) {
			keynum = 3;
			l += 1;
			r += 1;
			if (l >3  &&  r > 4)
			{
				l = 0;
				r =1;
			}
		}
		else if (wParam == VK_DOWN) {
			keynum = 4;
			t += 1;
			b += 1;
			if (t > 3 && b > 4)
			{
				t = 0;
				b = 1;
			}
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_KEYUP:
		/*if (wParam == VK_UP) {
			t -= 1;
			b -= 1;
			if (t < 0 && b < 1)
			{
				t = 3;
				b = 4;
			}
		}
		else if (wParam == VK_LEFT) {
			l -= 1;
			r -= 1;
			if (l < 0 && r < 1)
			{
				l = 3;
				r = 4;
			}
		}
		else if (wParam == VK_RIGHT) {
			l += 1;
			r += 1;
			if (l >3 && r > 4)
			{
				l = 0;
				r = 1;
			}
		}
		else if (wParam == VK_DOWN) {
			t += 1;
			b += 1;
			if (t > 3 && b > 4)
			{
				t = 0;
				b = 1;
			}
		}*/
		keynum = 0;
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		GetClientRect(hWnd, &rect);
		hDC = BeginPaint(hWnd, &ps);
		wide = rect.right / 4;
		high = rect.bottom / 4;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				Rectangle(hDC, i*wide, j*high, (i+1)*wide, (j+1)*high); // left top right bottom
			}
		if (logic == true)
		{
			hBrush = CreateSolidBrush((RGB(0, 0, 255)));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Rectangle(hDC, l * wide, t * high, r*wide, b*high);
			if (keynum == 1) // 위쪽
			{
				hBrush = CreateSolidBrush((RGB(255, 0, 0)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, l* wide, t * high, r*wide, b*high);
			}
			else if (keynum == 2) // 왼쪽
			{
				hBrush = CreateSolidBrush((RGB(255, 0, 0)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, l* wide, t * high, r*wide, b*high);
			}
			else if (keynum == 3) // 오른쪽
			{
				hBrush = CreateSolidBrush((RGB(255, 0, 0)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, l* wide, t * high, r*wide, b*high);
			}
			else if (keynum == 4) // 위쪽
			{
				hBrush = CreateSolidBrush((RGB(255, 0, 0)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, l* wide, t * high, r*wide, b*high);
			}
		}
		else if (logic == false)
		{
			hBrush = CreateSolidBrush((RGB(0, 0, 255)));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, l* wide, t * high, r*wide, b*high);
			if (keynum == 1) // 위쪽
			{
				hBrush = CreateSolidBrush((RGB(255, 0, 0)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, l* wide, t * high, r*wide, b*high);
			}
			else if (keynum == 2) // 왼쪽
			{
				hBrush = CreateSolidBrush((RGB(255, 0, 0)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, l* wide, t * high, r*wide, b*high);
			}
			else if (keynum == 3) // 오른쪽
			{
				hBrush = CreateSolidBrush((RGB(255, 0, 0)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, l* wide, t * high, r*wide, b*high);
			}
			else if (keynum == 4) // 위쪽
			{
				hBrush = CreateSolidBrush((RGB(255, 0, 0)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, l* wide, t * high, r*wide, b*high);
			}
		}
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}