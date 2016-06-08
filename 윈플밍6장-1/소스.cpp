#include <Windows.h>
#include "resource.h"
#define BSIZE 20
HINSTANCE g_hlnst;
LPCTSTR lpszClass = "파워 코딩";
static bool state = false;
static bool figure = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT umsg, WPARAM wparam, LPARAM IParam);
BOOL CALLBACK Dlg1Proc(HWND, UINT, WPARAM, LPARAM);
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
		0, 0, 400, 400,
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


typedef struct BALL {
	int x;
	int y;
}BALL;

enum { LEFT = -1, UP = -1, DOWN = 1, RIGHT = 1 }; // 공의 방향 체크 및 충돌후 방향 설정

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static RECT rect;
	static HINSTANCE hInstance;
	static BALL ball;
	static int ball_DX = 1;  // 볼의 direction x  ( 방향 x  값)
	static int ball_DY = 1;  // 방향 y 값
	

	GetClientRect(hWnd, &rect);

	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 1, NULL);
		ball.x = 200;
		ball.y = 50;
		g_hlnst = hInstance;
		break;

	case WM_LBUTTONDOWN: // 마우스 클릭하면 대화상자 띄우기
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg1Proc);
		break;

	case WM_TIMER:
		if (state == true) {
			ball.x += 5 * ball_DX;  //  1
			ball.y += 5 * ball_DY;  //  1
			if (ball.x - BSIZE <= rect.left)
				ball_DX = RIGHT;  // 1
			if (ball.y - BSIZE <= rect.top)
				ball_DY = DOWN; // 1
			if (ball.x + BSIZE >= rect.right)
				ball_DX = LEFT; // 1
			if (ball.y + BSIZE >= rect.bottom)
				ball_DY = UP; // 1
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if (figure==false)
			Ellipse(hDC, ball.x - BSIZE, ball.y - BSIZE, ball.x + BSIZE, ball.y + BSIZE);
		if (figure == true)
			Rectangle(hDC, ball.x - BSIZE, ball.y - BSIZE, ball.x + BSIZE, ball.y + BSIZE);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK Dlg1Proc(HWND hWnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam) // 대화상자 메시지 처리함수
{
	HDC hDC;
	PAINTSTRUCT ps;
	static RECT rect;
	static HINSTANCE hInstance;
	static BALL ball;
	static int ball_DX1 = 0;  // 볼의 direction x  ( 방향 x  값)
	static int ball_DY1 = 0;  // 방향 y 값

	GetClientRect(hWnd, &rect);

	switch (iMsg) {
	case WM_CREATE:
		g_hlnst = hInstance;
		ball.x = 200;
		ball.y = 50;
		break;
	case WM_TIMER:
		ball.x += 5 * ball_DX1;  //  1
		ball.y += 5 * ball_DY1;  //  1
		if (ball.x - BSIZE <= rect.left)
			ball_DX1 = RIGHT;  // 1
		if (ball.y - BSIZE <= rect.top)
			ball_DY1 = DOWN; // 1
		if (ball.x + BSIZE >= rect.right)
			ball_DX1 = LEFT; // 1
		if (ball.y + BSIZE >= rect.bottom)
			ball_DY1 = UP; // 1

		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		//case IDOK: // 버튼
		//	EndDialog(hWnd, 0);
		//	break;
		//case IDCANCEL: // 버튼
		//	EndDialog(hWnd, 0);
		//	break;
		case IDC_BUTTON1:
			state = true;
			break;
		case IDC_BUTTON2:
			state = false;
			break;
		case IDC_BUTTON3: // 
			PostQuitMessage(0);
			break;
		case IDC_BUTTON4:
			SetTimer(hWnd, 1, 50, NULL);
			break;
		case IDC_BUTTON5: // 
			KillTimer(hWnd, 1);
			break;
		case IDC_BUTTON6: // 
			EndDialog(hWnd, 0);
			break;
		case IDC_RADIO1:
			figure = true;
			break;
		case IDC_RADIO2:
			figure = false;
			break;
		}
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		if (figure == false)
			Ellipse(hDC, ball.x - BSIZE, ball.y - BSIZE, ball.x + BSIZE, ball.y + BSIZE);
		if (figure == true)
			Rectangle(hDC, ball.x - BSIZE, ball.y - BSIZE, ball.x + BSIZE, ball.y + BSIZE);
		EndPaint(hWnd, &ps);
		break;

	}
	return 0;
}