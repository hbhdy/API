#include <Windows.h>
#include <time.h>
#include <math.h>

HINSTANCE g_hlnst;
LPCTSTR lpszClass = "window program 1 - 2";
#define BSIZE 40

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
	double X;
	double Y;
}Circle;
typedef struct Hurdle {
	double X;
	double Y;
	bool flag;
	int col;
}Hurdle;

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush, oldBrush;
	static Circle pick;
	static Hurdle hurdle[20];
	static double mx, my;
	static int h_size[20];
	static int degree_T[4];
	static int degree_F[4];
	static int count = 0;
	static int num = -1;
	static bool select = false;
	static bool collision = false;
	char memo[50];

	srand((unsigned)time(NULL));

	switch (iMessage)
	{
	case WM_CREATE:
		degree_T[0] = 0;
		degree_T[1] = 90;
		degree_T[2] = 180;
		degree_T[3] = 270;
		degree_F[0] = 0;
		degree_F[1] = 90;
		degree_F[2] = 180;
		degree_F[3] = 270;

		pick.X = 0;
		pick.Y = 0;
		for (int i = 0; i < 20; i++) {
			hurdle[i].X = rand() % 1100;
			hurdle[i].Y = rand() % 700;
			hurdle[i].flag = true;
			hurdle[i].col = 0;
		}
		for (int i = 0; i < 20; i++)
			h_size[i] = rand() % 50 +15;

		break;

	case WM_LBUTTONDOWN:
		mx = HIWORD(lParam);
		my = LOWORD(lParam);
		if (select == false) {
			if (InCircle(pick.X, pick.Y, mx, my))
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
		//hDC = GetDC(hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (select)
		{
			pick.X = mx;
			pick.Y = my;
			InvalidateRgn(hWnd, NULL, true);
		}
		for (int i = 0; i < 20; i++) {
			if (InCircle(pick.X, pick.Y, hurdle[i].X, hurdle[i].Y))
			{
				collision = true;
				num = i;
				if (hurdle[num].flag == true)
				{
					hurdle[i].col += 1;
					count += 1;
				}
				hurdle[num].flag = false;
				SetTimer(hWnd, 1, 10, NULL);
			}
		}
		collision = false;
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:

			for (int i = 0; i < 4; i++)
				degree_T[i] += 3;
		}
		InvalidateRgn(hWnd, NULL, true);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, pick.X - BSIZE, pick.Y - BSIZE, pick.X + BSIZE, pick.Y + BSIZE);
		for (int i = 0; i < 20; i++)
		{
			if (hurdle[i].flag == true)
			{
				hBrush = CreateSolidBrush(RGB(0, 255, 0));
			}
			else {
				hBrush = CreateSolidBrush(RGB(0, 200, 255));
			}
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, hurdle[i].X - h_size[i], hurdle[i].Y - h_size[i], hurdle[i].X + h_size[i], hurdle[i].Y + h_size[i]);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (hurdle[j].col>=1)
				{
					MoveToEx(hDC, hurdle[j].X, hurdle[j].Y, NULL);
					LineTo(hDC, hurdle[j].X + (sin(degree_T[i] * 3.14 / 180) * h_size[j]), hurdle[j].Y + (cos(degree_T[i] * 3.14 / 180) * h_size[j]));
				}
				else if(hurdle[j].col==0)
				{
					MoveToEx(hDC, hurdle[j].X, hurdle[j].Y, NULL);
					LineTo(hDC, hurdle[j].X + (sin(degree_F[i] * 3.14 / 180) * h_size[j]), hurdle[j].Y + (cos(degree_F[i] * 3.14 / 180) * h_size[j]));
				}
			}
			for (int w = 10; w < 20; w++)
			{
				if (hurdle[w].col >= 1)
				{
					MoveToEx(hDC, hurdle[w].X, hurdle[w].Y, NULL);
					LineTo(hDC, hurdle[w].X + (cos(degree_T[i] * 3.14 / 180) * h_size[w]), hurdle[w].Y + (sin(degree_T[i] * 3.14 / 180) * h_size[w]));
				}
				else if (hurdle[w].col == 0)
				{
					MoveToEx(hDC, hurdle[w].X, hurdle[w].Y, NULL);
					LineTo(hDC, hurdle[w].X + (cos(degree_F[i] * 3.14 / 180) * h_size[w]), hurdle[w].Y + (sin(degree_F[i] * 3.14 / 180) * h_size[w]));
				}
			}
		}
		wsprintf(memo, TEXT("충돌한 원의 개수는 %d개 입니다."), count);
		TextOut(hDC, 600, 400, memo, strlen(memo));
		if(count>=20)
			PostQuitMessage(0);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}