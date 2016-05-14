#include <Windows.h>
#include "resource.h"
#include <iostream>
#include <random>
using namespace std;
static bool sel = false;

HINSTANCE g_hlnst;
LPCTSTR lpszClass = "파워 코딩";

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
		0, 0, 550, 800,
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
	RECT rt;
	static HDC MemDC, Mem1DC, Mem2DC;
	static HBITMAP RunBit[8], hBit,DieBit[4], hbmMem, hbmMemOld;
	static default_random_engine dre; //기본 엔진 사용
	static uniform_int_distribution<> ui(-50, 50);
	static int xpos, ypos;
	static int jumpState;  // 1은 위로 0은 아래로
	static int jumpcount; // 점프 시간 측정
	static int mx, my;
	static int count;
	static int count2;
	static int randX, randY;
	static int randCount;
	static int randState;

	GetClientRect(hWnd, &rt);

	switch (iMessage)
	{
	case WM_CREATE:
		xpos = ypos = 300;
		dre.seed(2012181042);

		hBit = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP3));

		RunBit[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP2));
		RunBit[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP4));
		RunBit[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP5));
		RunBit[3] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP6));
		RunBit[4] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP7));
		RunBit[5] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP8));
		RunBit[6] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP9));
		RunBit[7] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP10));

		DieBit[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP11));
		DieBit[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP12));
		DieBit[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP13));
		DieBit[3] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP14));
		
		SetTimer(hWnd, 2, 100, NULL);
		break;

	case WM_LBUTTONDOWN:
		//mx = LOWORD(lParam);
		//my = HIWORD(lParam);
		sel = true;
		randX = ui(dre) / 10;
		randY = ui(dre) / 10;
		SetTimer(hWnd, 3, 100, NULL);
		
		break;

	case WM_LBUTTONUP:
		
		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP) {
			ypos -= 10;
		}
		if (wParam == VK_DOWN) {
			ypos += 10;
		}
		if (wParam == VK_RIGHT) {
			xpos += 10;
		}
		if (wParam == VK_LEFT) {
			xpos -= 10;
		}
		if (wParam == VK_SPACE) {
			SetTimer(hWnd, 1, 100, NULL);
			jumpState = 1;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_TIMER:

		switch (wParam) {
		case 1:  // 점프
			if (jumpState == 1) {
				ypos -= 10;
				jumpcount++;
				if (jumpcount == 5) {
					jumpState = 2;
				}
			}
			if (jumpState == 2) {
				ypos += 10;
				jumpcount--;
				if (jumpcount == 0) {
					jumpState = 0;
					KillTimer(hWnd, 1);
					//SetTimer(hWnd, 1, 100, NULL);
				}
			}
			break;
		case 2:   // 걷기
			count++;
			//count2++;
			count = count % 8;
			//count2 = count2 % 4;
			break;
		case 3:  // 클릭시 날아감
			count2++;
			count2 = count2 % 4;
			randCount++;
			xpos += randX;
			ypos += randY;
			if (randCount == 10) {
				KillTimer(hWnd, 3);
				randCount = 0;
				sel = false;
			}
			break;
		}
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		MemDC = CreateCompatibleDC(hDC); // 배경 받기
		hbmMem = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);//3
		hbmMemOld = (HBITMAP)SelectObject(MemDC, hbmMem);//4
		Mem1DC = CreateCompatibleDC(MemDC); // 캐릭터 받기

		
		

		SelectObject(Mem1DC, hBit);
		BitBlt(MemDC, 0, 0, 550, 800, Mem1DC, 0, 0, SRCCOPY);

		if (sel == false) {
			SelectObject(Mem1DC, RunBit[count]);
			TransparentBlt(MemDC, xpos, ypos, 100, 100, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));
		}
		else {
			SelectObject(Mem1DC, DieBit[count2]);
			TransparentBlt(MemDC, xpos, ypos, 100, 100, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));
		}
		BitBlt(hDC, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY);

		SelectObject(MemDC, hbmMemOld); //-4
		DeleteObject(hbmMem); //-3
		DeleteObject(hbmMemOld); //-3
		DeleteDC(MemDC);
		DeleteDC(Mem1DC);
		DeleteDC(hDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}