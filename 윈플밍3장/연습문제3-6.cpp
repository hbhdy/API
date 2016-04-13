#include <Windows.h>
#include <math.h>
#include <time.h>

HINSTANCE g_hlnst;
LPCTSTR lpszClass = "window program 1 - 2";
#define BSIZE 50
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
		0,0,1200,800,
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
	int X;
	int Y;
}Circle;
typedef struct Hurdle {
	int X;
	int Y;
	bool flag;
}Hurdle;
LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush, oldBrush;
	static Circle pick;
	static Hurdle hurdle[20];  //·£´ý °´Ã¼
	static int h_size[20]; // ·£´ý °´Ã¼ÀÇ ·£´ý »çÀÌÁî
	static int mx, my;
	static bool select = false;
	static bool hurdle_pick;
	static int count = 0;
	static int num = -1;
	char memo[50];
	static bool collision;
	srand((unsigned int)time(NULL));
	switch (iMessage)
	{
	case WM_CREATE:
		pick.X = 0;
		pick.Y = 0;
		for (int i = 0; i < 20; i++)
		{
			h_size[i] = rand() % 60 + 5;
		}
		for (int i = 0; i < 20; i++)
		{
			hurdle[i].X = rand() % 1000;
			hurdle[i].Y = rand() % 700;
			hurdle[i].flag = true;
		}
		//SetTimer(hWnd, 1, 100, NULL);
		break;
	case WM_LBUTTONDOWN:
		mx = HIWORD(lParam);
		my = LOWORD(lParam);
		if (pick.X, pick.Y, mx, my)
		{
			select = true;
		};
		
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_LBUTTONUP:
		select = false;
		InvalidateRgn(hWnd, NULL, true);
		break;
	case WM_MOUSEMOVE:
		hDC = GetDC(hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (select)
		{
			pick.X= mx;
			pick.Y = my;
			InvalidateRgn(hWnd, NULL, true);
		}
		for (int i = 0; i < 20; i++) {
			if (InCircle(pick.X, pick.Y, hurdle[i].X, hurdle[i].Y))
			{
				collision = true;
				num = i;
			}
		}
		//InvalidateRgn(hWnd, NULL, true);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		Ellipse(hDC, pick.X - BSIZE, pick.Y - BSIZE, pick.X + BSIZE, pick.Y + BSIZE);
		for (int i = 0; i < 20; i++)
		{
			if (hurdle[i].flag == true)
			{
				hBrush = CreateSolidBrush(RGB(0, 255, 0));
			}
			else {
				hBrush = CreateSolidBrush(RGB(0, 0, 255));
			}
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, hurdle[i].X - h_size[i], hurdle[i].Y - h_size[i], hurdle[i].X + h_size[i], hurdle[i].Y + h_size[i]);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			
		}
		//if (collision) {
			if (hurdle[num].flag == true)
				count += 1;
			hurdle[num].flag = false;
		//}

	
		wsprintf(memo, TEXT("Ãæµ¹ÇÑ ¿øÀÇ °³¼ö´Â %d°³ ÀÔ´Ï´Ù."), count);
		TextOut(hDC, 600, 400, memo, strlen(memo));


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