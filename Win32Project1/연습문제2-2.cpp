#include <Windows.h>
#include <stdio.h>
#include <cstring>

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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1200, 800,
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
	char text[100];
	int i = 0;


	
	switch (iMessage)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		rect.top = 0;
		rect.bottom = 40;
		{
			for (int W = 1; W < 11; W++)
			{
				rect.left = 0;
				rect.right = 50;
				for (int x = 1; x < W; x++)
				{
					rect.left += 50;
					rect.right += 50;
				}
				for (int H = 11; H > W; H--)
				{
					i++;
					wsprintf(text, "(%d,%d)", W, i);
					DrawText(hDC, text, strlen(text), &rect, DT_CENTER);
					rect.left += 50;
					rect.right += 50;
				}
				i = W;
				rect.top += 40;
				rect.bottom += 40;
			}
		}
		//////////////////////////////////////////////////
		rect.top = 0;
		rect.bottom = 40;
		{
			for (int Z = 1; Z <11 ; Z++)
			{
				rect.left =500;
				rect.right = 550;
				for (int Y = 11; Z<Y; Y--)
				{
					i++;
					wsprintf(text, "(%d,%d)", Z, i);
					DrawText(hDC, text, strlen(text), &rect, DT_CENTER);
					rect.left += 50;
					rect.right += 50;
				}
				i = 10;
				rect.top += 40;
				rect.bottom += 40;
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}