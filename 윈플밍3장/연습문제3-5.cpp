#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
static double BSIZE = 20;
HINSTANCE g_hlnst;
LPCTSTR lpszClass = "window program 1 - 2";

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM IParam);


BOOL InCircle(double x, double y, double mx, double my)
{
	if (sqrt((float)((mx - x)*(mx - x) + (my - y)*(my - y)))<BSIZE)
		return TRUE;
	else 
		return FALSE;
}
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
		0, 0, 1200, 800,
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
typedef struct Circle {
	double X;
	double Y;
}Circle;
LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush, oldBrush;
	static Circle bug[7];
	static Circle meet[7];
	static Circle bomb[3];
	static int key = 0;
	static int speed = 100;
	static int JumpTime = 0;
	static int count = 1;
	static double mx, my = 0;
	srand((unsigned int)time(NULL));

	switch (iMessage)
	{
	case WM_CREATE:
		bug[0].X = 100;
		bug[0].Y = 100;
		bug[1].X = bug[0].X - 40;
		bug[1].Y = bug[0].Y;
		for (int i = 0; i < 7; i++)
		{
			meet[i].X = rand() % 1100;
			meet[i].Y = rand() % 700;
		}
		for (int i = 0; i < 3; i++)
		{
			bomb[i].X = rand() % 1100;
			bomb[i].Y = rand() % 700;
		}
		SetTimer(hWnd, 1, speed, NULL);
		break;
	case WM_CHAR:
		if (wParam == 'p')
		{
			//KillTimer(hWnd, 1);
			if (speed == 0) {
				speed = 10;
			}
			else {
				speed -= 10;
			}
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, speed, NULL);

		}
		if (wParam == 'm')
		{
			//KillTimer(hWnd, 1);
			if (speed == 130) {
				speed = 120;
			}
			else {
				speed += 10;
			}
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, speed, NULL);
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			key = 1;
		}
		else if (wParam == VK_UP) {
			key = 2;
		}
		else if (wParam == VK_DOWN) {
			key = 3;
		}
		else if (wParam == VK_RIGHT) {
			key = 4;
		}
		else if (wParam == VK_SPACE) {
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 2, speed, NULL);
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if ((my > bug[0].Y && mx < bug[0].X) ) {
			key = 3;
		}
		else if (my < bug[0].Y && bug[0].X < mx) {
			key = 2;
		}
		else if (mx > bug[0].X){
			key = 4;
		}
		else if (mx < bug[0].X){
			key = 1;
		}

		break;
	case WM_TIMER:
		for (int i = count; i != 0; --i) {
			bug[i] = bug[i - 1];
		}
		switch (wParam) {
		case 1:
			for (int i = 0; i < 7; i++) {
				if (InCircle(bug[0].X, bug[0].Y, meet[i].X, meet[i].Y)) {
					meet[i].X = 0;
					meet[i].Y = 0;
					count++;
				}
				else if (InCircle(bug[0].X, bug[0].Y, bomb[i].X, bomb[i].Y)) {
					bomb[i].X = 0;
					bomb[i].Y = 0;
					count--;
				}
			}

			if (key == 1) // LEFT
			{
				{
					bug[1].X = bug[0].X + 40;
					bug[1].Y = bug[0].Y;
					bug[0].X -= 40;
					bug[1].X -= 40;
					if (bug[0].X - 20 <= 0)
						key = 4;
				}
			}
			if (key == 2) // UP
			{
				bug[1].X = bug[0].X;
				bug[1].Y = bug[0].Y + 40;
				bug[0].Y -= 40;
				bug[1].Y -= 40;
				if (bug[0].Y - 20 <= 0)
					key = 3;
			}
			if (key == 3) // DOWN
			{
				bug[1].X = bug[0].X;
				bug[1].Y = bug[0].Y - 40;
				bug[0].Y += 40;
				bug[1].Y += 40;
				if (bug[0].Y + 20 >= 800)
					key = 2;
			}
			if (key == 4) // RIGHT
			{
				bug[1].X = bug[0].X - 40;
				bug[1].Y = bug[0].Y;
				bug[0].X += 40;
				bug[1].X += 40;
				if (bug[0].X + 20 >= 1200)
					key = 1;
			}
			break;
		case 2:
			if (JumpTime >= 4) {
				KillTimer(hWnd, 2);
				SetTimer(hWnd, 3, speed, NULL);
			}
			else {
				if (key == 1 || key == 4)
				{
					bug[0].Y -= 10;
					bug[1].Y -= 10;
					++JumpTime;
				}
				else {
					bug[0].X += 10;
					bug[1].X += 10;
					++JumpTime;
				}
			}
			break;
		case 3:
			if (JumpTime <= 0) {
				KillTimer(hWnd, 3);
				SetTimer(hWnd, 1, speed, NULL);
			}
			else {
				if (key == 1 || key == 4)
				{
					bug[0].Y += 10;
					bug[1].Y += 10;
					--JumpTime;
				}
				else
				{
					bug[0].X -= 10;
					bug[1].X -= 10;
					--JumpTime;
				}
			}
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 赣府
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, bug[0].X - BSIZE, bug[0].Y - BSIZE, bug[0].X + BSIZE, bug[0].Y + BSIZE);
		hBrush = CreateSolidBrush(RGB(0, 0, 255)); // 个烹
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, bug[1].X - BSIZE, bug[1].Y - BSIZE, bug[1].X + BSIZE, bug[1].Y + BSIZE);
		
		for (int i = 2; i <= count; ++i) { // 部府
			hBrush = CreateSolidBrush(RGB(0, 255, 255));   //
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			if (bug[i].X != 1)
				Ellipse(hDC, bug[i].X - BSIZE, bug[i].Y - BSIZE, bug[i].X + BSIZE, bug[i].Y + BSIZE);
		}
		for (int i = 0; i < 7; i++) {
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, meet[i].X - BSIZE, meet[i].Y - BSIZE, meet[i].X + BSIZE, meet[i].Y + BSIZE);
		}
	
		for (int i = 0; i < 3; i++) {
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, bomb[i].X - BSIZE, bomb[i].Y - BSIZE, bomb[i].X + BSIZE, bomb[i].Y + BSIZE);
		}
		if (count >= 7)
		{
			PostQuitMessage(0);
		}
		else if(count <= 0)
		{
			PostQuitMessage(0);
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