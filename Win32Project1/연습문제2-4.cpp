//2 - 4 ȭ�鿡 �ٵ��� ������� ����ϱ�
//
//ȭ�� 20 * 20ĭ 	��� 6���� ���� �����Ǽ��� ���Ѵ�.
//������ ���ڸ� ����Ͽ� �ٵ�ĭ �������(��, ��)�� ���� ����Ѵ�.
//���α׷��� ���� �����Ҷ����� ���ڴ� ���Ƿ� ��������.

#include <Windows.h>
#include <stdio.h>
#include <time.h>

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
		CW_USEDEFAULT, CW_USEDEFAULT,
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
	char text[20];
	char number[5];
	rect.left = 0;
	rect.right = 55;
	rect.top = 40;
	rect.bottom = 65;
	int num = 0;
	int set = 1;
	srand((unsigned int)time(NULL));
	switch (iMessage)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		num = rand() % 6 + 1;
		
		TextOut(hDC, 0, 0, "�������� :",10);
		sprintf(number, "%d", num);
		TextOut(hDC, 75, 0, number, 1);
		for (int i = 1; i < 21; i++) // ��
		{
				rect.top = 40;
				rect.bottom = 65;
				for (int j = 1; j < 21; j++) // ��
				{
					if (set == 1)
					{
						wsprintf(text, "(%d,%d)", j, i);
						DrawText(hDC, text, strlen(text), &rect, DT_CENTER);
						rect.top += 35;
						rect.bottom += 35;
						if (j%num == 0) {
							set = 2;
						}
					}
					else {
						if (set == 2 && j%num == 0) {
							set = 1;
						}
						rect.top += 35;
						rect.bottom += 35;
					}
				}
				rect.left += 55;
				rect.right += 55;
				if (num == 4)
					continue;
				else if (num == 6)
					continue;
				else if (set == 2)
					set = 1;
				else if(set==1)
					set = 2;
		}
	
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}