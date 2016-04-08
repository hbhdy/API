#include <Windows.h>
#include <time.h>
#include <stdio.h>

HINSTANCE g_hlnst;
LPCTSTR lpszClass = "Window";

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
	int L;
	int R;
	int T;
	int B;
}Circle;
LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	HDC hDC;
	static Circle bug[2];
	static Circle Mini_bug;
	static bool speed_P = true;
	static int key, mkey = 0;
	static int speed = 50;
	static int Bugspeed = 85;
	static int BugTime = 0;  // 애벌레 시간
	static int JumpTime = 0;  // 애벌레 쫓아오는 애
	/*static int main = 0, maintwo = 0;
	static int test = 0;
	static char Debug[50];*/
	static bool start = false;
	srand((unsigned int)time(NULL));
	switch (iMessage)
	{
	case WM_CREATE:
		//GetClientRect(hWnd, &rect);
		bug[0].L = 400;
		bug[0].T = 400;
		bug[0].R = 450;
		bug[0].B = 450;
		bug[1].L = 450;
		bug[1].T = 410;
		bug[1].R = 480;
		bug[1].B = 440;

		Mini_bug.L = 700;
		Mini_bug.T = 700;
		Mini_bug.R = 750;
		Mini_bug.B = 750;
		break;
	case WM_CHAR:
		if (wParam == 's') // 게임 시작
		{
			/*KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, speed, NULL);
			KillTimer(hWnd, 2);
			SetTimer(hWnd, 2, 100, NULL);*/
			SetTimer(hWnd, 1, speed, NULL);
			SetTimer(hWnd, 2, Bugspeed, NULL);
			SetTimer(hWnd, 5, 1000,NULL);
			start = true;
		}
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
			KillTimer(hWnd, 2);
			SetTimer(hWnd, 1, speed, NULL);
			SetTimer(hWnd, 2, Bugspeed, NULL);
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
			KillTimer(hWnd, 2);
			SetTimer(hWnd, 1, speed, NULL);
			SetTimer(hWnd, 2, Bugspeed, NULL);
		}
		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			key = 1;
			mkey = 1;
		}
		else if (wParam == VK_UP) {
			key = 2;
			mkey = 2;
		}
		else if (wParam == VK_DOWN) {
			key = 3;
			mkey = 3;
		}
		else if (wParam == VK_RIGHT) {
			key = 4;
			mkey = 4;
		}
		else if (wParam == VK_SPACE) {
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			SetTimer(hWnd, 3, speed, NULL);
			
		}
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_TIMER:
		/*test++;*/
		switch (wParam) {
		case 1:
			/*main++;*/
			if (key == 1) // LEFT
			{
				bug[1].R = bug[0].R + 30;
				bug[1].L = bug[0].L + 50;
				bug[1].T = bug[0].T + 40;
				bug[1].B = bug[0].B - 40;
				bug[0].L -= 10;
				bug[1].L -= 10;
				bug[0].R -= 10;
				bug[1].R -= 10;
				if (bug[0].L <= 0)
				{
					key = 4;
				}
			}
			if (key == 2) // UP
			{
				bug[1].R = bug[0].R - 40;
				bug[1].L = bug[0].L + 40;
				bug[1].T = bug[0].T + 50;
				bug[1].B = bug[0].B + 30;
				bug[0].T -= 10;
				bug[1].T -= 10;
				bug[0].B -= 10;
				bug[1].B -= 10;
				if (bug[0].T <= 0)
					key = 3;
			}
			if (key == 3) // DOWN
			{
				bug[1].R = bug[0].R - 40;
				bug[1].L = bug[0].L + 40;
				bug[1].T = bug[0].T - 30;
				bug[1].B = bug[0].B - 50;
				bug[0].T += 10;
				bug[1].T += 10;
				bug[0].B += 10;
				bug[1].B += 10;
				if (bug[0].B >= 800)
					key = 2;
			}
			if (key == 4) // RIGHT
			{
				bug[1].R = bug[0].R - 50;
				bug[1].L = bug[0].L - 30;
				bug[1].T = bug[0].T + 40;
				bug[1].B = bug[0].B - 40;
				bug[0].L += 10;
				bug[1].L += 10;
				bug[0].R += 10;
				bug[1].R += 10;
				if (bug[0].R >= 1200)
					key = 1;
			}
			break;
		case 2:
			/*maintwo++;*/
			if (mkey == 1) // LEFT
			{
				Mini_bug.L -= 10;
				Mini_bug.R -= 10;
				if (Mini_bug.L <= 0)
				{
					mkey = 4;
				}
				if (bug[0].L - Mini_bug.L > 0)
					mkey = 4;
			}
			if (mkey == 2) // UP
			{
				Mini_bug.T -= 10;
				Mini_bug.B -= 10;
				if (Mini_bug.T <= 0)
					mkey = 3;
				if (bug[0].T - Mini_bug.T > 0)
					mkey = 3;

			}
			if (mkey == 3) // DOWN
			{
				Mini_bug.T += 10;
				Mini_bug.B += 10;
				if (Mini_bug.B >= 800)
					mkey = 2;
				if (bug[0].T - Mini_bug.T < 0)
					mkey = 2;
			}
			if (mkey == 4) // RIGHT
			{
				Mini_bug.L += 10;
				Mini_bug.R += 10;
				if (Mini_bug.R >= 1200)
					mkey = 1;
				if (bug[0].L - Mini_bug.L < 0)
					mkey = 1;
			}
			break;
		case 3:
			if (JumpTime >= 5) {
				KillTimer(hWnd, 3);
				SetTimer(hWnd, 4, speed, NULL);
			}
			else {
				if (key == 1 || key == 4)
				{
					bug[0].T -= 10;
					bug[1].T -= 10;
					bug[0].B -= 10;
					bug[1].B -= 10;
					Mini_bug.T -= 10;
					Mini_bug.B -= 10;
					++JumpTime;
				}
				else {
					bug[0].L += 10;
					bug[1].L += 10;
					bug[0].R += 10;
					bug[1].R += 10;
					Mini_bug.L += 10;
					Mini_bug.R += 10;
					++JumpTime;
				}
			}
			break;
		case 4:
			if (JumpTime <= 0) {
				KillTimer(hWnd, 4);
				SetTimer(hWnd, 1, speed, NULL);
				SetTimer(hWnd, 2, Bugspeed, NULL);
			}
			else {
				if (key == 1 || key == 4)
				{
					bug[0].T += 10;
					bug[1].T += 10;
					bug[0].B += 10;
					bug[1].B += 10;
					Mini_bug.T += 10;
					Mini_bug.B += 10;
					--JumpTime;
				}
				else
				{
					bug[0].L -= 10;
					bug[1].L -= 10;
					bug[0].R -= 10;
					bug[1].R -= 10;
					Mini_bug.L -= 10;
					Mini_bug.R -= 10;
					--JumpTime;
				}
			}
			break;
		case 5:
			BugTime++;
			break;
			
		break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if (start == true)
		{
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, bug[0].L, bug[0].T, bug[0].R, bug[0].B);
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, bug[1].L, bug[1].T, bug[1].R, bug[1].B);

			if (BugTime >= 3)
			{
				hBrush = CreateSolidBrush(RGB(0, 0, 255));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, Mini_bug.L, Mini_bug.T, Mini_bug.R, Mini_bug.B);
			}
			/*sprintf(Debug, "main : %d, mainTwo : %d / speed : %d, Bugspeed : %d / test : %d", main, maintwo, speed, Bugspeed, test);
			TextOut(hDC, 0, 0, Debug, strlen(Debug));*/

			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
			EndPaint(hWnd, &ps);	
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


