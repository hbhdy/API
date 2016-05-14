#include <Windows.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define LIMIT 10
#define PI 3.1415

typedef struct
{
	int xPos;
	int yPos;
	int size;
	int dir_X;
	int dir_Y;
	int color_R;
	int color_G;
	int color_B;
	int state;
	/*void move()
	{
	xPos += 20 * dir_X;
	yPos -= 20 * dir_Y;
	};*/

	BOOL Going;
	BOOL IS_CRUSH;
	BOOL Collision;
} _BALL;

typedef struct
{
	int xPos;
	int yPos;
	int height;
	int width;
} _BAR;

typedef struct
{
	int xPos;
	int yPos;
	int height;
	int width;
	int state;
	int color_R;
	int life;
	int creat_b;
	int U;
	BOOL IS_CHECK;
	BOOL P_Block;
} _BLOCK;

enum { LEFT = -1, UP = -1, RIGHT = 1, DOWN = 1 };
enum MODE { S_Mode, M_Mode };
enum { Slow = 80, Medium = 50, Fast = 20 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	HINSTANCE g_hInst;//
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = "API Programming";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	RECT rcWindow = { 0, 0, 840, 1000 };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, false);
	hWnd = CreateWindow("API Programming", "API Programming", WS_OVERLAPPEDWINDOW, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

BOOL IsPointInCircle(int cx, int cy, int cr, int px, int py)
{
	float delX = cx - px;
	float delY = cy - py;
	int length = sqrt(delX*delX + delY*delY);
	if (length > cr)
		return FALSE;
	return TRUE;
}

BOOL IsCollision(int cx, int cy, int cr, const RECT* rec)
{
	int nCx = static_cast<int>(cx);
	int nCy = static_cast<int>(cy);
	int nR = static_cast<int>(cr);

	if ((rec->left <= nCx&&nCx <= rec->right) || (rec->top <= nCy&&nCy <= rec->bottom))
	{
		RECT rcEx = {
			rec->left - nR,
			rec->top - nR,
			rec->right + nR,
			rec->bottom + nR
		};
		if ((rcEx.left < nCx&&nCx < rcEx.right) && (rcEx.top < nCy && nCy < rcEx.bottom))
		{
			return TRUE;
		}
	}
	else
	{
		if (IsPointInCircle(cx, cy, cr, (float)rec->left, (float)rec->top))
			return TRUE;
		if (IsPointInCircle(cx, cy, cr, (float)rec->left, (float)rec->bottom))
			return TRUE;
		if (IsPointInCircle(cx, cy, cr, (float)rec->right, (float)rec->top))
			return TRUE;
		if (IsPointInCircle(cx, cy, cr, (float)rec->right, (float)rec->bottom))
			return TRUE;
	}
	return FALSE;

}

BOOL CollisionCheck(RECT rc, RECT rc2)
{
	float rc_R = (rc.right - rc.left) / 2;
	float rc2_R = (rc2.right - rc2.left) / 2;

	float rX = (rc.left + (rc.right - rc.left) / 2) - (rc2.left + (rc2.right - rc2.left) / 2);
	float rY = (rc.top + (rc.bottom - rc.top) / 2) - (rc2.top + (rc2.bottom - rc2.top) / 2);
	float length = (rc_R + rc2_R)  * (rc_R + rc2_R);

	if (length >= (rX * rX + rY * rY))
	{
		return true;
	}
	return false;
}

double LengthPts(int x1, int y1, int x2, int y2)
{
	double a = ((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	return(sqrt(a));
}
BOOL InCircle(int x, int y, int mx, int my)
{
	if (LengthPts(x, y, mx, my) < 10) return TRUE;
	else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int st = 0;
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static double vx, vy;
	static RECT rectView;
	static int mouse_X, mouse_Y;
	static char str[100];
	static int LifeCount = 0;
	static int Block_C = 4;
	static int Count = 0;
	static int Liner = 0;
	static int Ball_Count1 = 1, Ball_Count2 = 1, Ball_Count3 = 1;
	static int Random;
	static float clock = 0;
	static int Ball_Init = 0;
	static int stop = 0;
	static int L_Count = 0;
	static int Ball_Time = 0;
	static RECT R1, R2;
	static RECT rect;
	static BOOL Drag;
	static int startX, startY, oldX, oldY, endX, endY;
	static int px1 = 0, py1 = 0, px2 = 0, py2 = 0;

	static int ex, ey, my, mx;
	//   static int L_Count = 0;
	static POINT point1, point2, point3, point4;
	static int LINE = 10;
	static _BAR Bar;
	static _BALL Ball[9999];
	static _BLOCK Block[LIMIT][7];
	static _BLOCK C_Block[LIMIT][7];
	static int Speed;
	static BOOL B_Down;
	static BOOL Click;
	static MODE Mode;                              // 'S', 'M' 모드 변수
	static int Block_delete_num;                     // 제거된 블럭의 갯수
	static int Block_change_color_num;                  // 남아있는 색바뀐 블럭의 갯수
	static int Time;                              // 게임플레이 시간
	static int map[LIMIT][7];                       // 블럭 맵   
	static int i_X;
	static int i_Y;
	srand((unsigned)time(NULL));

	switch (iMsg) {
	case WM_CREATE:
		srand((unsigned)NULL(time));
		Drag = FALSE;
		GetClientRect(hWnd, &rectView);
		Ball[0].xPos = 400;
		Ball[0].yPos = 760;                     // Ball 세부 속성
		Ball[0].size = 10;
		Ball[0].dir_X = 1;
		Ball[0].dir_Y = 1;
		Ball[0].color_R = 255;
		Ball[0].color_G = 128;
		Ball[0].color_B = 128;
		Ball[0].Going = TRUE;
		Ball[0].IS_CRUSH = FALSE;
		ex = Ball[0].xPos + Ball[0].size / 2;
		ey = Ball[0].yPos + Ball[0].size / 2;

		B_Down = TRUE;
		Bar.xPos = 0;                        // Bar 세부 속성
		Bar.yPos = 780;
		Bar.height = 30;
		Bar.width = rectView.bottom;
		LifeCount++;
		for (int i = LIMIT; i >= 0; --i)                              // Block 초기화
		{
			for (int j = 0; j < 7; ++j)
			{
				Block[i][j].xPos = 0 + 120 * (j);
				Block[i][j].yPos = 0 + 80 * LINE;
				Block[i][j].width = 120;
				Block[i][j].height = 80;
				Block[i][j].color_R = 255;
				Block[i][j].life = 0;
				Block[i][j].color_R = 255;
				Block[i][j].state = FALSE;
				Block[i][j].creat_b = 0;
				Block[i][j].U = 0;
				Block[i][j].IS_CHECK = FALSE;
				Block[i][j].P_Block = FALSE;


			}
			LINE--;
		}

		Block_delete_num = 0;
		Block_change_color_num = 0;
		Speed = Medium;                                    // 공의 시작 속도
		break;

	case WM_CHAR:
		if (wParam == 'q' || wParam == 'Q')
			Speed = Slow;
		else if (wParam == 'w' || wParam == 'W')
			Speed = Medium;
		else if (wParam == 'e' || wParam == 'E')
			Speed = Fast;
		else if (wParam == 'p' || wParam == 'P')
		{

			for (int i = LIMIT - 1; i > 0; --i)
			{
				for (int j = 0; j < 7; ++j)
				{
					Block[i][j].color_R = Block[i - 1][j].color_R;
					Block[i][j].life = Block[i - 1][j].life;
					Block[i][j].state = Block[i - 1][j].state;
					Block[i][j].creat_b = Block[i - 1][j].creat_b;
					Block[i][j].U = Block[i - 1][j].U;
					Block[i][j].IS_CHECK = Block[i - 1][j].IS_CHECK;
					Block[i][j].P_Block = Block[i - 1][j].P_Block;

				}
			}


			for (int j = 0; j < 7; ++j)
			{
				Block[0][j].color_R = 255;
				Block[0][j].life = LifeCount;        // Block 라이프
				Block[0][j].state = FALSE;
				Block[0][j].U = rand() % 2;
				Block[0][j].IS_CHECK = FALSE;
				Block[0][j].P_Block = TRUE;
				Block[0][rand() % 7].creat_b = rand() % 3;
				if (Block[0][j].creat_b == 0)
					Block[0][j].P_Block = FALSE;;
			}
			LifeCount++;
			B_Down = FALSE;
			SetTimer(hWnd, 1, 15, NULL);
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		mx = Ball[0].xPos;
		my = Ball[0].yPos + Ball[0].size;

		if (InCircle(ex, ey, mx, my))
		{
			Drag = TRUE;
			startX = ex;
			startY = ey;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_LBUTTONUP:

		if (Drag) {
			Drag = FALSE;
			for (int i = 0; i < Ball_Count3; ++i)
			{
				i_X = (endX - mx) / 20;
				i_Y = (endY - my) / 20;

			}

			SetTimer(hWnd, 1, 15, NULL);

		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN)); // 흰 펜

		if (Drag)
		{
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			//if ((my - endY / endX - mx > tan(PI / 10) && my - endY / endX - mx < tan(PI / 12)))
			//if ((my - endY % endX - mx > tan(PI / 12)))
			{
				MoveToEx(hdc, startX, startY, NULL);
				LineTo(hdc, oldX, oldY); // 지우기 : 흰 바탕 XOR 검은 펜 = 흰 선
				MoveToEx(hdc, startX, startY, NULL);
				LineTo(hdc, endX, endY); // 그리기 : 흰 바탕 XOR 흰 펜 = 검은 선
				oldX = endX; oldY = endY; // 현 지점을 이젂 지점으로 설정
				px1 = startX; px2 = endX;
				py1 = startY; py2 = endY;
			}
		}
		InvalidateRgn(hWnd, NULL, TRUE);
		ReleaseDC(hWnd, hdc);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			// 공의 이동 타이머
			for (int a = 0; a < Ball_Count3; ++a)
			{
				for (int i = 0; i < Ball_Count3; ++i)
				{
					if (Ball[i].Going == TRUE)
					{
						Ball[i].xPos += i_X*Ball[i].dir_X;
						Ball[i].yPos += i_Y*Ball[i].dir_Y;
					}

					if (Ball[i].xPos <= 0)                  // 왼쪽 벽 충돌
					{
						if (Ball[i].dir_X == RIGHT)
							Ball[i].dir_X = LEFT;
						else Ball[i].dir_X = RIGHT;
					}

					else if (Ball[i].xPos + Ball[i].size > 840)            // 오른쪽 벽 충돌
					{
						if (Ball[i].dir_X == LEFT)
							Ball[i].dir_X = RIGHT;

						else Ball[i].dir_X = LEFT;
					}

					if (Ball[i].yPos < rectView.top + 30)                  // 윗쪽 벽 충돌
						Ball[i].dir_Y = UP;

					if (Bar.xPos <= Ball[i].xPos  && Ball[i].xPos <= Bar.xPos + Bar.width)   // 공의 아랫라인이 바 위치보다 작은지
					{
						if (Ball[i].yPos + 2 * Ball[i].size >= Bar.yPos)                     // 공의 아랫라인이 바의 윗라인에 도달하면
						{
							Ball[i].dir_Y = DOWN;
							Block_C--;
							stop++;
							Ball[i].Going = FALSE;
							stop = 0;
							L_Count++;
						}

					}

					ex += vx;
					ey += vy;
					////////////////////////////////////////////////////////////////////   Block 충돌 체크      

					for (int b = 0; b <= LIMIT; ++b)
					{
						for (int c = 0; c < 7; ++c)
						{
							for (int ba = 0; ba < Ball_Count3; ++ba)
							{
								rect.left = Block[b][c].xPos;
								rect.top = Block[b][c].yPos;
								rect.right = Block[b][c].xPos + Block[b][c].width;
								rect.bottom = Block[b][c].yPos + Block[b][c].height;

								point1.x = Ball[ba].xPos + Ball[ba].size;
								point1.y = Ball[ba].yPos;
								point2.x = Ball[ba].xPos - Ball[ba].size;
								point2.y = Ball[ba].yPos;
								point3.x = Ball[ba].xPos;
								point3.y = Ball[ba].yPos - Ball[ba].size;
								point4.x = Ball[ba].xPos;
								point4.y = Ball[ba].yPos + Ball[ba].size;

								R1.left = Block[b][c].xPos;
								R1.top = Block[b][c].yPos;
								R1.right = Block[b][c].xPos + Block[b][c].width;
								R1.bottom = Block[b][c].yPos + Block[b][c].height;

								//////////////////////////////////////////////////////////////////////////////////////////충돌체크




								if (Block[b][c].life != 0 && Ball[ba].xPos + 10 > Block[b][c].xPos && Ball[ba].xPos + 10 < Block[b][c].xPos + Block[b][c].width
									&& Ball[ba].yPos + 10 == Block[b][c].yPos + Block[b][c].height)
									Ball[ba].dir_Y = UP;



								//if (Block[b][c].P_Block == TRUE && Block[b][c].life > 0 && (PtInRect(&rect, point1) || PtInRect(&rect, point2) || PtInRect(&rect, point3) || PtInRect(&rect, point4)))
								//{


								//   if (point1.x == R1.bottom)   ///아래
								//      Ball[ba].dir_X = DOWN;
								//   if (point1.y == R1.right)   //오
								//   {

								//      if (Ball[ba].dir_X = RIGHT)
								//         Ball[ba].dir_X = LEFT;
								//      else Ball[ba].dir_X = RIGHT;

								//   }
								//   else if (point4.y == R1.top)//왼
								//   {
								//      if (Ball[ba].dir_X = LEFT)
								//         Ball[ba].dir_X = RIGHT;
								//      else Ball[ba].dir_X = LEFT;
								//   }
								//   if (point1.x == R1.left)            //위
								//      Ball[ba].dir_X = UP;







								/*if (R1.right == point1.x)
								Ball[ba].dir_X = RIGHT;

								else if (R1.top == point1.y)
								{
								Ball[ba].dir_X = LEFT;
								}

								if (R1.top == point1.y)
								{
								Ball[ba].dir_Y = DOWN;
								}

								else
								{
								if (Ball[ba].dir_X == LEFT)
								Ball[ba].dir_X == RIGHT;
								Ball[ba].dir_Y = UP;
								}*/


								if (Block[b][c].creat_b == 2)
								{
									if (Block[b][c].IS_CHECK == FALSE)
									{
										--Block[b][c].life;
										Ball_Count1++;
									}
									Block[b][c].IS_CHECK = TRUE;

								}
								else if (Block[b][c].creat_b == 1 && Ball[ba].IS_CRUSH == FALSE)
								{
									--Block[b][c].life;
									Ball[ba].IS_CRUSH = TRUE;
								}
								Block[b][c].state = TRUE;
								Block[b][c].color_R = 0;
							}
						}
					}

				}

				if (Block_delete_num == 20)                                 // 모든 벽돌 제거시 게임 종료
					KillTimer(hWnd, 1);

				break;
			}



			for (int i = 0; i < Ball_Count2; ++i)
			{
				if (Ball[i].Going == FALSE)
					stop++;
			}

			if (stop == Ball_Count2)      //////////////////////////////모든 볼이 바닥에 닿았을때
			{

				for (int i = 0; i < Ball_Count1; ++i)
				{
					Ball[i].Going = TRUE;
					Ball[i] = Ball[0];
					Ball_Count2 = Ball_Count1;
					Ball[i].IS_CRUSH = FALSE;

				}
				Liner++;

				stop = 0;
				B_Down = TRUE;

				///////////////////////////////////////////////////////////재 초기화 박스
				KillTimer(hWnd, 1);
				ex = Ball[0].xPos + Ball[0].size / 2;
				ey = Ball[0].yPos + Ball[0].size / 2;

				for (int i = 0; i <= LIMIT; ++i)
				{
					for (int j = 0; j < 7; ++j)
					{
						if (Block[i][j].life == 0)
							Block[i][j].P_Block = FALSE;
					}
				}
				for (int i = LIMIT - 1; i > 0; --i)
				{
					for (int j = 0; j < 7; ++j)
					{
						Block[i][j].color_R = Block[i - 1][j].color_R;
						Block[i][j].life = Block[i - 1][j].life;
						Block[i][j].state = Block[i - 1][j].state;
						Block[i][j].creat_b = Block[i - 1][j].creat_b;
						Block[i][j].U = Block[i - 1][j].U;

						Block[i][j].IS_CHECK = Block[i - 1][j].IS_CHECK;
						Block[i][j].P_Block = Block[i - 1][j].P_Block;

					}
				}


				for (int j = 0; j < 7; ++j)
				{
					Block[0][j].color_R = 255;
					Block[0][j].life = LifeCount;        // Block 라이프
					Block[0][j].state = FALSE;
					Block[0][j].creat_b = rand() % 3;
					Block[0][j].U = rand() % 2;
					Block[0][j].IS_CHECK = FALSE;
					Block[0][j].P_Block = TRUE;
					if (j != 0 && Block[0][j - 1].creat_b == 2 && Block[0][j].creat_b == 2)
						Block[0][j].creat_b = 1;
					if (Block[0][j].creat_b == 0)
						Block[0][j].P_Block = FALSE;
				}
				LifeCount++;
				B_Down = FALSE;

			}


		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////그립니다

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int pa = 0; pa < Ball_Count2; ++pa)
		{
			hBrush = CreateSolidBrush(RGB(Ball[pa].color_R, Ball[pa].color_G, Ball[pa].color_B));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, Ball[pa].xPos - Ball[pa].size, Ball[pa].yPos - Ball[pa].size, Ball[pa].xPos + Ball[pa].size, Ball[pa].yPos + Ball[pa].size);         // Ball 구현

			SelectObject(hdc, hBrush);
			DeleteObject(hBrush);
		}

		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		Rectangle(hdc, Bar.xPos, Bar.yPos, Bar.xPos + Bar.width, Bar.yPos + Bar.height);      // Bar 구현
		SelectObject(hdc, hBrush);
		DeleteObject(hBrush);


		MoveToEx(hdc, px1, py1, NULL); // 이동하고 선으로 연결
		LineTo(hdc, px2, py2);



		Count = Liner;

		for (int bic = 0; bic < LIMIT; ++bic)                                                // Block 구현
		{
			for (int j = 0; j < 7; ++j)
			{

				if (Block[bic][j].P_Block == TRUE)
				{

					if (Block[bic][j].life != 0)
					{
						if (Block[bic][j].state == FALSE)
						{
							hBrush = CreateSolidBrush(RGB(Block[bic][j].color_R, 255, 0));
						}
						else
							hBrush = CreateSolidBrush(RGB(125, 255, 0));
						SelectObject(hdc, hBrush);


						if (Block[bic][j].creat_b == 1)
						{
							Rectangle(hdc, Block[bic][j].xPos, Block[bic][j].yPos, Block[bic][j].xPos + Block[bic][j].width,
								Block[bic][j].yPos + Block[bic][j].height);      // Block 구현
							SelectObject(hdc, hBrush);
							wsprintf(str, "%d", Block[bic][j].life);
							TextOut(hdc, Block[bic][j].xPos, Block[bic][j].yPos, str, strlen(str));
						}

						else if (Block[bic][j].creat_b == 2)
						{
							Ellipse(hdc, Block[bic][j].xPos + 15, Block[bic][j].yPos + 15, Block[bic][j].xPos + 30,
								Block[bic][j].yPos + 30);   //아이템 구현

							Block[bic][j].life = 1;
						}

						DeleteObject(hBrush);
					}
				}
			}

		}


		Count = 0;

		wsprintf(str, "돌의 갯수 : %d", Ball_Count1);
		TextOut(hdc, 10, 900, str, strlen(str));
		//wsprintf(str, "제거된 벽돌 : %d", Block_delete_num);
		//TextOut(hdc, 10, 800, str, strlen(str));

		EndPaint(hWnd, &ps);
		Ball_Count3 = Ball_Count1;

		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}