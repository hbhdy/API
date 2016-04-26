#include <Windows.h>
#include <math.h>
#define BSIZE 40

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
	static Circle cir[2];
	static double x, y;
	static double mx, my;
	static bool select = false;
	static int degree[4];
	static bool collision = false;

	switch (iMessage)
	{
	case WM_CREATE:
		degree[0] = 0;
		degree[1] = 90;
		degree[2] = 180;
		degree[3] = 270;
		cir[0].X = 400;
		cir[0].Y = 400;
		cir[1].X = 0;
		cir[1].Y = 0;
		KillTimer(hWnd, 1);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (select == false) {
		if (InCircle(cir[1].X, cir[1].Y, mx, my))
		{
			select = true;
		}
	}
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_LBUTTONUP:
		select = false;
		InvalidateRgn(hWnd, NULL, true);
		break;
	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (select)
		{
			cir[1].X = mx;
			cir[1].Y = my;
			InvalidateRgn(hWnd, NULL, true);
		}
		if (InCircle(cir[0].X, cir[0].Y, cir[1].X, cir[1].Y))
		{
			collision = true;
			SetTimer(hWnd, 1, 10, NULL);
		}
		//InvalidateRgn(hWnd, NULL, true);
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			
			for (int i = 0; i < 4;i++)
				degree[i] += 5;
		}
		InvalidateRgn(hWnd, NULL, true);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if (collision == false)
		{
			hBrush = CreateSolidBrush(RGB(0, 200, 255));
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(255, 0, 255));
		}
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, cir[0].X - BSIZE, cir[0].Y - BSIZE, cir[0].X + BSIZE, cir[0].Y + BSIZE);
		
		
		if (collision == true)
		{
			for (int i = 0; i < 4; i++)
			{
				MoveToEx(hDC, cir[0].X, cir[0].Y, NULL);
				LineTo(hDC, cir[0].X + (sin(degree[i] * 3.14 / 180) * 40), cir[0].Y + (cos(degree[i] * 3.14 / 180) * 40));
			}
		}
		else if (collision == false)
		{
			for (int i = 0; i < 4; i++)
			{
				MoveToEx(hDC, cir[0].X, cir[0].Y, NULL);
				LineTo(hDC, cir[0].X + (sin(degree[i] * 3.14 / 180) * 40), cir[0].Y + (cos(degree[i] * 3.14 / 180) * 40));
			}
		}

		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, cir[1].X - BSIZE, cir[1].Y - BSIZE, cir[1].X + BSIZE, cir[1].Y + BSIZE);

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