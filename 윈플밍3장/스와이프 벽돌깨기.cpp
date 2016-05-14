#include <Windows.h>
#include <math.h>
#include <random>
#include <iostream>
#include <ctime>
#define BSIZE 15
#define Max_Ball 100
#define Max_Block_Width 6
#define Max_Block_Height 20
using namespace  std;

BOOL InCircle(double x, double y, double mx, double my)
{
	if (sqrt((float)((mx - x)*(mx - x) + (my - y)*(my - y))) < BSIZE)
		return TRUE;
	else
		return FALSE;
}


HINSTANCE g_hlnst;
LPCTSTR lpszClass = "스와이프 벽돌깨기";

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM IParam);

int WINAPI WinMain(HINSTANCE hlnstance, HINSTANCE hPrevlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hlnst = hlnstance;
	RECT clientRect = { 0,0,600,800 };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);
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
		0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
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
	double x;
	double y;
	int flag;
	bool move;
	int wait = 0;
}Circle;

typedef struct Block {
	double L;
	double T;
	double R;
	double B;
	int count;
	int flag;
}Block;

enum { LEFT = -1, UP = 1, DOWN = -1, RIGHT = 1 }; // 공의 방향 체크 및 충돌후 방향 설정


												  //   가로 벽돌 6칸 세로 벽돌 10 칸    가로칸 100픽셀 세로칸 60픽셀 - > 600 X 600
												  //  세로 탑 100 세로 바텀 100  그 중간에 그리기 

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	default_random_engine dre;
	uniform_int_distribution <> ui(0, 255);
	dre.seed(2012181042);

	PAINTSTRUCT ps;
	HDC hDC;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	static Block block[20][Max_Block_Width]; //블럭의 화면상 총 갯수 
	static Circle ball[Max_Ball]; // 공의 예상 최대치
	static Circle rball[20][Max_Block_Width];
	static int mx = 0, my = 0; // 마우스 위치
	static double yspeed;
	static double xspeed;
	static int ball_stateX1[Max_Ball];
	static int ball_stateX2[Max_Ball];
	static int ball_stateY[Max_Ball];
	static int pick_count = 0;  // 클릭 및 발사 횟수 
	static int ball_end_count = 0;
	static bool now_state = false;

	double speed = 5.0;
	static int ball_count = 1; // 공의 갯수

	char name[30];
	char shotball[30];
	srand((unsigned int)time(NULL));

	switch (iMessage)
	{
	case WM_CREATE:
		for (int i = 0; i < Max_Ball; ++i) {
			ball[i].x = 300;
			ball[i].y = 685;
			ball[i].move = false;
			ball[i].wait = i + (i * 3);
		}
		for (int i = 0; i < Max_Block_Width; ++i) {
			block[0][i].flag = rand() % 2;
		}
		for (int i = 0; i < Max_Block_Width; ++i) {
			if (block[0][i].flag == 1)
			{
				block[0][i].count = 1;
				block[0][i].L = i * 100;
				block[0][i].R = (i + 1) * 100;
				block[0][i].T = 0;
				block[0][i].B = 60;
				rball[0][i].x = -30;
				rball[0][i].y = -30;
			}
			if (block[0][i].flag == 0)
			{
				rball[0][i].flag = 1;
				rball[0][i].x = 50 * i + 50 * (i + 1);
				rball[0][i].y = 30;
			}
		}
		//SetTimer(hWnd, 3, 1, NULL);
		break;

	case WM_LBUTTONDOWN:
		pick_count++;
		for (int i = 0; i < Max_Ball; ++i) {
			ball_stateY[i] = UP;
		}
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (mx > 300) {
			for (int i = 0; i < ball_count; ++i) {
				ball_stateX1[i] = 1;
				ball_stateX2[i] = 0;
			}
		}
		if (mx < 300) {
			for (int i = 0; i < ball_count; ++i) {
				ball_stateX1[i] = 0;
				ball_stateX2[i] = -1;
			}
		}

		xspeed = (ball[0].x - mx) / 50;
		yspeed = (ball[0].y - my) / 50;

		for (int i = 0; i < ball_count; ++i) {
			ball[i].move = true;
		}
		InvalidateRect(hWnd, NULL, true);
		break;

	case  WM_LBUTTONUP:
		SetTimer(hWnd, 1, 1, NULL);
		SetTimer(hWnd, 2, 1, NULL);
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (mx > 300) {
				for (int i = 0; i < ball_count; ++i) {
					if (ball[i].move == true && ball[i].wait == 0) {
						ball[i].x -= xspeed*ball_stateX1[i];
					}
					else {
						if (ball[i].move == true)
							ball[i].wait -= 1;
					}
				}
			}

			if (mx < 300) {
				for (int i = 0; i < ball_count; ++i) {
					if (ball[i].move == true && ball[i].wait == 0) {
						ball[i].x += xspeed*ball_stateX2[i];
					}
					else {
						if (ball[i].move == true)
							ball[i].wait -= 1;
					}
				}
			}

			for (int i = 0; i < ball_count; ++i) {
				if (ball[i].move == true && ball[i].wait == 0)
					ball[i].y -= yspeed*ball_stateY[i];
			}
			break;

		case 2: // 충돌체크
			for (int i = 0; i < ball_count; ++i) {
				if (ball[i].x <= 15) {
					ball_stateX1[i] = RIGHT;
					ball_stateX2[i] = RIGHT;
				}
				if (ball[i].x >= 585) {
					ball_stateX1[i] = LEFT;
					ball_stateX2[i] = LEFT;
				}
				if (ball[i].y <= 15) {
					ball_stateY[i] = DOWN;
				}
			}
			for (int j = 0; j <= pick_count; j++) {
				for (int i = 0; i < Max_Block_Width; ++i) {
					for (int b = 0; b < ball_count; ++b) {
						if (ball[b].y - BSIZE <= rball[j][i].y + BSIZE && (ball[b].x >= rball[j][i].x - BSIZE && ball[b].x <= rball[j][i].x + BSIZE) && rball[j][i].flag != 0) {
							// 볼 충돌
							rball[j][i].flag = 0;
							rball[j][i].x = -30;
							rball[j][i].y = -30;
							if (Max_Ball - 1 >= ball_count)
								ball_count++;
						}
						//if (ball[b].y - BSIZE <= block[j][i].B && ball[b].y + BSIZE >= block[j][i].T && (ball[b].x <= block[j][i].R && ball[b].x >= block[j][i].L) && block[j][i].count < 2) {
						//	// 윗면 충돌
						//	block[j][i].flag = 0;
						//	block[j][i].count++;
						//	if (ball_stateY[b] ==DOWN)
						//		ball_stateY[b] = UP;
						//}
						if (ball[b].y - BSIZE <= block[j][i].B && (ball[b].x <= block[j][i].R && ball[b].x >= block[j][i].L) && block[j][i].count < 2 && block[j][i].flag != 0) {
							// 밑면 충돌

							block[j][i].flag = 0;
							block[j][i].count++;
							if (ball_stateY[b] == UP)
								ball_stateY[b] = DOWN;
						}
						if (ball[b].x + BSIZE == block[j][i].L && (ball[b].y >= block[j][i].T &&ball[b].y <= block[j][i].B) && block[j][i].count < 2 && block[j][i].flag != 0) {
							// 왼쪽면 충돌
							block[j][i].flag = 0;
							block[j][i].count++;
							if (ball_stateX1[b] == RIGHT || ball_stateX2[b] == RIGHT) {
								ball_stateX1[b] = LEFT;
								ball_stateX2[b] = LEFT;
							}
						}
						if (ball[b].x - BSIZE == block[j][i].R && (ball[b].y >= block[j][i].T &&ball[b].y <= block[j][i].B) && block[j][i].count < 2 && block[j][i].flag != 0 ) {
							// 오른쪽면 충돌
							block[j][i].flag = 0;
							block[j][i].count++;
							if (ball_stateX1[b] == LEFT || ball_stateX2[b] == LEFT) {
								ball_stateX1[b] = RIGHT;
								ball_stateX2[b] = RIGHT;
							}
						}
					}
				}
			}
			for (int i = 0; i < ball_count; ++i) {
				if (ball[i].y >= 686) {
					ball[i].y = 685;
					ball[i].move = false;
					ball[i].wait = i + (i * 3);
				}
			}

			ball_end_count = 0;
			for (int i = 1; i < ball_count; ++i) {
				if (ball[0].move == false && ball[i].move == false) {
					ball[i].x = ball[0].x;
					ball[i].wait = i + (i * 3);
					ball_end_count++;
				}
			}
			if ((ball_end_count == ball_count - 1) && ball_count != 1) {
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);

				for (int j = 0; j <pick_count; j++) {
					for (int i = 0; i < Max_Block_Width; ++i) {
						if (block[j][i].flag == 1) {
							block[j][i].T += 60;
							block[j][i].B += 60;
							rball[j][i].x = -30;
							rball[j][i].y = -30;
						}
						if (block[j][i].flag == 0)
						{
							rball[j][i].y += 60;
						}
					}
				}

				
				SetTimer(hWnd, 3, 1, NULL);
			}

			break;
		case 3:
				for (int i =0; i < Max_Block_Width; ++i) {
					block[pick_count][i].flag = rand() % 2;
				}
				for (int i = 0; i < Max_Block_Width; ++i) {
					if (block[pick_count][i].flag == 1)
					{
						block[pick_count][i].count = 1;
						block[pick_count][i].L = i * 100;
						block[pick_count][i].R = (i + 1) * 100;
						block[pick_count][i].T = 0;
						block[pick_count][i].B = 60;
						rball[pick_count][i].x = -30;
						rball[pick_count][i].y = -30;
					}
					if (block[pick_count][i].flag == 0)
					{
						rball[pick_count][i].flag = 1;
						rball[pick_count][i].x = 50 * i + 50 * (i + 1);
						rball[pick_count][i].y = 30;
					}
				}
				KillTimer(hWnd, 3);
			break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hDC, hPen);
		MoveToEx(hDC, 0, 700, NULL);
		LineTo(hDC, 600, 700);
		SelectObject(hDC, oldPen);
		DeleteObject(hPen);
		for (int j = 0; j < pick_count; j++) {
			for (int i = 0; i < Max_Block_Width; i++) {
				hBrush = CreateSolidBrush(RGB(255, 100, 0));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, block[j][i].L, block[j][i].T, block[j][i].R, block[j][i].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
				hBrush = CreateSolidBrush(RGB(0, 255, 100));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, rball[j][i].x - BSIZE, rball[j][i].y - BSIZE, rball[j][i].x + BSIZE, rball[j][i].y + BSIZE);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}
		}
		for (int j = 0; j < pick_count; j++) {
			for (int i = 0; i < Max_Block_Width; ++i) {
				if (block[j][i].flag == 0) {
					block[j][i].L = 50;
					block[j][i].R = 50;
					block[j][i].T = 50;
					block[j][i].B = 50;
				}
			}
		}

		for (int i = ball_count - 1; i >= 0; --i) {
			hBrush = CreateSolidBrush(RGB(0, 50, 255));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, ball[i].x - BSIZE, ball[i].y - BSIZE, ball[i].x + BSIZE, ball[i].y + BSIZE);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}

		wsprintf(name, TEXT("스와이프 벽돌 깨기"));
		wsprintf(shotball, TEXT("발사할 공 갯수 : %d개"), ball_count);
		TextOut(hDC, 10, 730, name, strlen(name));
		TextOut(hDC, 10, 770, shotball, strlen(shotball));

	/*	for (int i = 0; i < 20; ++i) {    // 오류 검사용  후.. 넘많이 잡힌다
			for (int j = 0; j < 6; ++j) {

				wsprintf(shotball, TEXT("%d"), block[i][j].count);
				TextOut(hDC, 10+(j*20), 200+(i*20), shotball, strlen(shotball));
			}
		}*/
		
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