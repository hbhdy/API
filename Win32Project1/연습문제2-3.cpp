//100x 100으로 나눈다
//
//행열의 숫자가 1칸씩 커진다
//drawText함수 사용 박스의 가운데 정렬하여 쓴다.
//문자의 색상을 다양하게 설정한다.
#include <Windows.h>
#include <ctime>
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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1200,800,
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rect;
	char text[10];
	int temp1 = 0;
	rect.top = 0;
	rect.bottom = 20;
	rect.left = 0;
	rect.right = 20;
	int W = 1;
	srand(time(NULL));
	switch (iMessage)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
			for (int H = 1; H < 20; H++)
			{
				wsprintf(text, "%d,%d", W, H);
				SetBkColor(hDC, RGB(rand()%255, rand() % 255, rand() % 255));
				SetTextColor(hDC, RGB(rand()%255, rand() % 255, rand() % 255));
				DrawText(hDC, text, strlen(text), &rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
				W++;
				temp1 = rect.right;
				rect.left = rect.right;
				rect.right = (H+1) * 20 + temp1;
				rect.bottom = (H+1)*20;
			}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}