#include <Windows.h>
#include "resource.h"
#include <stdio.h>
//5 - 7 �� �Ѿư��� �����
//- ��� �̹����� �׷�����, �߰��߰��� ��ֹ��� �ִ�.
//- ������ ��ġ���� ����� �ִϸ��̼��� ����ȴ�.
//- ������ ��ġ�� ���콺�� Ŭ���ϸ� �� �ִϸ��̼��� ��Ÿ���� ���콺�� �巡���ϸ� ��ִϸ��̼��� �̵��Ѵ�.
//- �㸦  ���Ͽ� ����̰� �Ѿƿ´�.
//- ���콺�� ��ֹ� ���δ� ������ �� ����, ����̵� ��ֹ��� ������ ������ �������� �����̰� ��� �Ѿƿ´�.
//- ���콺�� ���� ��ִϸ��̼��� ��������.
// ���콺 Ŭ���� Ÿ�̸� ���� �㸦 �Ѿ� ����̰� �´�
HINSTANCE g_hlnst;
LPCTSTR lpszClass = "�Ŀ� �ڵ�";

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
		0, 0, 800, 600,
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
typedef struct Block {
	int L;
	int R;
	int T;
	int B;
}Block;
LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	//#ifdef _DEBUG
	//	AllocConsole();
	//	freopen("CONOUT$", "wt", stdout);
	//#endif
	//
	// �̹��� 22~ 28 ( 28 �� ��� 22~4 �� ����� 5~ 7 �� ��)
	PAINTSTRUCT ps;
	static Block block[3];
	static RECT rt;
	static HDC hDC, MemDC, Mem1DC, Mem2DC;
	static HBITMAP back, cat[3], M[3], box[3], hBitmap;
	static int mx, my;
	static int count = 0;
	static int xpos, ypos; //  ����� ��ǥ
	static bool click = false;
	static bool timerdd = false;


	GetClientRect(hWnd, &rt);

	switch (iMessage)
	{
	case WM_CREATE:
		block[0].L = 200;
		block[0].T = 200;
		block[0].R = 320;
		block[0].B = 320;

		block[1].L = 400;
		block[1].T = 400;
		block[1].R = 520;
		block[1].B = 520;

		block[2].L = 500;
		block[2].T = 100;
		block[2].R = 620;
		block[2].B = 220;
		back = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP28));// ���

		box[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP29)); // ��ֹ�
		box[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP29));
		box[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP29));

		M[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP25)); // ��
		M[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP26));
		M[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP27));

		cat[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP22)); // �����
		cat[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP23));
		cat[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP24));
		//SetTimer(hWnd, 1, 100, NULL);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (timerdd == false) {
			SetTimer(hWnd, 1, 100, NULL);
			SetTimer(hWnd, 2, 10, NULL);
			SetTimer(hWnd, 3, 10, NULL);
			click = true;
			timerdd = true;
		}
		//InvalidateRect(hWnd, NULL, false);
		break;

	case WM_LBUTTONUP:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		timerdd = false;
		click = false;
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			count++;
			count = count % 3;
			break;
		case 2:
			if (xpos < mx) {
				xpos += 1;
				if (xpos >= block[0].L && ypos > block[0].T && ypos< block[0].B) // �ڽ��� �����浹
					KillTimer(hWnd, 2);
				if (xpos >= block[1].L && ypos> block[1].T && ypos < block[1].B) // �ڽ��� �����浹
					KillTimer(hWnd, 2);
				if (xpos >= block[2].L && ypos> block[2].T && ypos < block[2].B) // �ڽ��� �����浹
					KillTimer(hWnd, 2);
			}

			if (ypos < my) {
				ypos += 1;
				if (ypos + 40 >= block[0].T && xpos + 30>block[0].L && xpos < block[0].R)  //�ڽ��� �����浹
					KillTimer(hWnd, 2);
				if (ypos + 40 >= block[1].T && xpos + 30 > block[1].L && xpos < block[1].R)  //�ڽ��� �����浹
					KillTimer(hWnd, 2);
				if (ypos + 40 >= block[1].T && xpos + 30 > block[2].L && xpos < block[2].R)  //�ڽ��� �����浹
					KillTimer(hWnd, 2);
			}
			if (xpos > mx) {
				xpos -= 1;
				if (xpos <= block[0].R &&  ypos + 30 > block[0].T && ypos + 20 < block[0].B) // �ڽ��� �������浹
					KillTimer(hWnd, 2);
				if (xpos <= block[1].R &&  ypos + 30 > block[1].T && ypos + 20 < block[1].B) // �ڽ��� �������浹
					KillTimer(hWnd, 2);
				if (xpos <= block[2].R &&  ypos + 30 > block[2].T && ypos + 20 < block[2].B) // �ڽ��� �������浹
					KillTimer(hWnd, 2);
			}
			if (ypos > my) {
				ypos -= 1;
				if (ypos <= block[0].B &&  xpos + 30 > block[0].L && xpos < block[0].R)
					KillTimer(hWnd, 2);
				if (ypos <= block[1].B &&  xpos + 30 > block[1].L && xpos < block[1].R)
					KillTimer(hWnd, 2);
				if (ypos <= block[2].B &&  xpos + 30 > block[2].L && xpos < block[2].R)
					KillTimer(hWnd, 2);
			}
			break;
		case 3:
			//���� �浹������!
			if (xpos + 30 >= block[0].L && ypos + 60 > block[0].T && ypos + 20 < block[0].B) {
				if (ypos <= my)
					ypos += 1;
				if (ypos > my)
					ypos -= 1;
			}
			else if (xpos + 30 >= block[1].L && ypos + 60 > block[1].T && ypos + 20 < block[1].B) {
				if (ypos <= my)
					ypos += 1;
				if (ypos > my)
					ypos -= 1;
			}
			else if (xpos + 30 >= block[2].L && ypos + 60 > block[2].T && ypos + 20 < block[2].B) {
				if (ypos <= my)
					ypos += 1;
				if (ypos > my)
					ypos -= 1;
			}
			//���� �浹������!
			else if (ypos + 40 >= block[0].T && xpos + 30 > block[0].L && xpos < block[0].R) {
				if (xpos <= mx)
					xpos += 1;
				if (xpos > mx)
					xpos -= 1;
			}
			else if (ypos + 40 >= block[1].T && xpos + 30 > block[1].L && xpos < block[1].R) {
				if (xpos <= mx)
					xpos += 1;
				if (xpos > mx)
					xpos -= 1;
			}
			else if (ypos + 40 >= block[2].T && xpos + 30 > block[2].L && xpos < block[2].R) {
				if (xpos <= mx)
					xpos += 1;
				if (xpos > mx)
					xpos -= 1;
			}
			//������ �浹������!
			else if (xpos <= block[0].R &&  ypos + 30 > block[0].T && ypos + 20 < block[0].B) {
				if (ypos <= my)
					ypos += 1;
				if (ypos > my)
					ypos -= 1;
			}
			else if (xpos <= block[1].R &&  ypos + 30 > block[1].T && ypos + 20 < block[1].B) {
				if (ypos <= my)
					ypos += 1;
				if (ypos > my)
					ypos -= 1;
			}
			else if (xpos <= block[2].R &&  ypos + 30 > block[2].T && ypos + 20 < block[2].B) {
				if (ypos <= my)
					ypos += 1;
				if (ypos > my)
					ypos -= 1;
			}
			//�ٴ� �浹������!
			else if (ypos <= block[0].B &&  xpos + 30 > block[0].L && xpos < block[0].R) {
				if (xpos <= mx)
					xpos += 1;
				if (xpos > mx)
					xpos -= 1;
			}
			else if (ypos <= block[1].B &&  xpos + 30 > block[1].L && xpos < block[1].R) {
				if (xpos <= mx)
					xpos += 1;
				if (xpos > mx)
					xpos -= 1;
			}
			else if (ypos <= block[2].B &&  xpos + 30 > block[2].L && xpos < block[2].R) {
				if (xpos <= mx)
					xpos += 1;
				if (xpos > mx)
					xpos -= 1;
			}
			else
				SetTimer(hWnd, 2, 10, NULL);

			break;
		}
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		MemDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, 800, 600);  // MemDC�� �����׸���  (��ȭ�� ��?)
		SelectObject(MemDC, hBitmap);
		Mem1DC = CreateCompatibleDC(MemDC);

		SelectObject(Mem1DC, back);  // ��� ����
		StretchBlt(MemDC, 0, 0, 800, 600, Mem1DC, 0, 0, 800, 600, SRCCOPY);
		SelectObject(Mem1DC, box[1]);  // ��ֹ� ����
		StretchBlt(MemDC, 400, 400, 120, 120, Mem1DC, 0, 0, 60, 60, SRCCOPY);
		SelectObject(Mem1DC, box[2]);  // ��ֹ� ����
		StretchBlt(MemDC, 500, 100, 120, 120, Mem1DC, 0, 0, 60, 60, SRCCOPY);
		SelectObject(Mem1DC, box[3]);  // ��ֹ� ����
		StretchBlt(MemDC, 200, 200, 120, 120, Mem1DC, 0, 0, 60, 60, SRCCOPY);

		SelectObject(Mem1DC, cat[count]);
		TransparentBlt(MemDC, xpos, ypos, 30, 60, Mem1DC, 0, 0, 15, 29, RGB(255, 255, 255));
		if (click == true) {
			// Ŭ���� ���丮 ����
			SelectObject(Mem1DC, M[count]);
			TransparentBlt(MemDC, mx, my, 33, 33, Mem1DC, 0, 0, 33, 33, RGB(255, 255, 255));
		}

		//printf("���콺 x = %d , ���콺 y = %d , ����� x = %d , ����� y = %d\n", mx, my, xpos, ypos);


		BitBlt(hDC, 0, 0, 800, 600, MemDC, 0, 0, SRCCOPY);

		DeleteDC(Mem2DC);
		DeleteDC(Mem1DC);
		DeleteObject(hBitmap);
		DeleteDC(MemDC);
		DeleteDC(hDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}