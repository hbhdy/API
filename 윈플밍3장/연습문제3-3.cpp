#include <Windows.h>

HINSTANCE g_hlnst;
LPCTSTR lpszClass = "window program 1 - 2";

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
		0,0,1200,800,
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

typedef struct REC {
	int L;
	int T;
	int R;
	int B;
}REC;

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rect;
	HDC hDC;
	HBRUSH hBrush, oldBrush;
	static REC rec[6];  //사각형 그린다. 자동차~
	static REC lamp[3];
	static int wide;
	static int high;
	//static bool Bb = true;  // 이동시작
	//static bool Yy = true; // 3초후 정지
	//static bool Rr = true; // 바로 정지
	static int stopTime = 0;
	


	switch (iMessage)
	{
	case WM_CREATE:
		// 원래 차의 위치
		rec[0].L = 50;
		rec[0].R = 200;
		rec[0].T = 100;
		rec[0].B = 250;
		rec[1].L = 900;
		rec[1].R = 1050;
		rec[1].T = 300;
		rec[1].B = 450;
		rec[2].L = 50;
		rec[2].R = 200;
		rec[2].T = 500;
		rec[2].B = 650;

		//숨은 차 ㅋㅋㅋㅋㅋㅋㅋ
		rec[3].L = -150;
		rec[3].R = 0;
		rec[3].T = 100;
		rec[3].B = 250;
		rec[4].L = 1200;
		rec[4].R = 1350;
		rec[4].T = 300;
		rec[4].B = 450;
		rec[5].L = -150;
		rec[5].R = 0;
		rec[5].T = 500;
		rec[5].B = 650;

		lamp[0].L = 900;
		lamp[0].R = 980;
		lamp[0].T = 10;
		lamp[0].B = 90;
		lamp[1].L = 1000;
		lamp[1].R = 1080;
		lamp[1].T = 10;
		lamp[1].B = 90;
		lamp[2].L = 1100;
		lamp[2].R = 1180;
		lamp[2].T = 10;
		lamp[2].B = 90;

		
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_CHAR:
		if (wParam == 'B' || wParam == 'b')
		{
			SetTimer(hWnd, 1, 60, NULL);
			SetTimer(hWnd, 2, 80, NULL);
			SetTimer(hWnd, 3, 100, NULL);
		}
		if (wParam == 'R' || wParam == 'r')
		{
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
		}
		if (wParam == 'Y' || wParam == 'y')
		{
			SetTimer(hWnd, 4, 100, NULL);
		}
		

		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			rec[0].L += 40;
			rec[0].R += 40;
			if (rec[0].R > 1200)
			{
				rec[3].L += 40;
				rec[3].R += 40;
				if (rec[3].R >= 150)
				{
					rec[0].L = rec[3].L;
					rec[0].R = rec[3].R;
					rec[3].L = -150;
					rec[3].R = 0;
				}
			}
			break;
		case 2:
			rec[1].L -= 40;
			rec[1].R -= 40;
			if (rec[1].L < 0)
			{
				rec[4].L -= 40;
				rec[4].R -= 40;
				if (rec[4].L < 1050)
				{
					rec[1].L = rec[4].L;
					rec[1].R = rec[4].R;
					rec[4].L = 1200;
					rec[4].R = 1350;
				}
			}
			break;
		case 3:
			rec[2].L += 40;
			rec[2].R += 40;
			if (rec[2].R > 1200)
			{
				rec[5].L += 40;
				rec[5].R += 40;
				if (rec[5].R >= 150)
				{
					rec[2].L = rec[5].L;
					rec[2].R = rec[5].R;
					rec[5].L = -150;
					rec[5].R = 0;
				}
			}
			break;
		case 4:
			stopTime++;
			if (stopTime >=30)
			{
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				stopTime = 0;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		GetClientRect(hWnd, &rect);
		hDC = BeginPaint(hWnd, &ps);
		wide = rect.right / 3;
		high = rect.bottom;
		for (int i = 0; i < 3; i++)
		{
			Rectangle(hDC, i*wide, 0, (i + 1)*wide, high); // left top right bottom
		}

		Rectangle(hDC, rec[0].L, rec[0].T, rec[0].R, rec[0].B);
		Rectangle(hDC, rec[3].L, rec[3].T, rec[3].R, rec[3].B);
		Rectangle(hDC, rec[1].L, rec[1].T, rec[1].R, rec[1].B);
		Rectangle(hDC, rec[4].L, rec[4].T, rec[4].R, rec[4].B);
		Rectangle(hDC, rec[2].L, rec[2].T, rec[2].R, rec[2].B);
		Rectangle(hDC, rec[5].L, rec[5].T, rec[5].R, rec[5].B);
		
		hBrush = CreateSolidBrush((RGB(0, 0, 255)));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, lamp[0].L, lamp[0].T, lamp[0].R, lamp[0].B);
		hBrush = CreateSolidBrush((RGB(255, 255, 0)));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, lamp[1].L, lamp[1].T, lamp[1].R, lamp[1].B);
		hBrush = CreateSolidBrush((RGB(255, 0, 0)));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, lamp[2].L, lamp[2].T, lamp[2].R, lamp[2].B);


		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}