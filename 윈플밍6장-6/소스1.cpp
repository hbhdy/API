#include <Windows.h>
#include "resource.h"
HINSTANCE g_hlnst;
LPCTSTR lpszClass = "파워 코딩";

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM IParam);
BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
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

static HBITMAP tile1, tile2, tile3, tile4, tile5, tile6;
static HINSTANCE hInstance;

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	
	PAINTSTRUCT ps;
	HDC hDC;
	HWND hDlg = NULL;
	static RECT rect;
	
	//static HBITMAP hBitmap, tile1, tile2, tile3, tile4, tile5, tile6;

	GetClientRect(hWnd, &rect);

	switch (iMessage)
	{
	case WM_CREATE:
		if (!IsWindow(hDlg))
		{
			hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_1Proc);
			ShowWindow(hDlg, SW_SHOW);
		}
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HBITMAP tile1, tile2, tile3, tile4, tile5, tile6;
	static HINSTANCE hInst = NULL;
	HWND hButton1, hButton2, hButton3, hButton4, hButton5, hButton6;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		hInst = hInstance;
		// 비트맵 저장 설정
		tile1 = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		tile2 = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		tile3 = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		tile4 = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		tile5 = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		tile6 = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		// 버튼 활성화
		hButton1 = GetDlgItem(hDlg, IDC_BUTTON1);
		hButton2 = GetDlgItem(hDlg, IDC_BUTTON2);
		hButton3 = GetDlgItem(hDlg, IDC_BUTTON3);
		hButton4 = GetDlgItem(hDlg, IDC_BUTTON4);
		hButton5 = GetDlgItem(hDlg, IDC_BUTTON5);
		hButton6 = GetDlgItem(hDlg, IDC_BUTTON6);
		
		SendMessage(hButton1, BM_SETIMAGE, 0, (LPARAM)tile1);
		SendMessage(hButton2, BM_SETIMAGE, 0, (LPARAM)tile2);
		SendMessage(hButton3, BM_SETIMAGE, 0, (LPARAM)tile3);
		SendMessage(hButton4, BM_SETIMAGE, 0, (LPARAM)tile4);
		SendMessage(hButton5, BM_SETIMAGE, 0, (LPARAM)tile5);
		SendMessage(hButton6, BM_SETIMAGE, 0, (LPARAM)tile6);
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EDIT1:
			//GetDlgItemText(hDlg, IDC_EDIT1, num, 20);
			//map.x = atoi(num);
			break;
		case IDC_EDIT2:
			//GetDlgItemText(hDlg, IDC_EDIT2, num, 20);
			//map.y = atoi(num);
			break;
		case IDC_EDIT3:
			//GetDlgItemText(hDlg, IDC_EDIT3, num, 20);
			//t.x = atoi(num);
			break;
		case IDC_EDIT4:
			//GetDlgItemText(hDlg, IDC_EDIT4, num, 20);
			//t.y = atoi(num);
			break;
		case IDC_BUTTON1:
	
			break;
		case IDC_BUTTON2:
	
			break;
		case IDC_BUTTON3:

			break;
		case IDC_BUTTON4:
	
			break;
		case IDC_BUTTON5:
		
			break;
		case IDC_BUTTON6:
			
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