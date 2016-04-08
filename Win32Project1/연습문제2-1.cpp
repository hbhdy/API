#include <Windows.h>

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
		0,0,800,600,
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
	char LT[] = "LeftTop(0,0)";
	char LB[] = "LeftBottom(0,600)";
	char RT[] = "RightTop(800,0)";
	char RB[] = "RightBottom(800,600)";
	char C[] = "Center(400,300)";
	int lt_x = 0, lt_y = 0;
	int lb_x=0, lb_y = 546;
	int rt_x = 682, rt_y = 0;
	int rb_x = 645, rb_y = 546;
	int c_x = 370, c_y = 270;

	switch (iMessage)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SetBkColor(hDC, RGB(255, 0, 0));
		SetTextColor(hDC, RGB(0, 255, 255));
		TextOut(hDC, lt_x, lt_y, LT,strlen(LT));
		SetBkColor(hDC, RGB(0, 255, 0));
		SetTextColor(hDC, RGB(255, 0, 255));
		TextOut(hDC, lb_x, lb_y, LB, strlen(LB));
		SetBkColor(hDC, RGB(0, 0, 255));
		SetTextColor(hDC, RGB(255, 255, 0));
		TextOut(hDC, rt_x, rt_y,RT,strlen(RT));
		SetBkColor(hDC, RGB(200, 30, 100));
		SetTextColor(hDC, RGB(55, 225, 155));
		TextOut(hDC, rb_x, rb_y, RB,strlen(RB));
		SetBkColor(hDC, RGB(130, 80, 255));
		SetTextColor(hDC, RGB(125, 175, 0));
		TextOut(hDC, c_x, c_y,C,strlen(C));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}