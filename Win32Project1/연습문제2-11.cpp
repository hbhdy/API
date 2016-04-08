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
	static int high, wide = 0;
	static int keynum;
	static int l = 20;
	static int t = 20;
	static int r = 21;
	static int b = 21;
	static int sizeUp = 1;
	static bool change = true;


	switch (iMessage)
	{
	case WM_KEYDOWN:
		if (wParam == VK_UP) {
			keynum = 1;
			t -= 1;
			b -= 1;
			if (t < 0 && b < 1)
			{
				t = 39;
				b = 40;
			}
		}
		else if (wParam == VK_LEFT) {
			keynum = 2;
			l -= 1;
			r -= 1;
			if (l < 0 && r < 1)
			{
				l = 39;
				r = 40;
			}
		}
		else if (wParam == VK_RIGHT) {
			keynum = 3;
			l += 1;
			r += 1;
			if (l > 39 && r > 40)
			{
				l = 0;
				r = 1;
			}
		}
		else if (wParam == VK_DOWN) {
			keynum = 4;
			t += 1;
			b += 1;
			if (t > 39 && b > 40)
			{
				t = 0;
				b = 1;
			}
		}

		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_CHAR:
		if (wParam == 'p') // + 입력 부분
		{
			sizeUp += sizeUp;
		}
		else if (wParam == 'm')// - 입력 부분
		{
			sizeUp /= 2;
			if (sizeUp < 1)
			{
				change = false;
				sizeUp = 1; 
			}
		}
		break;
	case WM_KEYUP:
		keynum = 0;
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		GetClientRect(hWnd, &rect);
		hDC = BeginPaint(hWnd, &ps);
		wide = rect.right / 40;
		high = rect.bottom / 40;
		for (int i = 0; i < 40; i++)
			for (int j = 0; j < 40; j++)
			{
				Rectangle(hDC, i*wide, j*high, (i + 1)*wide, (j + 1)*high); // left top right bottom
			}
		if (change == true)  //체인지가 참일때 네모로 그려진다
		{

			hBrush = CreateSolidBrush((RGB(0, 0, 255)));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			//Rectangle(hDC, l * wide, t * high, r*wide, b*high);
			Rectangle(hDC, l * wide, t * high, (r + sizeUp - 1)*wide, (b + sizeUp - 1)*high);

			if (keynum == 1) // 위쪽
			{
				Rectangle(hDC, l * wide, t * high, (r + sizeUp - 1)*wide, (b + sizeUp - 1)*high);
			}
			else if (keynum == 2) // 왼쪽
			{
				Rectangle(hDC, l * wide, t * high, (r + sizeUp - 1)*wide, (b + sizeUp - 1)*high);
			}
			else if (keynum == 3) // 오른쪽
			{
				Rectangle(hDC, l * wide, t * high, (r + sizeUp - 1)*wide, (b + sizeUp - 1)*high);
			}
			else if (keynum == 4) // 위쪽
			{
				Rectangle(hDC, l * wide, t * high, (r + sizeUp - 1)*wide, (b + sizeUp - 1)*high);
			}
		}
		if (change == false) // 체인지 거짓일때 원으로 그려진다
		{
			hBrush = CreateSolidBrush((RGB(0, 0, 255)));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, l* wide, t * high, (r+sizeUp-1)*wide, (b+sizeUp-1)*high);
			if (keynum == 1) // 위쪽
			{
				Ellipse(hDC, l* wide, t * high, (r + sizeUp - 1)*wide, (b + sizeUp - 1)*high);
			}
			else if (keynum == 2) // 왼쪽
			{
				Ellipse(hDC, l* wide, t * high, (r + sizeUp - 1)*wide, (b + sizeUp - 1)*high);
			}
			else if (keynum == 3) // 오른쪽
			{
				Ellipse(hDC, l* wide, t * high, (r + sizeUp - 1)*wide, (b + sizeUp - 1)*high);
			}
			else if (keynum == 4) // 위쪽
			{
				Ellipse(hDC, l* wide, t * high, (r + sizeUp - 1)*wide, (b + sizeUp - 1)*high);
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