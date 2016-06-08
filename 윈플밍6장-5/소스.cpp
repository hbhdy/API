#include <windows.h>
#include <stdio.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgEx6_8Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
LPCTSTR IpszClass = "파워코딩";
HWND hList, hCombo;
int Sex;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	hwnd;
	MSG 	msg;
	WNDCLASS	WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//WndClass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);		
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name",
		"6-5",
		WS_OVERLAPPEDWINDOW,
		0,0,400,400,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	HWND hDlg = NULL;

	switch (iMsg)
	{
	case WM_CREATE:

		hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgEx6_8Proc);
		ShowWindow(hDlg, SW_SHOW);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
void InitDlg(HWND hDlg)
{
	int i;
	//static char birthY[10];
	char birthY[500];
	hList = GetDlgItem(hDlg, IDC_LIST1);
	hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	SetDlgItemText(hDlg, IDC_EDIT1, "");
	SetDlgItemText(hDlg, IDC_EDIT2, "");
	CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);

	Sex = 0;
	for (int i = 1970; i<2010; i++)
	{
		wsprintf(birthY, "%d", i);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)birthY);
	}
	return;
}
void NewMember(HWND hDlg)
{
	SetDlgItemText(hDlg, IDC_EDIT1, "");
	SetDlgItemText(hDlg, IDC_EDIT2, "");
	CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	Sex = 0;
	SendMessage(hCombo, CB_SETCURSEL, 0, 0);
	return;
}
void Joining(HWND hDlg)
{
	char name[30], phone[30], birthY[10], output[200];
	int Year;
	char gender[][30] = { "여자","남자" };
	GetDlgItemText(hDlg, IDC_EDIT1, name, 20);
	if (strcmp(name, ""))
	{
		GetDlgItemText(hDlg, IDC_EDIT2, phone, 20);
		Year = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
		wsprintf(birthY, "%d", 1970 + Year);
		wsprintf(output, "이름 : %s,  전화번호 : %s,  성별 : %s,  출생년도 : %s", name, phone, gender[Sex], birthY);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)output);
	}
}
BOOL CALLBACK DlgEx6_8Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int selection;
	static char harmar[100];
	HDC hDC;
	switch (iMsg)
	{
	
	case WM_INITDIALOG:
		InitDlg(hDlg);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON2:
			Joining(hDlg);
			return 0;
		case IDC_BUTTON1:
			NewMember(hDlg);
			return 0;
		case IDC_BUTTON3:
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			return 0;
		case IDC_LIST1:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
			break;
		case IDC_RADIO1:
			Sex = 0;
			break;
		case IDC_RADIO2:
			Sex = 1;
			break;
		case IDC_BUTTON4:
			PostQuitMessage(0);
			break;
		
		}
		break;
	}
	return 0;
}