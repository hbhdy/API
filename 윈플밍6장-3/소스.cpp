#include <Windows.h>
#include <cmath>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR IpszClass = "파워코딩";

static bool sinL = false;
static bool cosL = false;
static bool tanL = false;

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDD_DIALOG1);;
	WndClass.lpszClassName = IpszClass;
	WndClass.hIconSm = LoadCursor(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(IpszClass, IpszClass, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_SYSMENU,
		0, 0,800, 600,
		NULL, (HMENU)NULL,
		hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

typedef struct COLOR
{
	int R;
	int G;
	int B;
}COLOR;

typedef struct FUNC
{
	float siny;
	float cosy;
	float tan;
	float r;
	float r2;
}FUNC;

static POINT key = { 3, 4 };
static COLOR color = { 0, 0, 0 };
static BOOL keyChange = FALSE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC   memdc, mem2dc, hDC;
	HBRUSH MyBrush = 0, OldBrush = 0;
	HPEN MyPen = 0, OldPen = 0;
	static HBITMAP Hbit = NULL;
	RECT rect = { 0, 0, 800, 600 };
	static double xpos;
	static HINSTANCE hInstance;
	static SIZE size;
	static FUNC func;
	static BOOL x = FALSE, y = FALSE;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		SetTimer(hWnd, 1, 100, NULL);
		g_hInst = hInstance;
		func.r = 100.0f;
		func.r2 = 180.0f;
		xpos = 180;
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		memdc = CreateCompatibleDC(hDC);
		SelectObject(memdc, Hbit);
		BitBlt(hDC, 0, 0, 800, 600, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_1Proc);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_TIMER:
		GetClientRect(hWnd, &rect);
		hDC = GetDC(hWnd);
	
		if (keyChange)
		{
			func.r = 100.0f;
			func.r2 = 180.0f;
			keyChange = FALSE;
		}

		if (Hbit == NULL)
			Hbit = CreateCompatibleBitmap(hDC, 800, 600);
		memdc = CreateCompatibleDC(hDC);
		mem2dc = CreateCompatibleDC(memdc);

		SelectObject(mem2dc, Hbit);
		MyPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		OldPen = (HPEN)SelectObject(mem2dc, MyPen);
		Rectangle(mem2dc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(mem2dc, OldPen);
		DeleteObject(MyPen);

		for (int i = -rect.right; i < rect.right; i++)
		{		
			if(sinL==true)
				func.siny = (sin(((float)i * 3.141592f / func.r2) + xpos) * 100);
			if(cosL==true)
				func.cosy = (cos(((float)i * 3.141592f / func.r2) +xpos) * 100);
			if(tanL==true)
				func.tan = (tan(((float)i * 3.141592f / func.r2) + xpos) * 100);

			if(sinL==false)
				func.siny = sin((float)i * 3.141592f / func.r2)* func.r * -1;
			if(cosL==false)
				func.cosy = cos((float)i * 3.141592f / func.r2) * func.r * -1;
			if(tanL==false)
				func.tan = tan((float)i * 3.141592f / func.r2) * func.r * -1;
			if (key.x == 0 && key.y != 0)
			{
				SetPixel(mem2dc, i + rect.right / 2, func.siny + 280, RGB(color.R, color.G, color.B));
			}
			else if (key.x == 1 && key.y != 0)
			{
				SetPixel(mem2dc, i + rect.right / 2, func.cosy + 280, RGB(color.R, color.G, color.B));
			}
			else if (key.x == 2 && key.y != 0)
			{
				SetPixel(mem2dc, i + rect.right / 2, func.tan + 280, RGB(color.R, color.G, color.B));
			}
		}

		MyPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		OldPen = (HPEN)SelectObject(mem2dc, MyPen);
		MoveToEx(mem2dc, (int)(rect.right / 2.), 0, NULL);
		LineTo(mem2dc, (int)(rect.right / 2.), rect.bottom);
		MoveToEx(mem2dc, 0, (int)(rect.bottom / 2.), NULL);
		LineTo(mem2dc, rect.right, (int)(rect.bottom / 2.));
		SelectObject(mem2dc, OldPen);
		DeleteObject(MyPen);

		if (wParam == 1)
		{
			if (key.y == 1)  //위아래
			{
				if (y == FALSE)
					func.r+=3;
				else
					func.r-=3;

				if (func.r > 200)
					y = TRUE;
				if (func.r < 0)
					y = FALSE;
			}
			else if (key.y == 2) //좌측으로
			{
				/*if (x == FALSE)
					func.r2--;
				else
					func.r2++;

				if (func.r2 > 180)
					x = FALSE;
				if (func.r2 < 45)
					x = TRUE;*/
				xpos++;
			}
		}

		BitBlt(memdc, 0, 0, 800, 600, mem2dc, 0, 0, SRCCOPY);
		SetBkMode(memdc, TRANSPARENT);
		SelectObject(mem2dc, Hbit);
		DeleteDC(mem2dc);
		SelectObject(memdc, Hbit);
		DeleteDC(memdc);
		ReleaseDC(hWnd, hDC);
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int timerCnt = 0;
	RECT rect = { 0, 0, 800, 600 };

	switch (iMsg)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, NULL);
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		/*case IDOK:
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;*/
		case IDC_BUTTON1:
			sinL = true;
			cosL = true;
			tanL = true;
			key.y = 2;
			break;
		case IDC_BUTTON2:
			sinL = false;
			cosL = false;
			tanL = false;
			key.y = 1;
			break;
		case IDC_BUTTON3:
			key.y = 4;
			break;
		case IDC_BUTTON4:
			key.y = 0;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, NULL);
			break;
		case IDC_BUTTON5:
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			break;
		case IDC_RADIO1:
			keyChange = TRUE;
			key.x = 0;
			key.y = 4;
			break;
		case IDC_RADIO2:
			keyChange = TRUE;
			key.x = 1;
			key.y = 4;
			break;
		case IDC_RADIO3:
			keyChange = TRUE;
			key.x = 2;
			key.y = 4;
			break;
		case IDC_CHECK1:
			if (color.R == 0)
				color.R = 255;
			else
				color.R = 0;
			break;
		case IDC_CHECK2:
			if (color.G == 0)
				color.G = 255;
			else
				color.G = 0;
			break;
		case IDC_CHECK3:
			if (color.B == 0)
				color.B = 255;
			else
				color.B = 0;
			break;
		}
		InvalidateRect(hDlg, NULL, TRUE);
		break;
	}
	return 0;
}