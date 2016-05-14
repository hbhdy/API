#include <Windows.h>
#include "resource.h"

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
		0, 0, 600, 480,
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
	PAINTSTRUCT ps;
	HDC hDC, MemDC;
	static HBITMAP hBitmap;
	static RECT rect;
	static Block block;
	static double size = 1;
	
	static int startX, startY;
	static int endX, endY;
	static int s_copyX, s_copyY;
	static int e_copyX, e_copyY;
	static bool select = false;
	static bool trans = false;
	
	GetClientRect(hWnd, &rect);

	
	switch (iMessage)
	{
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP1));
		SetTimer(hWnd, 1, 50, NULL);
		break;

	case WM_LBUTTONDOWN:
		startX = LOWORD(lParam);
		startY = HIWORD(lParam);
		//InvalidateRect(hWnd, NULL, true);
		break;
	case WM_MOUSEMOVE:
		hDC = GetDC(hWnd);
		endX = LOWORD(lParam);
		endY = HIWORD(lParam);
		//InvalidateRect(hWnd, NULL, true);
		break;

	case WM_LBUTTONUP:
		//InvalidateRect(hWnd, NULL, false);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:  // ����
			startX -= 5;
			endX -= 5;
			break;
		case VK_RIGHT:
			startX += 5;
			endX += 5;
			break;
		case VK_UP:
			startY -= 5;
			endY -= 5;
			break;
		case VK_DOWN:
			startY += 5;
			endY += 5;
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		if (wParam == '1') { // �簢������ �׸��� 1.2�� Ȯ��
			size = 0.8;
		}
		if (wParam == '2') { // �簢������ �׸��� 1.4�� Ȯ��
			size = 0.7;
		}
		if (wParam == '0') { // �簢������ �׸��� ������ ũ��� ��ȯ
			size = 1;
		}
		if (wParam == 'p') { // �簢������ �׸��� ����ȴ�.
			select = true;
		}
		if (wParam == 'c') { // �簢������ �׸��� �׸��� (0,0) ��ġ�� ����ȴ�.
			/*s_copyX = startX;
			s_copyY = startY;
			e_copyX = endX;
			e_copyY = endY;*/
			trans = true;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (trans == true) {
				s_copyX = startX;
				s_copyY = startY;
				e_copyX = endX;
				e_copyY = endY;
			}
			break;
		}
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		MemDC = CreateCompatibleDC(hDC);
		SelectObject(MemDC, hBitmap);
		
		//BitBlt = 1:1 copy , StretchBlt = Ȯ�� , ��� copy
		BitBlt(hDC, 0, 0,640, 480, MemDC, 0, 0, SRCCOPY);
		Rectangle(hDC,startX,startY,endX,endY);
		StretchBlt(hDC, startX, startY, endX-startX, endY-startY, MemDC, startX*size, startY*size, (endX-startX)*size , (endY-startY)*size , SRCCOPY);
		if (select == true) {
			StretchBlt(hDC, 0, 0, e_copyX-s_copyX,e_copyY-s_copyY, MemDC, s_copyX*size, s_copyY*size, (e_copyX - s_copyX)*size, (e_copyY - s_copyY)*size, SRCCOPY);
		}
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}