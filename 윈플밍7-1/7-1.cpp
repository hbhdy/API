#include "resource.h"
#include <windows.h> 
#include<time.h>

CLIENTCREATESTRUCT clientcreate; // 차일드 생성을 관리하기 위한 구조체
HINSTANCE hInst;
LPCTSTR lpszClass = "SHIN";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HPEN hpen, oldpen;
HFONT font, oldfont;

static char str[10][10][99] = {'0'};
static char buffer[10];
HWND hwndChild[10];
static int Childnum = 0;
static int incr = 0;
static int   check, home;
static int line[10] = {0};
static int count[10] = { 0 };
static char score[30];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	hInst = hInstance;

	// 윈도우 클래스 구조체 값 설정 



	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = FrameWndProc; // 프레임 윈도우 프로시저 
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = "SHIN";// 프레임 윈도우 클래스 이름 
	RegisterClass(&WndClass);// 윈도우 클래스 등록 
	WndClass.lpfnWndProc = ChildWndProc; // 차일드 윈도우 프로시저 
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window Class Name";// 차일드 윈도우 클래스 이름
	RegisterClass(&WndClass); // 차일드 윈도우 클래스 등록

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1280, 860, NULL, (HMENU)NULL, hInstance, NULL);  // 윈도우 생성 

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// 이벤트 루프 처리 
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
HWND hwndClient; // 클라이언트 윈도우 

MDICREATESTRUCT mdicreate;
HWND children[10];
size_t cntChildren = 0;
int GetChildrenHwnd(HWND findHwnd)
{
	for (int i = 0; i < 10; ++i)
		if (findHwnd == children[i]) return i;
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	

	static int i;

	switch (iMsg)
	{
	case WM_CREATE: // 메뉴핸들 획득 -> 창 관리 부메뉴(0) 핸들 획득 
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		clientcreate.idFirstChild = 100; // 첫 번째 차일드 ID

		hwndClient = CreateWindow("MDICLIENT",NULL,WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE
			, 0, 0, 0, 0, hwnd, NULL, hInst, (LPSTR)&clientcreate);
		ShowWindow(hwndClient, SW_SHOW); // 클라이언트 윈도우 출력
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEWWIN_ADD: // 새문서 
			Childnum = incr;
			Childnum++;
			wsprintf(buffer, "Child Window Title Name %d", Childnum);
			mdicreate.szClass = "Child Window Class Name";
			mdicreate.szTitle = buffer;
			mdicreate.hOwner = hInst;// 프로그램 인스턴스 핸들
			mdicreate.x = 0; // 자식 윈도우 X좌표 
			mdicreate.y = 0; // 자식 윈도우 Y좌표 
			mdicreate.cx = 800; // 자식 윈도우 폭 
			mdicreate.cy = 600; // 자식 윈도우 높이 
			mdicreate.style = 0;
			mdicreate.lParam = 0; // MDI 클라이언트 윈도우를 만들어라 
			hwndChild[Childnum] = (HWND)SendMessage(hwndClient, WM_MDICREATE,
				0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			children[Childnum] = hwndChild[Childnum];
			SendMessage(hwndClient, WM_MDICASCADE, 0, 0);
			if (incr < Childnum)
			{
				incr = Childnum;
			}

			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefFrameProc(hwnd, hwndClient, iMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int ypos;

	static SIZE size;

	int i, j;

	switch (iMsg) // 현재 특별한 내용은 정의되어 있지 않음 
	{
	
	case WM_CREATE:
		
		 ypos = 0;
		break;

	case WM_CHAR:
		hdc = GetDC(hWnd);
	
	
		
		if ((wParam == VK_BACK))//백스페이스
		{

			if (count[Childnum] > 0)
			{
				count[Childnum]--;
			
			}
			if (count[Childnum] == 0 && line>0)
			{
			
				check = strlen(str[Childnum][line[Childnum] - 1]);
				line[Childnum]--;
				count[Childnum] = check;
		

			}
		}
		else if (wParam == VK_RETURN)//엔터키
		{
			if (line[Childnum] < 9)
			{
				count[Childnum] = 0;
				line[Childnum]++;
			}

		}
		else if (wParam == VK_TAB)//탭키
		{
			for (int i = 0; i < 4; i++)
			{
				str[Childnum][line[Childnum]][count[Childnum]++] = ' ';

				if (count[Childnum] >= 98)
				{
					count[Childnum]--;
				}
			}
		}
		else if (wParam == VK_ESCAPE)//esc
		{
			count[Childnum] = 0;
			line[Childnum] = 0;

		}
		else
		{
			if (count[Childnum] >= 98)
			{

				count[Childnum]--;

			}
			else
			{
				str[Childnum][line[Childnum]][count[Childnum]++] = wParam;
			}
		}
		
		str[Childnum][line[Childnum]][count[Childnum]] = '\0';
		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_PAINT:
		CreateCaret(hwndChild[Childnum], NULL, 5, 15);
		for (int j = 0; j <= 10; j++)
		{
			hdc = BeginPaint(hwndChild[j], &ps);
		
			for (i = 0; i <= line[Childnum]; i++)
			{
				GetTextExtentPoint(hdc, str[j][i], strlen(str[j][i]), &size);
				TextOut(hdc, 0, i * 20, str[j][i], strlen(str[j][i]));
				SetCaretPos(size.cx, i * 20);
			}

	
		
			EndPaint(hwndChild[j], &ps);
		}
	
		for (i = 0; i <= line[Childnum]; i++)
		{
			hdc = BeginPaint(hwndChild[Childnum], &ps);
			GetTextExtentPoint(hdc, str[Childnum][i], strlen(str[Childnum][i]), &size);
			SetCaretPos(size.cx, i * 20);
			ShowCaret(hwndChild[Childnum]); // 빈 화면에 캐럿 표시
			EndPaint(hwndChild[Childnum], &ps);
		}
		break;
	case WM_LBUTTONDOWN:
		

		//현재 홗성화된 차일드의 핶들을 리턴
	Childnum= GetChildrenHwnd(hWnd);
	//	hwndChild[Childnum] = (HWND)SendMessage(hwndClient, WM_MDIGETACTIVE,
	//		0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
		
		//SendMessage(hwndChild[Childnum], WM_MDIGETACTIVE, 0, 0);
		//현재 홗성화된 차일드의 핶들을 리턴
		//SendMessage(hwndChild[Childnum], WM_MDICASCADE, 0, 0);
	EndPaint(hwndChild[Childnum], &ps);
		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		
		HideCaret(hwndChild[Childnum]); // 빈 화면에 캐럿 표시
		
		DestroyCaret();
		//PostQuitMessage(0);
		return 0;

	}

	return DefMDIChildProc(hWnd, iMsg, wParam, lParam);

	// 차일드 윈도우 메시지 함수에서는 메시지 처리한 후 마지막으로 호출하는 
	// 함수가 DefMDIChildProc (hwnd, iMsg, wParam, lParam)이다 
}