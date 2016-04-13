#include <windows.h> 
#include <math.h>

#define BSIZE 40

float LengthPts(int x1, int y1, int x2, int y2)
{
	return(sqrt((float)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))));
}

BOOL InRectangle(int x, int y, int mx, int my)
{
	if (LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}






HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	// 윈도우 클래스 구조체 값 설정 
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	// 윈도우 클래스 등록 
	RegisterClass(&WndClass);

	// 윈도우 생성 
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	// 윈도우 출력 
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 이벤트 루프 처리 
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

RECT rect;

typedef struct circle {
	double centerX;
	double centerY;
	int vecX;
	int vecY;

}Circle;

Circle moveCircle;

static int speed = 200;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;

	GetClientRect(hWnd, &rect);

	static int x, y, flag;
	static bool Selection;
	int mx, my;


	switch (uMsg) {

	case WM_CREATE:
		//speed = 200;


		SetTimer(hWnd, 1, 1000, NULL);


		x = rect.right / 2;
		y = rect.bottom - 10;

		moveCircle.centerX = rect.right / 2;
		moveCircle.centerY = rect.bottom - 40;
		moveCircle.vecX = 20;
		moveCircle.vecY = 20;



		flag = 0;
		Selection = false;
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			SetTimer(hWnd, 2, speed, NULL);

			break;


		case 2:
			moveCircle.centerX -= moveCircle.vecX;
			moveCircle.centerY -= moveCircle.vecY;

			if (moveCircle.centerX - 10 <= rect.left)
				moveCircle.vecX *= -1;

			else if (moveCircle.centerY - 10 <= rect.top)
				moveCircle.vecY *= -1;

			else if (moveCircle.centerX + 10 >= rect.right)
				moveCircle.vecX *= -1;

			else if (moveCircle.centerY + 10 >= y - 20)
				moveCircle.vecY *= -1;


			InvalidateRect(hWnd, NULL, true);


			break;
		}

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		MyBrush = CreateSolidBrush(RGB(0, 255, 255));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);

		Rectangle(hdc, x - BSIZE, y - 20, x + BSIZE, y);

		SelectObject(hdc, OldBrush);
		DeleteObject(MyBrush);


		MyBrush = CreateSolidBrush(RGB(255, 0, 255));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);

		Ellipse(hdc, moveCircle.centerX - 10, moveCircle.centerY - 10, moveCircle.centerX + 10, moveCircle.centerY + 10);

		SelectObject(hdc, OldBrush);
		DeleteObject(MyBrush);

		EndPaint(hWnd, &ps);
		break;


	case WM_KEYDOWN:

		if (wParam == 'm' || wParam == 'M') {
			flag = 1;
		}

		else if (wParam == 's' || wParam == 'S') {
			flag = 2;
		}

		else if (wParam == VK_F1)
			speed += 25;

		else if (wParam == VK_F2)
			speed -= 25;

		break;

	case WM_LBUTTONDOWN:

		//마우스 좌표 받음
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (InRectangle(x, y, mx, my))
			Selection = true;

		InvalidateRect(hWnd, NULL, true);

		break;

	case WM_LBUTTONUP:

		if (flag == 2) {

			x = rect.right / 2;
		}

		InvalidateRect(hWnd, NULL, true);
		Selection = false;

		break;


	case WM_MOUSEMOVE:

		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (Selection) {
			x = mx;

			InvalidateRect(hWnd, NULL, true);
		}
		break;


	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 나머지는 OS로 
}