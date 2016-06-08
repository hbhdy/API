//5 - 8 �ǽ� 3 - 3!!�̹� ����ϱ�
//- ����̹����� ����Ѵ�.
//- ���� ��� �̹����� ����Ѵ�.
//- ��ȣ�� ������ ���콺�� �Ѵ�
//- ���δ� 4����(��2 �� 2) ���� ����� ���θ� ��Ÿ���� ������ �׸���.
//- ���� ��� �ǳθ��� �ְ�, �ǳθ� �տ��� �ڵ����� �ϴ� ���߰� �ٽ� �޸���.

#include <Windows.h>
#include <math.h>
#include "resource.h"
#include <stdio.h>
#define BSIZE 25

BOOL InCircle(int x, int y, int mx, int my)
{
	if (sqrt((float)((mx - x)*(mx - x) + (my - y)*(my - y)))<BSIZE)
		return TRUE;
	else
		return FALSE;
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
		0, 0, 1000, 800,
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

typedef struct REC {
	int L;
	int T;
	int R;
	int B;
}REC;
typedef struct LAMP {
	int x;
	int y;
}LAMP;

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	#ifdef _DEBUG
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);
	#endif
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static RECT rt;
	static HDC hDC, MemDC, Mem1DC, Mem2DC;
	static HBITMAP hBit, carL[4], carR[4], cross, road;
	static LAMP lamp[3]; //��ȣ�� ���� ���ݾ� ? 
	static REC LcarPoint[4], RcarPoint[4];   // ��... ���� �ڵ��� ������ �ڵ����� ��ǥ��
	static int mx, my;
	static int wide;
	static int high;
	static bool redRight;
	static bool blueRight;
	static bool yellowRight;
	
	//static bool Bb = true;  // �̵�����
	//static bool Yy = true; // 3���� ����
	//static bool Rr = true; // �ٷ� ����
	static int stopTime = 0;

	GetClientRect(hWnd, &rt);

	switch (iMessage)
	{
	case WM_CREATE:
		redRight = false;
		blueRight = false;
		yellowRight = false;
		//�������� ���� �� 
		carL[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP30)); 
		carL[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP30));
		carL[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP30));
		carR[3] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP30));
		// ���������� ���� ��
		carR[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP31)); 
		carR[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP31));
		carR[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP31));
		carR[3] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP31));
		// ���� ���
		road = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP33));
		// Ⱦ�ܺ��� ���
		cross = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP32));

		LcarPoint[0].L = 750;  //���� �� ��ġ��
		LcarPoint[0].T = 30;
		LcarPoint[1].L = 1200;
		LcarPoint[1].T = 30;
		LcarPoint[2].L = 750;
		LcarPoint[2].T = 220;
		LcarPoint[3].L = 1200;
		LcarPoint[3].T = 220;
		
		RcarPoint[0].L = 100; //�Ʒ��� �� ��ġ��
		RcarPoint[0].T = 430;
		RcarPoint[1].L = -300;
		RcarPoint[1].T = 430;
		RcarPoint[2].L = 30;
		RcarPoint[2].T = 600;
		RcarPoint[3].L = -500;
		RcarPoint[3].T = 600;

		lamp[0].x = 425; // �Ķ���
		lamp[0].y = 400;
		lamp[1].x = 500; // �����
		lamp[1].y = 400;
		lamp[2].x = 575; // ������
		lamp[2].y = 400;

		SetTimer(hWnd, 1, 0, NULL);
		break;


	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(lamp[0].x, lamp[0].y, mx, my)) {  // �Ķ��� Ŭ��
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			//KillTimer(hWnd, 3);
			SetTimer(hWnd, 1, 0, NULL);
			blueRight = true;
			yellowRight = false;
			redRight = false;
		}
		if (InCircle(lamp[1].x, lamp[1].y, mx, my)) {  // ����� Ŭ��
			yellowRight = true;
			blueRight = true;
			redRight = false;
			SetTimer(hWnd, 3, 100, NULL);
		}
		if (InCircle(lamp[2].x, lamp[2].y, mx, my)) {  // ������ Ŭ��
			redRight = true;
			blueRight = false;
			yellowRight = false;
			
		}
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (blueRight == true) {
				for (int i = 0; i < 4; ++i) {
					if (LcarPoint[i].L <= -250)
						LcarPoint[i].L = 1000;
					if (RcarPoint[i].L + 250 >= 1250)
						RcarPoint[i].L = -250;
				}
				LcarPoint[0].L -= 5;
				LcarPoint[1].L -= 5;
				LcarPoint[2].L -= 3;
				LcarPoint[3].L -= 3;
				RcarPoint[0].L += 5;
				RcarPoint[1].L += 5;
				RcarPoint[2].L += 4;
				RcarPoint[3].L += 4;
			}
			
			if (redRight == true) {
				SetTimer(hWnd, 2, 0, NULL);
				KillTimer(hWnd, 1);
			}
			//if (yellowRight == true) {
			//	printf("YellowTimer! = %d\n", stopTime); 
			//	SetTimer(hWnd, 3, 100, NULL);
			//	//KillTimer(hWnd, 1);
			//}
			break;
		case 2:
			LcarPoint[0].L -= 5;
			LcarPoint[1].L -= 5;
			LcarPoint[2].L -= 3;
			LcarPoint[3].L -= 3;
			RcarPoint[0].L += 5;
			RcarPoint[1].L += 5;
			RcarPoint[2].L +=4;
			RcarPoint[3].L += 4;
			for (int i = 0; i < 4; ++i) {
				if (LcarPoint[i].L <= -250)
					LcarPoint[i].L = 1000;
				if (RcarPoint[i].L + 250 >= 1250)
					RcarPoint[i].L = -250;
			}
			for (int i = 0; i < 2; ++i) {      // 1 �� �浹üũ
				if (LcarPoint[i].L <= 600 && LcarPoint[i].L >= 500) {
					LcarPoint[i].L += 5;
					if (LcarPoint[0].L >= 500 && LcarPoint[0].L <= 605) {
						if (LcarPoint[1].L >= 600 && LcarPoint[1].L <= 1300)
							if (LcarPoint[0].L + 250 >= LcarPoint[1].L)
								LcarPoint[1].L += 5;
					}
					if (LcarPoint[1].L >= 500 && LcarPoint[1].L <= 605) {
						if (LcarPoint[0].L >= 600 && LcarPoint[0].L <= 1300)
							if (LcarPoint[1].L + 250 >= LcarPoint[0].L)
								LcarPoint[0].L += 5;
					}
				}
			}
			for (int i = 2; i < 4; ++i) {   // 2�� �浹üũ
				if (LcarPoint[i].L <= 600 && LcarPoint[i].L >= 500) {
					LcarPoint[i].L += 3;
					if (LcarPoint[2].L >= 500 && LcarPoint[2].L <= 605) {
						if (LcarPoint[3].L >= 600 && LcarPoint[3].L <= 1300)
							if (LcarPoint[2].L + 250 >= LcarPoint[3].L)
								LcarPoint[3].L += 3;
					}
					if (LcarPoint[3].L >= 500 && LcarPoint[3].L <= 605) {
						if (LcarPoint[2].L >= 600 && LcarPoint[2].L <= 1300)
							if (LcarPoint[3].L + 250 >= LcarPoint[2].L)
								LcarPoint[2].L += 3;
					}
				}
			}
			for (int i = 0; i < 2; ++i) {        // 3�� �浹üũ
				if (RcarPoint[i].L+250 <= 500 && RcarPoint[i].L+250 >= 400) {
					RcarPoint[i].L -= 5;
					if (RcarPoint[0].L+250 >= 395 && RcarPoint[0].L+250 <= 500) {
						if (RcarPoint[1].L >= -300 && RcarPoint[1].L+250 <= 400)
							if (RcarPoint[0].L <= RcarPoint[1].L+250)
								RcarPoint[1].L -= 5;
					}
					if (RcarPoint[1].L + 250 >= 395 && RcarPoint[1].L + 250 <= 500) {
						if (RcarPoint[0].L >= -300 && RcarPoint[0].L + 250 <= 400)
							if (RcarPoint[1].L <= RcarPoint[0].L + 250)
								RcarPoint[0].L -= 5;
					}
				}
			}
			for (int i = 2; i < 4; ++i) {        // 4�� �浹üũ
				if (RcarPoint[i].L + 250 <= 500 && RcarPoint[i].L + 250 >= 400) {
					RcarPoint[i].L -= 4;
					if (RcarPoint[2].L + 250 >= 395 && RcarPoint[2].L + 250 <= 500) {
						if (RcarPoint[3].L >= -300 && RcarPoint[3].L + 250 <= 400)
							if (RcarPoint[2].L <= RcarPoint[3].L + 250)
								RcarPoint[3].L -=4;
					}
					if (RcarPoint[3].L + 250 >= 395 && RcarPoint[3].L + 250 <= 500) {
						if (RcarPoint[2].L >= -300 && RcarPoint[2].L + 250 <= 400)
							if (RcarPoint[3].L <= RcarPoint[2].L + 250)
								RcarPoint[2].L -= 4;
					}
				}
			}

			break;
		case 3:
			stopTime++;
			printf("stopTime = %d\n", stopTime);
			if (stopTime == 30) {
				blueRight = false;
				redRight = true;
				yellowRight = false;
				stopTime = 0;
				KillTimer(hWnd, 3);
			}
			//printf("stopTime = %d\n", stopTime);
		
			break;
		case 4:
	
			break;
		}

		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		MemDC = CreateCompatibleDC(hDC);
		hBit = CreateCompatibleBitmap(hDC, 1000, 800);  // Mem1DC�� �����׸���
		SelectObject(MemDC, hBit);
		Mem1DC = CreateCompatibleDC(MemDC);
		Mem2DC = CreateCompatibleDC(MemDC);

		SelectObject(Mem2DC, road); // ���� ��� ����
		BitBlt(MemDC, 0, 0, 1000, 800, Mem2DC, 0, 0, SRCCOPY);  // ���� ��� �׸���
		SelectObject(Mem2DC, cross);
		BitBlt(MemDC, 400, 0, 200, 800, Mem2DC, 0, 0, SRCCOPY);  // Ⱦ�ܺ���  �׸���

		hBrush = CreateSolidBrush((RGB(0, 0, 255)));  // ��ȣ�� �׸���
		oldBrush = (HBRUSH)SelectObject(MemDC, hBrush);
		Ellipse(MemDC, lamp[0].x-BSIZE, lamp[0].y - BSIZE, lamp[0].x + BSIZE, lamp[0].y + BSIZE);
		hBrush = CreateSolidBrush((RGB(255, 255, 0)));
		oldBrush = (HBRUSH)SelectObject(MemDC, hBrush);
		Ellipse(MemDC, lamp[1].x - BSIZE, lamp[1].y - BSIZE, lamp[1].x + BSIZE, lamp[1].y + BSIZE);
		hBrush = CreateSolidBrush((RGB(255, 0, 0)));
		oldBrush = (HBRUSH)SelectObject(MemDC, hBrush);
		Ellipse(MemDC, lamp[2].x - BSIZE, lamp[2].y - BSIZE, lamp[2].x + BSIZE, lamp[2].y + BSIZE);


		SelectObject(Mem1DC, carL[0]);
		TransparentBlt(MemDC, LcarPoint[0].L, LcarPoint[0].T, 250, 150, Mem1DC, 0, 0,250,150,RGB(255,255,255));  //���� 1�� 1��
		SelectObject(Mem1DC, carL[1]);
		TransparentBlt(MemDC, LcarPoint[1].L, LcarPoint[1].T, 250, 150, Mem1DC, 0, 0, 250, 150, RGB(255, 255, 255));  //���� 1�� 2��
		SelectObject(Mem1DC, carL[2]);
		TransparentBlt(MemDC, LcarPoint[2].L, LcarPoint[2].T, 250, 150, Mem1DC, 0, 0, 250, 150, RGB(255, 255, 255)); // ���� 2�� 1��
		SelectObject(Mem1DC, carL[3]);
		TransparentBlt(MemDC, LcarPoint[3].L, LcarPoint[3].T, 250, 150, Mem1DC, 0, 0, 250, 150, RGB(255, 255, 255)); // ���� 2�� 2��

		SelectObject(Mem1DC, carR[0]);
		TransparentBlt(MemDC, RcarPoint[0].L, RcarPoint[0].T, 250, 150, Mem1DC, 0, 0, 250, 150, RGB(255, 255, 255)); // ������ 1�� 1��
		SelectObject(Mem1DC, carR[1]);
		TransparentBlt(MemDC, RcarPoint[1].L, RcarPoint[1].T, 250, 150, Mem1DC, 0, 0, 250, 150, RGB(255, 255, 255)); // ������ 1�� 2��
		SelectObject(Mem1DC, carR[2]);
		TransparentBlt(MemDC, RcarPoint[2].L, RcarPoint[2].T, 250, 150, Mem1DC, 0, 0, 250, 150, RGB(255, 255, 255)); // ������ 2�� 1��
		SelectObject(Mem1DC, carR[3]);
		TransparentBlt(MemDC, RcarPoint[3].L, RcarPoint[3].T, 250, 150, Mem1DC, 0, 0, 250, 150, RGB(255, 255, 255)); // ������ 2�� 2��


		BitBlt(hDC, 0, 0, 1000, 800, MemDC, 0, 0, SRCCOPY);   // ���� �׸���
		
		SelectObject(MemDC, oldBrush);
		DeleteObject(hBrush);
		DeleteDC(Mem2DC);
		DeleteDC(Mem1DC);
		DeleteDC(MemDC);
		DeleteObject(hBit);
		DeleteDC(hDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}