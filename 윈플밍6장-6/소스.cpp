#include <Windows.h>
#include <cmath>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR IpszClass = "파워코딩";

BOOL CALLBACK Dlg6_1Proc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDD_DIALOG1);
	WndClass.lpszClassName = IpszClass;
	WndClass.hIconSm = LoadCursor(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(IpszClass, IpszClass, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_SYSMENU, 0, 0,
		800, 600, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}
static BOOL Winkey = false;
typedef struct MAP {
	double x;
	double y;
}MAP;
static MAP map;
static MAP t;
//static POINT map, t;

static int key = 0, selection;

typedef struct MY
{
	BOOL click;
	BOOL finish;
	int num;
	int num2;
}MY;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC   MemDC, hDC;
	HBRUSH MyBrush = 0, OldBrush = 0;
	HPEN MyPen = 0, OldPen = 0;
	HWND hDlg = NULL;
	static HBITMAP hBitmap, hBitmap2, oldbit, tile1, tile2, tile3, tile4, tile5, tile6;
	static HINSTANCE hInstance;
	static RECT rect, tRect, pic[100];
	static BOOL start = FALSE;
	static int cntx = 0, cnty = 0, num;
	static float x, y;
	static POINT m;
	static MY my[100];

	static int temp = 0;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		SetTimer(hWnd, 1, 100, NULL);
		hInstance = g_hInst;
		if (!IsWindow(hDlg))
		{
			hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_1Proc);
			ShowWindow(hDlg, SW_SHOW);
		}
		for (int i = 0; i < 100; ++i)
		{
			my[i].num = 0;
			my[i].num2 = 0;
			my[i].click = FALSE;
		}

		tile1 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		tile2 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		tile3 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
		tile4 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
		tile5 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
		tile6 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
	
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		MemDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, 800, 600);
		SelectObject(MemDC, hBitmap);  // 바탕 설정
		x = (float)map.x / (float)t.x;
		y = (float)map.y / (float)t.y;

		for (int i = 0; i < (t.x * t.y); ++i)
		{
			pic[i].left = (int)(cntx * x);
			pic[i].right = (int)(cntx + 1) * x;
			pic[i].top = (int)(cnty * y);
			pic[i].bottom = (int)(cnty + 1) * y;
			cntx++;
			if ((i + 1) %(int)t.x == 0)
			{
				cntx = 0;
				cnty++;
			}
		}
		cnty = 0;
		if (Winkey==true)
		{
			
			cnty = 0;
			//Winkey = FALSE;
			start = TRUE;
		}
	
		if (start==TRUE){
			for (int i = 0; i < (t.x * t.y); ++i)
			{
				Rectangle(hDC, pic[i].left, pic[i].top, pic[i].right, pic[i].bottom);
				//FrameRect(MemDC, &pic[i], MyBrush);
			}
		
			for (int i = 0; i < (t.x * t.y); ++i)
			{
				if (my[i].click==true)
				{
					if (my[i].num == 1)
					{
						SelectObject(MemDC, tile1);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, MemDC, 0, 0, 40, 40, SRCCOPY);
						//SelectObject(memdc, oldbit);
					}
					else if (my[i].num == 2)
					{
						SelectObject(MemDC, tile2);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, MemDC, 0, 0, 40, 40, SRCCOPY);
						//SelectObject(memdc, oldbit);
					}
					else if (my[i].num == 3)
					{
						SelectObject(MemDC, tile3);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, MemDC, 0, 0, 40, 40, SRCCOPY);
						//SelectObject(memdc, oldbit);
					}
					else if (my[i].num == 4)
					{
						SelectObject(MemDC, tile4);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, MemDC, 0, 0, 40, 40, SRCCOPY);
						//SelectObject(memdc, oldbit);
					}
					else if (my[i].num == 5)
					{
						SelectObject(MemDC, tile5);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, MemDC, 0, 0, 40, 40, SRCCOPY);
						//SelectObject(memdc, oldbit);
					}
					else if (my[i].num == 6)
					{
						SelectObject(MemDC, tile6);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, MemDC, 0, 0, 40, 40, SRCCOPY);
						//SelectObject(memdc, oldbit);
					}
				}
			}
		}

		
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		m.x = LOWORD(lParam);
		m.y = HIWORD(lParam);
		for (int i = 0; i < t.x * t.y; ++i)
		{
			if (PtInRect(&pic[i], m))
			{
						my[i].click = TRUE;				
						my[i].num = key;	
			}
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_LBUTTONUP:
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_CHAR:
		if (wParam == 's') {
			Winkey = true;
		}
		
		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HBITMAP hbit1, hbit2, hbit3, hbit4, hbit5, hbit6;
	HWND hButton1, hButton2, hButton3, hButton4, hButton5, hButton6;
	static HINSTANCE hInst = NULL;
	static HWND  hList;
	static char num[10];

	switch (iMsg)
	{
	case WM_INITDIALOG:
		hInst = g_hInst;
		hbit1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hbit2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		hbit3 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		hbit4 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		hbit5 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		hbit6 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));

		hButton1 = GetDlgItem(hDlg, IDC_BUTTON1);
		hButton2 = GetDlgItem(hDlg, IDC_BUTTON2);
		hButton3 = GetDlgItem(hDlg, IDC_BUTTON3);
		hButton4 = GetDlgItem(hDlg, IDC_BUTTON4);
		hButton5 = GetDlgItem(hDlg, IDC_BUTTON5);
		hButton6 = GetDlgItem(hDlg, IDC_BUTTON6);

		SendMessage(hButton1, BM_SETIMAGE, 0, (LPARAM)hbit1);
		SendMessage(hButton2, BM_SETIMAGE, 0, (LPARAM)hbit2);
		SendMessage(hButton3, BM_SETIMAGE, 0, (LPARAM)hbit3);
		SendMessage(hButton4, BM_SETIMAGE, 0, (LPARAM)hbit4);
		SendMessage(hButton5, BM_SETIMAGE, 0, (LPARAM)hbit5);
		SendMessage(hButton6, BM_SETIMAGE, 0, (LPARAM)hbit6);
		
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EDIT1:
			map.x=GetDlgItemInt(hDlg, IDC_EDIT1, NULL, FALSE);
			
			break;
		case IDC_EDIT2:
			map.y = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, FALSE);
			
			break;
		case IDC_EDIT3:
			t.x = GetDlgItemInt(hDlg, IDC_EDIT3, NULL, FALSE);
			
			break;
		case IDC_EDIT4:
			t.y = GetDlgItemInt(hDlg, IDC_EDIT4, NULL, FALSE);
			
			break;
		case IDC_BUTTON1:
			key = 1;
			break;
		case IDC_BUTTON2:
			key = 2;
			break;
		case IDC_BUTTON3:
			key = 3;
			break;
		case IDC_BUTTON4:
			key = 4;
			break;
		case IDC_BUTTON5:
			key = 5;
			break;
		case IDC_BUTTON6:
			key = 6;
			break;
		case IDC_BUTTON7:
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			DestroyWindow(hDlg);
			hDlg = NULL;
			break;
		}
		InvalidateRect(hDlg, NULL, false);
		break;
	}
	return 0;
}