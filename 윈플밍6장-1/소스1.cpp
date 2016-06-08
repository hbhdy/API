#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg6_1Proc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;
static int isStartMove = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hbrBackground = CreateSolidBrush(RGB(51, 181, 229));
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name", "Window Title Name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static RECT rt, isOne;
	static int mx, my, moveLeft = 1, moveRight = 1, moveTop = -1, moveBottom = -1;
	static int startMove;
	static HINSTANCE hInstance;
	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rt);
		hInstance = hInst;
		SetTimer(hwnd, 1, 1, NULL);
		isOne.left = 393, isOne.right = 393 - 50;
		isOne.bottom = 473 + 50, isOne.top = 473;
		break;
	case WM_TIMER:
		if (isStartMove == 1) {
			isOne.left += moveLeft, isOne.right += moveRight;
			isOne.top += moveTop, isOne.bottom += moveBottom;

			if (rt.right - 50 <= isOne.right) {


				if (startMove == 3) {
					startMove = 0;
					moveLeft = -1, moveRight = -1, moveTop = 1, moveBottom = 1;
				}
				else {
					moveLeft = -1, moveRight = -1, moveTop = -1, moveBottom = -1;
				}


			}
			else if (rt.top == isOne.top) {
				if (1 == startMove) {
					startMove = 3;
					moveLeft = 1, moveRight = 1, moveTop = 1, moveBottom = 1;
				}
				else {
					startMove = 2;
					moveLeft = -1, moveRight = -1, moveTop = 1, moveBottom = 1;
				}
			}
			else if (rt.left + 50 == isOne.left) {

				//startMove =1;
				moveLeft = 1, moveRight = 1, moveTop = -1, moveBottom = -1;
				if (startMove == 2) {
					startMove = 1;
					moveLeft = 1, moveRight = 1, moveTop = 1, moveBottom = 1;
					//moveLeft=-1, moveRight=-1, moveTop=1, moveBottom=1;
				}
			}


			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONDOWN:
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Dlg6_1Proc);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, isOne.left, isOne.top, isOne.right, isOne.bottom); //
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK Dlg6_1Proc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static RECT rt, isOne;
	static int mx, my, moveLeft = 1, moveRight = 1, moveTop = 1, moveBottom = 1;
	static int startMove;
	static int left = 0, top = 0, right = 50, bottom = 50;
	static HINSTANCE hInstance;
	switch (iMsg) {

	case WM_CREATE:
		GetClientRect(hwnd, &rt);
		hInstance = hInst;
		//SetTimer(hwnd,1,1,NULL);
		isOne.left = 0, isOne.right = 30;
		isOne.bottom = 30, isOne.top = 0;
		break;

	case WM_TIMER:

		left += moveLeft, right += moveRight;
		top += moveTop, bottom += moveBottom;

		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, left, top, right, bottom); //
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: // 
			isStartMove = 1;
			break;
		case IDC_BUTTON2: // 
			isStartMove = 0;
			break;
		case IDC_BUTTON3: // 
			EndDialog(hwnd, 0);
			break;
		case IDC_BUTTON6: // 
			EndDialog(hwnd, 0);
			break;
		case IDC_BUTTON4: // 
			SetTimer(hwnd, 1, 1, NULL);
			break;
		case IDC_BUTTON5: // 
			KillTimer(hwnd, 1);
			break;
		}

		break;



	}
	return 0;
}