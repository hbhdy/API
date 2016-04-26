//3 - 7
//화면의 상단에 2 * 10개의 벽돌이 있다.
//화면의 하단에 바가 있고 마우스를 이용하여 바를 움직인다.
//마우스를 누르면 마우스의 x 위치로 이동한다.
//공이 튀기면서 벽돌에 1번 닿으면 벽돌의 색이 바뀐다.
//공이 튀기면서 별돌에 2번 닿으면 벽돌이 없어진다.
//색이 변한 벽돌의 개수와 없어진 개수를 화면에 출력한다
//명령어 입력 : 공의 이동속도가 늘어난다.
//벽돌이 모두 없어지면 게임에 종료한다.
#include <Windows.h>
#include <math.h>
#define BSIZE 20

float LengthPts(int x1, int y1, int x2, int y2)
{
	return(sqrt((float)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))));
}

BOOL InRectangle(int x, int y, int mx, int my)
{
	if (LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}

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
typedef struct Circle {
	double X;
	double Y;
}Circle;

typedef struct Block {
	double L;
	double T;
	double R;
	double B;
	int count;
	int flag;
}Block;

enum { LEFT = 1, UP = 1, DOWN = -1, RIGHT = -1 }; // 공의 방향 체크 및 충돌후 방향 설정

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush, oldBrush;
	static RECT rect;
	static Block block[20];
	static Circle circle;
	static int speed = 100;
	static int bar_x, bar_y;
	static int ball_DX = 1;  // 볼의 direction x  ( 방향 x  값)
	static int ball_DY = 1;  // 방향 y 값
	static int mx, my;
	static int block_level = 0;
	static int c_block, d_block = 0;
	char changeblock[30];
	char deleteblock[30];
	char speed_check[30];
	
	static bool select = false;

	switch (iMessage)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		bar_x = rect.right / 2;
		bar_y = rect.bottom - 30;
		circle.X = rect.right / 2;
		circle.Y = rect.bottom - 70;
		for (int i = 0; i < 20; i++)
		{
			if (i!=0 && i%10==0)
				block_level += 20;
			block[i].L = (rect.right / 10)*(i%10);
			block[i].T = rect.top + block_level;
			block[i].R = (rect.right / 10)*((i%10)+1);
			block[i].B = rect.top + block_level + 20;
			block[i].count = 0;
			block[i].flag = 0;
		}
		SetTimer(hWnd, 1, speed, NULL);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InRectangle(bar_x, bar_y, mx, my))
			select = true;

		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_CHAR:
		if (wParam == 'p')
			speed -= 10;
		if (wParam == 'm')
			speed += 10;

		KillTimer(hWnd, 1);
		SetTimer(hWnd, 1, speed, NULL);
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_LBUTTONUP:
		select = false;
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (select == true) {
			bar_x = mx;
			InvalidateRect(hWnd, NULL, true);
		}
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			circle.X -= 20 * ball_DX;
			circle.Y -= 20 * ball_DY;
			if (circle.X - 10 <= rect.left)
				ball_DX = RIGHT;  //-1
			else if (circle.Y - 10 <= rect.top)
				ball_DY = DOWN; // -1
			else if (circle.X + 10 >= rect.right)
				ball_DX = LEFT; // 1
			else if (circle.Y + 10 >= bar_y-15)
				ball_DY = UP; // 1

			for (int i = 0; i < 20; i++)
			{
				if (circle.Y - 20 <= block[i].B && (circle.X <= block[i].R && circle.X >= block[i].L) && block[i].count < 2)
				{
					block[i].count += 1;
					block[i].flag += 1;
					if (ball_DY == 1)
						ball_DY = -1;
					else
						ball_DY = 1;
					if (block[i].flag == 1) {
						c_block++;
					}
					else if (block[i].flag == 2) {
						c_block -= 1;
						d_block++;
					}
				}
			}
			break;
		}
		if (d_block == 20)
			PostQuitMessage(0);
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		for (int i = 0; i < 20; i++)
		{
			Rectangle(hDC, block[i].L, block[i].T, block[i].R, block[i].B);
			if (block[i].count == 1)
			{
				hBrush = CreateSolidBrush(RGB(255, 255, 0));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, block[i].L, block[i].T, block[i].R, block[i].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}
			else if (block[i].count == 2)
			{
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, block[i].L, block[i].T, block[i].R, block[i].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}
		}

		hBrush = CreateSolidBrush(RGB(255, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Rectangle(hDC, bar_x-2*BSIZE,bar_y-20,bar_x+2*BSIZE,bar_y);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		hBrush = CreateSolidBrush(RGB(255, 0, 255));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, circle.X - 10, circle.Y - 10, circle.X + 10, circle.Y + 10);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		wsprintf(changeblock, TEXT("바뀐 돌 : %d개"),c_block);
		wsprintf(deleteblock, TEXT("삭제 돌 : %d개"), d_block);
		wsprintf(speed_check, TEXT("스피드 : %d"), speed);

		TextOut(hDC, 10, 400, changeblock, strlen(changeblock));
		TextOut(hDC, 10, 420, deleteblock, strlen(deleteblock));
		TextOut(hDC, 10, 440, speed_check, strlen(speed_check));

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}