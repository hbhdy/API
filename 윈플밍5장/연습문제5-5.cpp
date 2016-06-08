#include <Windows.h>
#include "resource.h"
#include <atlimage.h>  // �߰� �߰� �߰� 

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	static RECT rt;
	static HDC hDC,MemDC,Mem1DC, Mem2DC;
	static HBITMAP enemyRunBit[3],RunBit[8], DieBit[4], back1, back2, back1Copy, back2Copy, hBitmap, hBitmap2;
	static int xpos, ypos;  // ĳ���� ��ǥ
	static int enemyXpos, enemyYpos;
	static int jumpState;  // 1�� ���� 0�� �Ʒ���
	static int jumpcount; // ���� �ð� ����
	static int glassX, copyglassX; 
	static int skyX, copyskyX;
	static int count1, count2, count3;
	static int Dietime = 0;
	static bool collision = false;
	GetClientRect(hWnd, &rt);
	
	switch (iMessage)
	{
	case WM_CREATE:
		glassX = 0;
		copyglassX = 800;
		skyX = 0;
		copyskyX = 800;
		xpos = 100;
		ypos = 425;
		enemyXpos = 700;
		enemyYpos = 450;
		// �ݴ� ĳ���� �̹��� ( ���������� �޸� )
		enemyRunBit[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP15));
		enemyRunBit[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP19));
		enemyRunBit[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP20));
		// �� ���
		back1 = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP16));
		back1Copy = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP16));
		// �ϴ� ���
		back2 = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP17));
		back2Copy = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP18));


		// ���ΰ� ( �������� �޸�) 
		RunBit[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP2));
		RunBit[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP4));
		RunBit[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP5));
		RunBit[3] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP6));
		RunBit[4] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP7));
		RunBit[5] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP8));
		RunBit[6] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP9));
		RunBit[7] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP10));
		// ����
		DieBit[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP11));  
		DieBit[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP12));
		DieBit[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP13));
		DieBit[3] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP14));
		
		SetTimer(hWnd, 1, 100, NULL);
		SetTimer(hWnd, 3, 100, NULL);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RIGHT) {
			xpos += 10;
		}
		if (wParam == VK_LEFT) {
			xpos -= 10;
		}
		if (wParam == VK_SPACE) {
			SetTimer(hWnd, 2, 100, NULL);
			jumpState = 1;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_TIMER:
		count1++;
		count1 = count1 % 8;
		count2++;
		count2 = count2 % 3;
		switch (wParam) {
		case 1: //  sky �� glass
			glassX -= 50;
			copyglassX -= 50;
			skyX -= 20;
			copyskyX -= 20;
			if (skyX <= -800) {
				skyX = 0;
				copyskyX = 800;
			}
			if (glassX <= -800) {
				glassX = 0;
				copyglassX = 800;
			}
			enemyXpos -= 20;
			if (enemyXpos <= -10)
				enemyXpos = 700;
			break;
		case 2:  // ����
			if (jumpState == 1) {
				ypos -= 25;
				xpos += 15;
				jumpcount++;
				if (jumpcount == 5) {
					jumpState = 2;
				}
			}
			if (jumpState == 2) {
				ypos += 25;
				xpos += 15;
				jumpcount--;
				if (jumpcount == 0) {
					jumpState = 0;
					KillTimer(hWnd, 2);
				}
			}
			break;
		case 3:
			if ((xpos + 70 >= enemyXpos && xpos < enemyXpos) && ypos+80>=enemyYpos) {
				collision = true;
			}
			if (collision == true) {
				Dietime++;
				count3++;
				count3 = count3 % 4;
				if (Dietime >= 8) {
					collision = false;
					Dietime = 0;
				}
			}
			break;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		MemDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, 800, 600);  // Mem1DC�� �����׸���
		SelectObject(MemDC, hBitmap);  // ���� ����
		Mem1DC = CreateCompatibleDC(MemDC);
		Mem2DC = CreateCompatibleDC(MemDC); // 1DC�� ������� 2DC ����
		SelectObject(Mem2DC, back2); // �ϴ�
		SelectObject(Mem2DC, back2Copy);
		SelectObject(Mem1DC, back1); // ��
		SelectObject(Mem1DC, back1Copy);

		BitBlt(MemDC, skyX, 0, 800, 600, Mem2DC, 0, 0, SRCCOPY); // ��� ������  �Ѿ �ؿ��� ����
		BitBlt(MemDC, copyskyX, 0, 800, 600, Mem2DC, 0, 0, SRCCOPY);

		TransparentBlt(MemDC, glassX, 500, 800, 60, Mem1DC, 0, 0,800,60, RGB(255,255,255));  // ����ȭ �� 1DC�� �ٴڱ׸� ( ù���� )
		TransparentBlt(MemDC, copyglassX, 500, 800, 60, Mem1DC, 0, 0, 800, 60, RGB(255, 255, 255)); // ����ȭ �� 1D�� �ٴڱ׸� ( �ι�° ) �� ������ �Ѿ
		
		SelectObject(Mem1DC, RunBit[count1]); // ĳ���� �̹���
		TransparentBlt(MemDC, xpos, ypos, 100, 100, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));

		SelectObject(Mem1DC, enemyRunBit[count2]); // ��ĳ���� �̹���
		TransparentBlt(MemDC, enemyXpos, enemyYpos, 60, 60, Mem1DC, 0, 0, 76, 100, RGB(255, 255, 255));
		
		if (collision == true) {
			SelectObject(Mem1DC, DieBit[count3]);
			TransparentBlt(MemDC, enemyXpos-150, 300, 300, 300, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));
		}
		BitBlt(hDC, 0, 0, 800, 600, MemDC, 0, 0, SRCCOPY);


	
		DeleteDC(Mem2DC);
		DeleteDC(Mem1DC);
		DeleteDC(MemDC);
		//DeleteDC(hDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}