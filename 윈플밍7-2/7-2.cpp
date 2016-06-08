#include <windows.h> 

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

CLIENTCREATESTRUCT clientcreate; // 차일드 생성을 관리하기 위한 구조체
static int index = 0;
static char buffer[10];
HWND HwndChild[5];
RECT rect;
static int check = 0;
HWND children[4];
size_t cntChildren = 0;
typedef struct Circle {
	int x;
	int y;
	int r;
	int speedx;
	int speedy;
}cir;
cir c_cir;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	// 윈도우 클래스 구조체 값 설정 

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = FrameWndProc; // 프레임 윈도우 프로시저 
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	
	// 윈도우 클래스 등록 
	RegisterClass(&WndClass);


	WndClass.lpfnWndProc = ChildWndProc; // 차일드 윈도우 프로시저 
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window Class Name";
	// 차일드 윈도우 클래스 이름

	RegisterClass(&WndClass); // 차일드 윈도우 클래스 등록


							  // 윈도우 생성 
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	// 윈도우 출력 
	HwndChild[4] = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// 이벤트 루프 처리 
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


int GetChildrenHwnd(HWND findHwnd)
{
	for (int i = 0; i < 10; ++i)
		if (findHwnd == children[i]) return i;
}

LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient; // 클라이언트 윈도우

	switch (iMsg)
	{
	case WM_CREATE: // 메뉴핸들 획득 -> 창 관리 부메뉴(0) 핸들 획득 
		GetClientRect(hWnd, &rect);

		HwndChild[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
			"Child Window Class Name", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			0, 0, rect.right / 2 - 1, rect.bottom / 2 - 1, hWnd, NULL, g_hInst, NULL);
		children[0] = HwndChild[0];
		HwndChild[1] = CreateWindowEx(WS_EX_CLIENTEDGE,
			"Child Window Class Name", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			0, rect.bottom / 2 + 1, rect.right / 2 - 1, rect.bottom,
			hWnd, NULL, g_hInst, NULL);
		children[1] = HwndChild[1];
		HwndChild[2] = CreateWindowEx(WS_EX_CLIENTEDGE,
			"Child Window Class Name", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			rect.right / 2 + 1, 0, rect.right, rect.bottom / 2 - 1,
			hWnd, NULL, g_hInst, NULL);
		children[2] = HwndChild[2];
		HwndChild[3] = CreateWindowEx(WS_EX_CLIENTEDGE,
			"Child Window Class Name", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			rect.right / 2 + 1, rect.bottom / 2 + 1, rect.right, rect.bottom,
			hWnd, NULL, g_hInst, NULL);
		children[3] = HwndChild[3];
		break;



	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefFrameProc(hWnd, hwndClient, iMsg, wParam, lParam);
}



LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT Child_rect[4];

	static int select = 0;
	static bool drag;

	static int mx, my;

	static char str[10][10][99] = { '0' };
	static char strcopy[10][99];
	static int  yPos;
	static int line[2] = { 0 };
	static int count[2] = { 0 };
	static SIZE size;
	int i, j;


	GetClientRect(HwndChild[0], &Child_rect[0]);
	GetClientRect(HwndChild[1], &Child_rect[1]);
	GetClientRect(HwndChild[2], &Child_rect[2]);
	GetClientRect(HwndChild[3], &Child_rect[3]);

	switch (iMsg)
	{

	case WM_CREATE:
		c_cir.x = 40;
		c_cir.y = 40;
		c_cir.r = 20;
		c_cir.speedx = 1;
		c_cir.speedy = 1;
		break;


	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		select = GetChildrenHwnd(hWnd);
		hdc = BeginPaint(HwndChild[select], &ps);
		//EndPaint(HwndChild[select], &ps);
		if (hWnd == HwndChild[0]) {
			KillTimer(HwndChild[3], 1);
			select = 0;
			drag = true;

		}
		else if (hWnd == HwndChild[1]) {
			KillTimer(HwndChild[3], 1);
			select = 1;
			SetFocus(HwndChild[1]);

		}
		else if (hWnd == HwndChild[2]) {
			KillTimer(HwndChild[3], 1);
			select = 2;
			SetFocus(HwndChild[2]);
		}

		else if (hWnd == HwndChild[3]) {
			SetTimer(HwndChild[3], 1, 10, NULL);
			SetFocus(HwndChild[3]);
		}
		ReleaseDC(HwndChild[select], hdc);
		break;

	case WM_LBUTTONUP:
		drag = false;
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(HwndChild[select]);
		if (drag) {

			MoveToEx(hdc, mx, my, NULL);
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			LineTo(hdc, mx, my);
		}
		ReleaseDC(HwndChild[select], hdc);
		break;
	case WM_KEYDOWN:
		if(wParam==VK_ESCAPE)
				PostQuitMessage(0);
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
		{
	
				c_cir.x += 5 * c_cir.speedx;
				c_cir.y += 5 * c_cir.speedy;
				if (c_cir.x - c_cir.r <= Child_rect[3].left)
				{
					c_cir.speedx *= -1;
				}
				else if (c_cir.x + c_cir.r >= 720)
				{
					c_cir.speedx *= -1;
				}
				else if (c_cir.y - c_cir.r <= Child_rect[3].top)
				{
					c_cir.speedy *= -1;
				}
				else if (c_cir.y + c_cir.r >= 360)
				{
					c_cir.speedy *= -1;
				}
			
		}
		break;
		}
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		hdc = GetDC(hWnd);
		if ((wParam == VK_BACK))//백스페이스
		{

			if (count[select] > 0)
			{
				count[select]--;

			}
			if (count[select] == 0 && line>0)
			{

				check = strlen(str[select][line[select] - 1]);
				line[select]--;
				count[select] = check;


			}
		}
		else if (wParam == VK_RETURN)//엔터키
		{
			if (line[select] < 9)
			{
				count[select] = 0;
				line[select]++;
			}

		}
		else if (wParam == VK_TAB)//탭키
		{
			for (int i = 0; i < 4; i++)
			{
				str[select][line[select]][count[select]++] = ' ';

				if (count[select] >= 98)
				{
					count[select]--;
				}
			}
		}
		else if (wParam == VK_ESCAPE)//esc
		{
			count[select] = 0;
			line[select] = 0;

		}
		else
		{
			if (count[select] >= 98)
			{

				count[select]--;

			}
			else
			{
				str[select][line[select]][count[select]++] = wParam;
			}
		}

		str[select][line[select]][count[select]] = '\0';
		InvalidateRect(hWnd, NULL, true);
		break;


	case WM_PAINT:
		if (select == 2 || select == 1)
		{
			CreateCaret(HwndChild[select], NULL, 5, 15);
		}
		for (int j = 1; j < 4; j++)
		{
			hdc = BeginPaint(HwndChild[j], &ps);

			if (j < 3)
			{
				for (i = 0; i <= line[select]; i++) {
					GetTextExtentPoint(hdc, str[j][i], strlen(str[j][i]), &size);
					TextOut(hdc, 0, i * 20, str[j][i], strlen(str[j][i]));
				}
			}
			else
			{
				Ellipse(hdc, c_cir.x - c_cir.r, c_cir.y - c_cir.r, c_cir.x + c_cir.r, c_cir.y + c_cir.r);
			}
			EndPaint(HwndChild[j], &ps);
		}
		for (i = 0; i <= line[select]; i++)
		{
			hdc = BeginPaint(HwndChild[select], &ps);
			GetTextExtentPoint(hdc, str[select][i], strlen(str[select][i]), &size);
			SetCaretPos(size.cx, i * 20);
			ShowCaret(HwndChild[select]); // 빈 화면에 캐럿 표시
			EndPaint(HwndChild[select], &ps);
		}
		break;
	case WM_DESTROY:
		HideCaret(HwndChild[select]);
		KillTimer(HwndChild[3], 1);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}