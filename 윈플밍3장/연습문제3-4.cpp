#include <Windows.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
HINSTANCE g_hlnst;
LPCTSTR lpszClass = "window program 1 - 2";
static int BSIZE = 40;
float LengthPts(int x1, int y1, int x2, int y2)
{
	return (sqrt((float)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))));
}
BOOL InCircle(int x, int y, int mx, int my)
{
	if (LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}



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
typedef struct circle {
	double X;
	double Y;
}Circle;
LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static Circle cir;
	static HPEN hPen, oldPen;
	static HBRUSH hBrush, oldBrush;
	static int count = 0;
	static double startX, startY, oldX, oldY, endX, endY;
	static double vx, vy;
	static bool create = false; // 원이 사라지면 재생성
	static bool select =false; // 원을 픽
	srand((unsigned int)time(NULL));
	
	switch (iMessage)
	{
	case WM_CREATE:
		startX = 60;
		startY = 60;
		oldX = 60;
		oldY = 60;
		cir.X = rand() % 800;
		cir.Y = rand() % 600;
		SetTimer(hWnd, 2, 80,NULL);
		break;
	case WM_LBUTTONDOWN:
		if (InCircle,cir.X,cir.Y, LOWORD(lParam), HIWORD(lParam))
		{
			create = true;
			select = true;
			startX = cir.X;
			startY = cir.Y;
			oldX = LOWORD(lParam);
			oldY = HIWORD(lParam);
		}
		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_LBUTTONUP:
		if (select)
		{
			select = false;
			vx = (endX - cir.X) / 30.000;
			vy = (endY - cir.Y) / 30.000;
			SetTimer(hWnd, 1, 10, NULL);
			count = 0;
		}
		InvalidateRect(hWnd, NULL, true);	
		break;

	case WM_MOUSEMOVE:
		hDC = GetDC(hWnd);
		if(select)
		{//흰바탕~
			SetROP2(hDC, R2_XORPEN); //펜 XOR 연산
			SelectObject(hDC, (HPEN)GetStockObject(WHITE_PEN)); // 흰펜으로 설정
			// 결론은 검은펜 ㅋㅋ
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			MoveToEx(hDC, startX, startY, NULL);
			LineTo(hDC, oldX, oldY);  //흰 선으로 한번 지워주고

			MoveToEx(hDC, startX, startY, NULL);
			LineTo(hDC, endX, endY); // 검은펜으로 그려주고~

			oldX = endX;
			oldY = endY;  // 현위치를 이전위치로 설정~
			
		}
		ReleaseDC(hWnd, hDC);
		break;
		
	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (count >= 30)
			{
				KillTimer(hWnd, 1);
				create = false;
				SetTimer(hWnd, 2, 80,NULL);
			}
				cir.X += vx;
			cir.Y += vy;
			++count;
			break;
		case 2:
			if (create == false)
			{
				BSIZE -= 1;
				if (BSIZE <= 0) {
					BSIZE = 40;
					cir.X = rand() % 800;
					cir.Y = rand() % 600;
				}
			}
			if(create == true)
			{
				KillTimer(hWnd, 2);
			}
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		MoveToEx(hDC, startX, startY, NULL); // 이동하고 선으로 연결 
		LineTo(hDC, oldX, oldY);
		
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		oldPen = (HPEN)SelectObject(hDC, hPen);
		
		Ellipse(hDC,cir.X-BSIZE, cir.Y-BSIZE,cir.X+BSIZE, cir.Y+BSIZE);
		
		SelectObject(hDC, oldBrush);
		SelectObject(hDC, oldPen);
		DeleteObject(hBrush);
		DeleteObject(hPen);
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}