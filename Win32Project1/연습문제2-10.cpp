#include <Windows.h>
#include <time.h>

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rect;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	static int logic[40][40]; // 도형 배열
	static char pos[10];
	static int draw_logic = 0; //그린 도형 개수
	static int lx,ly,rx,ry= 0; //랜덤 좌표 값
	static int select = 6;
	GetClientRect(hWnd, &rect);

	
	static bool nemo = true;
	static bool semo = true;
	static bool won = true;
	
	switch (iMessage)
	{
	case WM_CHAR:
		hDC = GetDC(hWnd);
		//랜덤 좌표
		rx = rand() % 700;
		ry = rand() % 700;
		lx = rand() % 700;
		ly = rand() % 700;

		if (draw_logic >= 5 )
			draw_logic = 0;

		if (wParam == 'E' || wParam == 'e') // 원을 임의의 위치에 그린다.
		{
			logic[draw_logic][0] = 1;
			logic[draw_logic][1] = lx;
			logic[draw_logic][2] = ly;
			logic[draw_logic][3] = rx;
			logic[draw_logic++][4] = ry;  //다음 도형을 위해 draw_logic을 1 올린다.
		}
		else if (wParam == 'T' || wParam == 't') // 삼각형을 임의의 위치에 그린다
		{
			logic[draw_logic][0] = 2;
			logic[draw_logic][1] = lx;
			logic[draw_logic][2] = ly;
			logic[draw_logic][3] = rx;
			logic[draw_logic][4] = ry;
			logic[draw_logic][5] = (rx+ly)/2;
			logic[draw_logic++][6] = (lx+ry)/2; // 솔직히 두개는 걍 똑같은거 넣었다.
		}
		else if (wParam == 'R' || wParam == 'r') // 사각형을 임의의 위치에 그린다.
		{
			logic[draw_logic][0] = 3;
			logic[draw_logic][1] = lx;
			logic[draw_logic][2] = ly;
			logic[draw_logic][3] = rx;
			logic[draw_logic++][4] = ry;
		}
		else if (wParam == '1')
		{
			select = 0;
		}
		else if (wParam == '2')
		{
			select = 1;
		}
		else if (wParam == '3')
		{
			select = 2;
		}
		else if (wParam == '4')
		{
			select = 3;
		}
		else if (wParam == '5')
		{
			select = 4;
		}
		else if (wParam =='/')
		{
			logic[select][0] = '\0';
			//logic[select][1] = '\0';
		}
		wParam = '0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		for (int i = 0; i <= 4; i++)
		{
			hBrush = CreateSolidBrush((RGB(rand()%255, rand()%255, rand()%255)));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			if (logic[i][0] == 1) // 원 그리기
			{
				Ellipse(hDC, logic[i][1], logic[i][2], logic[i][3], logic[i][4]);
			}
			else if (logic[i][0] == 2)
			{
				POINT tri[3] = { {logic[i][1],logic[i][2]},
				{ logic[i][3],logic[i][4] },
				{ logic[i][5],logic[i][6] } };
				Polygon(hDC, tri, 3);
			}
			else if (logic[i][0] == 3) // 사각형 그리기
			{
				Rectangle(hDC, logic[i][1], logic[i][2], logic[i][3], logic[i][4]);  //셀렉을 픽하고 셀렉과 드로우 로직을 넣고 배열 0번째 비교후 도형찾고 그도형에 맞게 그리기
			}
		}
		if (select==0|| select == 1|| select == 2|| select == 3|| select == 4) {
			if (logic[select][0] == 1)
			{
				hPen = CreatePen(PS_SOLID, 10, (RGB(0, 0, 0)));
				oldPen = (HPEN)SelectObject(hDC, hPen);
				Ellipse(hDC, logic[select][1], logic[select][2], logic[select][3], logic[select][4]);
			}
			else if (logic[select][0] == 2)
			{
				hPen = CreatePen(PS_SOLID, 10, (RGB(0, 0, 0)));
				oldPen = (HPEN)SelectObject(hDC, hPen);
				POINT tri[3] = { { logic[select][1],logic[select][2] },
				{ logic[select][3],logic[select][4] },
				{ logic[select][5],logic[select][6] } };
				Polygon(hDC, tri, 3);
			}
			else if (logic[select][0] == 3)
			{
				hPen = CreatePen(PS_SOLID, 10, (RGB(0, 0, 0)));
				oldPen = (HPEN)SelectObject(hDC, hPen);
				Rectangle(hDC, logic[select][1], logic[select][2], logic[select][3], logic[select][4]);
			}
		}
		SelectObject(hDC, oldPen);
		DeleteObject(hPen);
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