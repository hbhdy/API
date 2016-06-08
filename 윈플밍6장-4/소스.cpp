#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hlnst;
LPCTSTR lpszClass = "ÆÄ¿öÄÚµù";

HWND hMDlg;
HWND hWndMain;
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{


	static int num = 0;
	static int cnt6 = 0;
	static int x = 0; // 1000 0000
	static int cnt = 0;
	static int cnt2 = 0;
	static int cnt3 = 0;
	static int cnt4 = 0;
	static int cnt5 = 0;
	static int realnumber[2] = { 0,0 };
	static int realnumber2[2] = { 0,0 };
	static int carry = 1;
	static int number[2][10];
	static int sumnumber = 0;
	static char *signal[10][10];
	static char show[128];
	static char show2[128];
	static char show3[128];
	static char show4[128];
	static char show5[128];
	static int flag = 5;


	HDC hdc;
	PAINTSTRUCT ps;
	HWND hButton;
	switch (iMessage) {
	case WM_INITDIALOG:

		break;


	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDC_BUTTON3: // 9

			show[cnt4] = '9';

			number[cnt2][cnt] = 9;

			for (int i = 0; i <= cnt; ++i) {

				carry *= 10;
				if (i == 0) {
					carry = 1;
				}
				realnumber[cnt2] += carry*number[cnt2][cnt - i];
			}



			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber[cnt2], FALSE);
			realnumber2[cnt2] = realnumber[cnt2];
			realnumber[cnt2] = 0;
			carry = 1;
			++cnt4;
			++cnt;
			break;
		case IDC_BUTTON2: // 8

			show[cnt4] = '8';

			number[cnt2][cnt] = 8;

			for (int i = 0; i <= cnt; ++i) {

				carry *= 10;
				if (i == 0) {
					carry = 1;
				}
				realnumber[cnt2] += carry*number[cnt2][cnt - i];
			}

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber[cnt2], FALSE);
			realnumber2[cnt2] = realnumber[cnt2];
			realnumber[cnt2] = 0;
			carry = 1;
			++cnt;
			++cnt4;
			break;
		case IDC_BUTTON1: // 7
			show[cnt4] = '7';
			number[cnt2][cnt] = 7;

			for (int i = 0; i <= cnt; ++i) {

				carry *= 10;
				if (i == 0) {
					carry = 1;
				}
				realnumber[cnt2] += carry*number[cnt2][cnt - i];
			}

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber[cnt2], FALSE);
			carry = 1;
			realnumber2[cnt2] = realnumber[cnt2];
			realnumber[cnt2] = 0;
			++cnt;
			++cnt4;
			break;
		case IDC_BUTTON6: // 6
			show[cnt4] = '6';
			number[cnt2][cnt] = 6;

			for (int i = 0; i <= cnt; ++i) {

				carry *= 10;
				if (i == 0) {
					carry = 1;
				}
				realnumber[cnt2] += carry*number[cnt2][cnt - i];
			}

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber[cnt2], FALSE);
			carry = 1;
			realnumber2[cnt2] = realnumber[cnt2];
			realnumber[cnt2] = 0;
			++cnt;
			++cnt4;
			break;
		case IDC_BUTTON5: // 5
			show[cnt4] = '5';
			number[cnt2][cnt] = 5;

			for (int i = 0; i <= cnt; ++i) {

				carry *= 10;
				if (i == 0) {
					carry = 1;
				}
				realnumber[cnt2] += carry*number[cnt2][cnt - i];
			}

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber[cnt2], FALSE);
			carry = 1;
			realnumber2[cnt2] = realnumber[cnt2];
			realnumber[cnt2] = 0;
			++cnt;
			++cnt4;
			break;
		case IDC_BUTTON4: // 4 
			show[cnt4] = '4';
			number[cnt2][cnt] = 4;

			for (int i = 0; i <= cnt; ++i) {

				carry *= 10;
				if (i == 0) {
					carry = 1;
				}
				realnumber[cnt2] += carry*number[cnt2][cnt - i];
			}

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber[cnt2], FALSE);
			carry = 1;
			realnumber2[cnt2] = realnumber[cnt2];
			realnumber[cnt2] = 0;
			++cnt;
			++cnt4;
			break;
		case IDC_BUTTON9: // 3
			show[cnt4] = '3';
			number[cnt2][cnt] = 3;

			for (int i = 0; i <= cnt; ++i) {

				carry *= 10;
				if (i == 0) {
					carry = 1;
				}
				realnumber[cnt2] += carry*number[cnt2][cnt - i];
			}

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber[cnt2], FALSE);
			carry = 1;
			realnumber2[cnt2] = realnumber[cnt2];
			realnumber[cnt2] = 0;
			++cnt;
			++cnt4;
			break;
		case IDC_BUTTON8: // 2
			show[cnt4] = '2';
			number[cnt2][cnt] = 2;

			for (int i = 0; i <= cnt; ++i) {

				carry *= 10;
				if (i == 0) {
					carry = 1;
				}
				realnumber[cnt2] += carry*number[cnt2][cnt - i];
			}

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber[cnt2], FALSE);
			carry = 1;
			realnumber2[cnt2] = realnumber[cnt2];
			realnumber[cnt2] = 0;
			++cnt;
			++cnt4;
			break;

		case IDC_BUTTON7: // 1
			show[cnt4] = '1';
			number[cnt2][cnt] = 1;

			for (int i = 0; i <= cnt; ++i) {

				carry *= 10;
				if (i == 0) {
					carry = 1;
				}
				realnumber[cnt2] += carry*number[cnt2][cnt - i];
			}

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber[cnt2], FALSE);
			carry = 1;
			realnumber2[cnt2] = realnumber[cnt2];
			realnumber[cnt2] = 0;
			++cnt;
			++cnt4;
			break;

		case IDC_BUTTON10: // 0
			show[cnt4] = '0';
			number[cnt2][cnt] = 0;

			for (int i = 0; i <= cnt; ++i) {

				carry *= 10;
				if (i == 0) {
					carry = 1;
				}
				realnumber[cnt2] += carry*number[cnt2][cnt - i];
			}

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber[cnt2], FALSE);
			++cnt;
			++cnt4;
			realnumber2[cnt2] = realnumber[cnt2];
			realnumber[cnt2] = 0;
			carry = 1;
			break;

		case IDC_BUTTON13:  // ³ª´°¼À
			show[cnt4] = '/';
			SetDlgItemText(hDlg, IDC_EDIT1, show);
			++cnt2;
			++cnt4;
			cnt = 0;
			if (flag != 5 && cnt2 == 2) {
				if (flag == 0) {
					sumnumber = realnumber2[0] / realnumber2[1];
				}
				else if (flag == 1) {
					sumnumber = realnumber2[0] * realnumber2[1];
				}
				else if (flag == 2) {
					sumnumber = realnumber2[0] - realnumber2[1];
				}
				else if (flag == 3) {
					sumnumber = realnumber2[0] + realnumber2[1];
				}
				realnumber[0] = sumnumber;
				cnt2 = 0;
			}

			flag = 0;

			break;

		case IDC_BUTTON14:  // °ö¼À
			show[cnt4] = 'X';
			SetDlgItemText(hDlg, IDC_EDIT1, show);
			++cnt2;
			++cnt4;
			cnt = 0;
			if (flag != 5 && cnt2 == 2) {
				if (flag == 0) {
					sumnumber = realnumber2[0] / realnumber2[1];
				}
				else if (flag == 1) {
					sumnumber = realnumber2[0] * realnumber2[1];
				}
				else if (flag == 2) {
					sumnumber = realnumber2[0] - realnumber2[1];
				}
				else if (flag == 3) {
					sumnumber = realnumber2[0] + realnumber2[1];
				}
				realnumber2[0] = sumnumber;
				cnt2 = 1;
			}

			flag = 1;
			break;
		case IDC_BUTTON15:  // »¬¼À
			show[cnt4] = '-';
			SetDlgItemText(hDlg, IDC_EDIT1, show);
			++cnt2;
			++cnt4;
			cnt = 0;
			if (flag != 5 && cnt2 == 2) {
				if (flag == 0) {
					sumnumber = realnumber2[0] / realnumber2[1];
				}
				else if (flag == 1) {
					sumnumber = realnumber2[0] * realnumber2[1];
				}
				else if (flag == 2) {
					sumnumber = realnumber2[0] - realnumber2[1];
				}
				else if (flag == 3) {
					sumnumber = realnumber2[0] + realnumber2[1];
				}
				realnumber2[0] = sumnumber;
				cnt2 = 1;
			}

			flag = 2;
			break;

		case IDC_BUTTON12:
			++cnt2;
			if (flag != 5 && cnt2 == 2) {
				if (flag == 0) {
					sumnumber = realnumber2[0] / realnumber2[1];
				}
				else if (flag == 1) {
					sumnumber = realnumber2[0] * realnumber2[1];
				}
				else if (flag == 2) {
					sumnumber = realnumber2[0] - realnumber2[1];
				}
				else if (flag == 3) {
					sumnumber = realnumber2[0] + realnumber2[1];
				}
				realnumber2[0] = sumnumber;
				cnt2 = 0;
			}

			SetDlgItemInt(hDlg, IDC_EDIT2, sumnumber, TRUE);
			for (int i = 0; i<10; ++i) {
				number[1][i] = 0;
			}

			for (int i = 0; i<128; ++i) {
				show[i] = '\0';
			}
			cnt4 = 0;
			sprintf(show2, "%d", realnumber2[0]);

			for (int i = 0; i<128; ++i) {
				show[i] = show2[i];
				if (show[i] != '\0') {
					++cnt4;
				}
			}

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			break;

		case IDC_BUTTON16: // µ¡¼À
			show[cnt4] = '+';
			SetDlgItemText(hDlg, IDC_EDIT1, show);
			++cnt2;
			++cnt4;
			cnt = 0;
			if (flag != 5 && cnt2 == 2) {
				if (flag == 0) {
					sumnumber = realnumber2[0] / realnumber2[1];
				}
				else if (flag == 1) {
					sumnumber = realnumber2[0] * realnumber2[1];
				}
				else if (flag == 2) {
					sumnumber = realnumber2[0] - realnumber2[1];
				}
				else if (flag == 3) {
					sumnumber = realnumber2[0] + realnumber2[1];
				}
				realnumber2[0] = sumnumber;
				cnt2 = 1;
			}

			flag = 3;
			break;

		case IDC_BUTTON17: // reverse
			cnt5 = cnt4;
			for (int i = cnt5 - 1; i >= 0; --i) {
				show3[(cnt5 - 1) - i] = show[i];
			}
			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemText(hDlg, IDC_EDIT2, show3);
			break;

		case IDC_BUTTON18: // µÞÀÚ¸® Áö¿ì±â
			show[cnt4 - 1] = '\0';
			--cnt4;

			if (show[cnt4 - 1] == '+' || show[cnt4 - 1] == '-' || show[cnt4 - 1] == 'X' || show[cnt4 - 1] == '/') {
				show[cnt4 - 1] = '\0';
				--cnt4;
				--cnt2;
				SetDlgItemText(hDlg, IDC_EDIT1, show);
				SetDlgItemInt(hDlg, IDC_EDIT2, realnumber2[0], TRUE);
				break;
			}
			if (flag != 5) {
				realnumber2[1] = realnumber2[1] / 10;
				SetDlgItemText(hDlg, IDC_EDIT1, show);
				SetDlgItemInt(hDlg, IDC_EDIT2, realnumber2[0], TRUE);
				break;
			}
			realnumber2[0] = realnumber2[0] / 10;
			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber2[0], TRUE);

			break;

		case IDC_BUTTON21:  // reset
			for (int i = 0; i<128; ++i) {
				show[i] = '\0';
			}
			cnt4 = 0;
			cnt2 = 0;
			cnt = 0;
			sumnumber = 0;
			realnumber2[0] = 0;
			realnumber2[1] = 0;

			SetDlgItemText(hDlg, IDC_EDIT1, show);
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber2[0], TRUE);
			break;

		case IDC_BUTTON22: // 2Áø¼ö
			num = atoi(show);
			cnt6 = 0;
			x = 0x80000000; // 1000 0000

			for (int i = 0; i<32; ++i) {
				if ((num&x) == 0x80000000)
					show4[cnt6] = '1';
				else
					show4[cnt6] = '0';
				num = num << 1;
				++cnt6;
			}
			SetDlgItemText(hDlg, IDC_EDIT2, show4);

			break;

		case IDC_BUTTON23: // 10Áø¼ö
			SetDlgItemInt(hDlg, IDC_EDIT2, realnumber2[0], TRUE);
			break;

		case IDCANCEL:
			DestroyWindow(hMDlg);
			hMDlg = NULL;
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
		EndPaint(hDlg, &ps);
		return 0;
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;

	MSG Message;
	WNDCLASSEX WndClass; // ±¸Á¶Ã¼ÀÇ Á¤ÀÇ
	g_hlnst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); 
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static BOOL flag = TRUE;
	static RECT rect;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		rect.left = 700;
		rect.top = 50;
		rect.right = 770;
		rect.bottom = 120;
		return 0;

	case WM_LBUTTONDOWN:
		if (!IsWindow(hMDlg)) {
			hMDlg = CreateDialog(g_hlnst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
			ShowWindow(hMDlg, SW_SHOW);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}