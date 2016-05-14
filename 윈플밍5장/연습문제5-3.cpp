#include <Windows.h>
#include "resource.h"

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
		0, 0, 1200, 480,
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
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam)
{
	HDC hdc, mem1dc;
	static HBITMAP hBitmap;
	PAINTSTRUCT ps;
	static RECT rect, isNemo[5][5], isTempNemo;
	static Block block;
	HPEN hPen, oldPen;
	static int isX = 0, isY = 0, isLineData = 3, isNemoData = 0;
	int isRealX, isRealY;
	int tempbx, tempby;
	static int mx, my;
	static int wide, high = 3;
	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		hBitmap = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP1));

		// 최대 그림 5개 삽입
		for (int i = 0; i<5; ++i) {
			for (int j = 0; j<5; ++j) {
				isNemo[i][j].right = 0, isNemo[i][j].left = 0, isNemo[i][j].bottom = 0, isNemo[i][j].top = 0;
			}
		}
		break;

	case WM_CHAR:
		if (wParam == '0') {
			
		}
		if (wParam == '1') {
			wide = 1;
			high = 1;
		}
		if (wParam == '2') {
			wide = 2;
			high = 2;
		}
		if (wParam == '3') {
			wide = 3;
			high = 3;
		}
		if (wParam == '4') {
			wide = 4;
			high = 4;
		}
		if (wParam == '5') {
			wide = 5;
			high = 5;
		}
		InvalidateRect(hwnd, NULL, true);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		isRealX = rect.right / 2;
		isRealY = rect.bottom;

		// 왼쪽 클릭시 범위 체크후 temp변수에 넣음
		for (int i = 0; i<isLineData; ++i) {
			for (int j = 0; j<isLineData; ++j) {
				if (isRealX / isLineData*j <= mx && isRealY / isLineData*i <= my && isRealX / isLineData*(j + 1) >= mx && isRealY / isLineData*(i + 1) >= my) {
					isTempNemo.left = isRealX / isLineData*j;
					isTempNemo.right = isRealX / isLineData*(j + 1);
					isTempNemo.top = isRealY / isLineData*i;
					isTempNemo.bottom = isRealY / isLineData*(i + 1);
					break;
				}

			}
		}
		break;

	case WM_RBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		isRealX = rect.right / (2*wide);
		isRealY = rect.bottom / high;

		// 오른쪽 클릭시 복사한 temp변수를 네모변수에 넣음
		for (int i = 0; i<high; ++i) {
			for (int j = 0; j<wide; ++j) {
				if ((isRealX*j+rect.right/2 <= mx && isRealY * i <= my && isRealX*(j+1)+rect.right/2 >= mx && isRealY * (i + 1) >= my))
				{

					isNemo[i][j].left = isTempNemo.left;
					isNemo[i][j].right = isTempNemo.right;
					isNemo[i][j].top = isTempNemo.top;
					isNemo[i][j].bottom = isTempNemo.bottom;
				}
			}
		}
		InvalidateRgn(hwnd, NULL, true);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBitmap);
		// SRCCOPY : 바탕색을 무시하고 그려라
		StretchBlt(hdc, 0, 0, rect.right / 2, rect.bottom, mem1dc, 0, 0, 600, 480, SRCCOPY);

		isRealX = rect.right / 2;
		isRealY = rect.bottom;

		for (int i = 0; i < high; i++)  // 옆에 네모 분할
			for (int j = 0; j < wide; j++)
			{
				Rectangle(hdc, (rect.right/(wide*2))*j +rect.right/2, (rect.bottom/high)*i, (rect.right/(wide*2))*(j + 1)+rect.right/2, (rect.bottom/high)*(i + 1)); // 
			}

		for (int i = 0; i<high; ++i) {
			for (int j = 0; j<wide; ++j) {

				if (isNemo[i][j].left == 0 && isNemo[i][j].right == 0 && isNemo[i][j].top == 0 && isNemo[i][j].bottom == 0) {

				}
				else {
					tempbx = isRealX / isLineData;
					tempby = isRealY / isLineData;
					//StretchBlt(hdc, isRealX / isLineData*(i + isLineData), isRealY / isLineData*j, tempbx, tempby, mem1dc, isNemo[i][j].left*1.7, isNemo[i][j].top / 1.3, 800 / isLineData, 500 / isLineData, SRCCOPY);
					StretchBlt(hdc, (rect.right / (wide * 2))*j + rect.right / 2, (rect.bottom / high)*i, (rect.right / (wide * 2))*(j + 1), (rect.bottom / high)*(i + 1), mem1dc, isTempNemo.left, isTempNemo.top, isTempNemo.right, isTempNemo.bottom, SRCCOPY);
					//StretchBlt(hdc, (rect.right / (wide * 2))*j + rect.right / 2, (rect.bottom / high)*i, (rect.right / (wide * 2))*(j + 1), (rect.bottom / high)*(i + 1), mem1dc, isNemo[i][j].left, isNemo[i][j].right, isNemo[i][j].top, isNemo[i][j].bottom, SRCCOPY);
				}
			}
		}

		DeleteDC(mem1dc);

		hPen = CreatePen(PS_DOT, 2, RGB(81, 222, 92));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		for (int i = 1; i<3; ++i) {
			MoveToEx(hdc, rect.right / (isLineData * 2)*i, 0, NULL);
			LineTo(hdc, rect.right / (isLineData * 2)*i, 480);
		}
		for (int i = 0; i <= isLineData; ++i) {
			if (i == 0) {
				MoveToEx(hdc, rect.left, 0, NULL);
				LineTo(hdc, rect.right/2, 0);
			}
			else {
				MoveToEx(hdc, rect.left, rect.bottom / (isLineData)*i, NULL);
				LineTo(hdc, rect.right/2, rect.bottom / (isLineData)*i);
			}
		}
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}