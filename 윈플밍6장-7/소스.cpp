#include <Windows.h>
#include <cmath>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR IpszClass = "¸ÊÅø";

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
static BOOL Winkey = FALSE;
static POINT map, t;
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
	HDC   memdc, hDC;
	HBRUSH MyBrush = 0, OldBrush = 0;
	HPEN MyPen = 0, OldPen = 0;
	HWND hDlg = NULL;
	static HBITMAP Hbit = NULL, oldbit, hbit1, hbit2, hbit3, hbit4, hbit5, hbit6, hbit8, hbit9, hbit10, cbit1, cbit2;
	static HINSTANCE hInstance;
	static RECT rect = { 0, 0, 800, 600 }, tRect, pic[100];
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
			hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_1Proc);
			ShowWindow(hDlg, SW_SHOW);
		}

		for (int i = 0; i < 100; ++i)
		{
			my[i].num = 0;
			my[i].num2 = 0;
			my[i].finish = TRUE;
			my[i].click = FALSE;
		}

		hbit1 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
		hbit2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
		hbit3 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
		hbit4 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
		hbit5 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP7));
		hbit6 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
		hbit8 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP9));
		hbit9 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP10));
		hbit10 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP11));
		cbit1 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		cbit2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hDC);

		if (Winkey)
		{
			tRect = { 0, 0, map.x, map.y };

			x = (float)map.x / (float)t.x;
			y = (float)map.y / (float)t.y;

			for (int i = 0; i < (t.x * t.y); ++i)
			{
				pic[i].left = (int)(cntx * x);
				pic[i].right = (int)(cntx + 1) * x;
				pic[i].top = (int)(cnty * y);
				pic[i].bottom = (int)(cnty + 1) * y;
				cntx++;
				if ((i + 1) % t.x == 0)
				{
					cntx = 0;
					cnty++;
				}
			}
			cnty = 0;
			Winkey = FALSE;
			start = TRUE;
		}

		if (start)
		{
			if (selection == 0)
			{
				oldbit = (HBITMAP)SelectObject(memdc, hbit8);
				StretchBlt(hDC, tRect.left, tRect.top, tRect.right, tRect.bottom, memdc, 0, 0, 800, 600, SRCCOPY);
			}
			else if (selection == 2)
			{
				oldbit = (HBITMAP)SelectObject(memdc, hbit9);
				StretchBlt(hDC, tRect.left, tRect.top, tRect.right, tRect.bottom, memdc, 0, 0, 800, 600, SRCCOPY);
			}
			else if (selection == 1)
			{
				oldbit = (HBITMAP)SelectObject(memdc, hbit10);
				StretchBlt(hDC, tRect.left, tRect.top, tRect.right, tRect.bottom, memdc, 0, 0, 800, 600, SRCCOPY);
			}

			for (int i = 0; i < (t.x * t.y); ++i)
			{
				FrameRect(hDC, &pic[i], MyBrush);
			}
			for (int i = 0; i < (t.x * t.y); ++i)
			{
				if (my[i].click)
				{
					if (my[i].num == 1)
					{
						oldbit = (HBITMAP)SelectObject(memdc, hbit1);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, memdc, 0, 0, 40, 40, SRCCOPY);
						SelectObject(memdc, oldbit);
					}
					else if (my[i].num == 2)
					{
						oldbit = (HBITMAP)SelectObject(memdc, hbit2);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, memdc, 0, 0, 40, 40, SRCCOPY);
						SelectObject(memdc, oldbit);
					}
					else if (my[i].num == 3)
					{
						oldbit = (HBITMAP)SelectObject(memdc, hbit3);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, memdc, 0, 0, 40, 40, SRCCOPY);
						SelectObject(memdc, oldbit);
					}
					else if (my[i].num == 4)
					{
						oldbit = (HBITMAP)SelectObject(memdc, hbit4);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, memdc, 0, 0, 40, 40, SRCCOPY);
						SelectObject(memdc, oldbit);
					}
					else if (my[i].num == 5)
					{
						oldbit = (HBITMAP)SelectObject(memdc, hbit5);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, memdc, 0, 0, 40, 40, SRCCOPY);
						SelectObject(memdc, oldbit);
					}
					else if (my[i].num == 6)
					{
						oldbit = (HBITMAP)SelectObject(memdc, hbit6);
						StretchBlt(hDC, pic[i].left, pic[i].top, (int)x, (int)y, memdc, 0, 0, 40, 40, SRCCOPY);
						SelectObject(memdc, oldbit);
					}

					if (my[i].num2 == 7 || my[i].num == 7)
					{
						oldbit = (HBITMAP)SelectObject(memdc, cbit1);
						StretchBlt(hDC, pic[i].left + 10, pic[i].top + 10, (int)x - 15, (int)y - 15, memdc, 0, 0, 100, 100, SRCCOPY);
						SelectObject(memdc, oldbit);
						DrawText(hDC, "0", 1, &pic[i], DT_BOTTOM || DT_RIGHT || DT_SINGLELINE);
					}
					else if (my[i].num2 == 8 || my[i].num == 8)
					{
						oldbit = (HBITMAP)SelectObject(memdc, cbit2);
						StretchBlt(hDC, pic[i].left + 10, pic[i].top + 10, (int)x - 15, (int)y - 15, memdc, 0, 0, 100, 100, SRCCOPY);
						SelectObject(memdc, oldbit);
						DrawText(hDC, "1", 1, &pic[i], DT_BOTTOM || DT_RIGHT || DT_SINGLELINE);
					}

				}
			}
		}

		SelectObject(hDC, Hbit);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		m.x = LOWORD(lParam);
		m.y = HIWORD(lParam);
		for (int i = 0; i < t.x * t.y; ++i)
		{
			if (PtInRect(&pic[i], m))
			{
				if (my[i].finish)
				{
					if (key == 7 || key == 8)
					{
						my[i].click = TRUE;
						my[i].num2 = key;
					}
					else
					{
						my[i].click = TRUE;
						my[i].num = key;
					}
					if (key == 7)
					{
						my[i].finish = FALSE;
					}
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		if (wParam == '1')
		{
			key = 1;
		}
		else if (wParam == '2')
		{
			key = 2;
		}
		else if (wParam == '3')
		{
			key = 3;
		}
		else if (wParam == '4')
		{
			key = 4;
		}
		else if (wParam == '5')
		{
			key = 5;
		}
		else if (wParam == '6')
		{
			key = 6;
		}
		InvalidateRgn(hWnd, NULL, false);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HBITMAP hbit1, hbit2, hbit3, hbit4, hbit5, hbit6, cbit1, cbit2;
	HWND hButton1, hButton2, hButton3, hButton4, hButton5, hButton6, cbutton1, cbutton2;
	static HINSTANCE hInst = NULL;
	static HWND  hList;
	static char num[10];

	switch (iMsg)
	{
	case WM_INITDIALOG:
		hInst = g_hInst;
		hbit1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		hbit2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		hbit3 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		hbit4 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		hbit5 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		hbit6 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		cbit1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		cbit2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));

		hButton1 = GetDlgItem(hDlg, IDC_BUTTON1);
		hButton2 = GetDlgItem(hDlg, IDC_BUTTON2);
		hButton3 = GetDlgItem(hDlg, IDC_BUTTON3);
		hButton4 = GetDlgItem(hDlg, IDC_BUTTON4);
		hButton5 = GetDlgItem(hDlg, IDC_BUTTON5);
		hButton6 = GetDlgItem(hDlg, IDC_BUTTON6);
		cbutton1 = GetDlgItem(hDlg, IDC_BUTTON7);
		cbutton2 = GetDlgItem(hDlg, IDC_BUTTON8);

		SendMessage(hButton1, BM_SETIMAGE, 0, (LPARAM)hbit1);
		SendMessage(hButton2, BM_SETIMAGE, 0, (LPARAM)hbit2);
		SendMessage(hButton3, BM_SETIMAGE, 0, (LPARAM)hbit3);
		SendMessage(hButton4, BM_SETIMAGE, 0, (LPARAM)hbit4);
		SendMessage(hButton5, BM_SETIMAGE, 0, (LPARAM)hbit5);
		SendMessage(hButton6, BM_SETIMAGE, 0, (LPARAM)hbit6);
		SendMessage(cbutton1, BM_SETIMAGE, 0, (LPARAM)cbit1);
		SendMessage(cbutton2, BM_SETIMAGE, 0, (LPARAM)cbit2);

		hList = GetDlgItem(hDlg, IDC_LIST1);
		SendMessage(hList, LB_ADDSTRING, 10, (LPARAM)"Ç®");
		SendMessage(hList, LB_ADDSTRING, 10, (LPARAM)"ÇÏ´Ã");
		SendMessage(hList, LB_ADDSTRING, 10, (LPARAM)"¹Ù´Ù");

		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EDIT1:
			GetDlgItemText(hDlg, IDC_EDIT1, num, 20);
			map.x = atoi(num);
			break;
		case IDC_EDIT2:
			GetDlgItemText(hDlg, IDC_EDIT2, num, 20);
			map.y = atoi(num);
			break;
		case IDC_EDIT3:
			GetDlgItemText(hDlg, IDC_EDIT3, num, 20);
			t.x = atoi(num);
			break;
		case IDC_EDIT4:
			GetDlgItemText(hDlg, IDC_EDIT4, num, 20);
			t.y = atoi(num);
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
			key = 7;
			break;
		case IDC_BUTTON8:
			key = 8;
			break;
		case IDC_BUTTON9:
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			DestroyWindow(hDlg);
			hDlg = NULL;
			break;
		case IDC_BUTTON10:
			Winkey = TRUE;
			break;
		case IDC_LIST1:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = SendMessage(hList, LB_GETCURSEL, 0, 0);
			break;
		}
		InvalidateRect(hDlg, NULL, TRUE);
		break;
	}
	return 0;
}