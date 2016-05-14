#include <Windows.h>
#include "resource.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
HINSTANCE g_hlnst;
LPCTSTR lpszClass = "파워 코딩";

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
		0, 0, 620, 620,
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

typedef struct Block {
	int x;
	int y;
}Block;
typedef struct Rec {
	int L;
	int R;
	int T;
	int B;
}Rec;

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
#endif

	PAINTSTRUCT ps;
	static RECT rt;
	static HDC hDC, MemDC;
	static HBITMAP img;
	static Block block1[9], block1Cpy[9], block2[16],block2Cpy[16], block3[25],block3Cpy[25];
	static Rec empty1[9], empty2[16], empty3[25];
	static int lineX, lineY = 0;
	static int randX, randY;
	static int mx, my;
	static int sel3[8], sel4[15], sel5[24];
	static int empsel3[9], empsel4[16], empsel5[25];
	srand((unsigned int)time(NULL));
	GetClientRect(hWnd, &rt);
	int B3 = 0;
	int emB3 = 0;
	int B4 = 0;
	int emB4 = 0;
	int B5 = 0;
	int emB5 = 0;

	switch (iMessage)
	{
	case WM_CREATE:
		img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP21)); // 바탕이미지

		for (int i = 0; i < 3; ++i)   // 3*3 빈블럭 좌표 생성
			for (int j = 0; j < 3; ++j) {
				empty1[emB3].L = j * 200;
				empty1[emB3].T = i * 200;
				empty1[emB3].R = (j+1) * 200;
				empty1[emB3].B = (i+1) * 200;			
				emB3++;
			}
		for (int i = 0; i < 4; ++i)   // 4*4 빈블럭 좌표 생성
			for (int j = 0; j < 4; ++j) {
				empty2[emB4].L = j * 150;
				empty2[emB4].T = i * 150;
				empty2[emB4].R = (j + 1) * 150;
				empty2[emB4].B = (i + 1) * 150;
				emB4++;
			}
		for (int i = 0; i < 5; ++i)   // 5*5 빈블럭 좌표 생성
			for (int j = 0; j < 5; ++j) {
				empty3[emB5].L = j * 120;
				empty3[emB5].T = i * 120;
				empty3[emB5].R = (j + 1) * 120;
				empty3[emB5].B = (i + 1) * 120;
				emB5++;
			}

		for (int i = 0; i < 8; i++)   // 왼쪽클릭시 위치값때문에
			sel3[i] = 0;
		for (int i = 0; i <9; i++)   // 오른쪽클릭시 위치값때문에
			empsel3[i] = 0;
		for (int i = 0; i < 15; i++)   // 왼쪽클릭시 위치값때문에
			sel4[i] = 0;
		for (int i = 0; i <16; i++)   // 오른쪽클릭시 위치값때문에
			empsel4[i] = 0;
		for (int i = 0; i < 24; i++)   // 왼쪽클릭시 위치값때문에
			sel5[i] = 0;
		for (int i = 0; i <25; i++)   // 오른쪽클릭시 위치값때문에
			empsel5[i] = 0;

		for (int i = 0; i < 3; ++i) // 위치값 복사 -> 그려줄때 랜덤 그려주기 위해 원래값과 복사값이 있어야함
			for (int j = 0; j < 3; ++j) {
				block1[B3].x = j * 200;
				block1Cpy[B3].x = j * 200;
				block1[B3].y = i * 200;
				block1Cpy[B3].y = i * 200;		
				B3++;
			}
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j) {
				block2[B4].x = j * 150;
				block2Cpy[B4].x = j * 150;
				block2[B4].y = i * 150;
				block2Cpy[B4].y = i * 150;
				B4++;
			}
		for (int i = 0; i < 5; ++i) 
			for (int j = 0; j < 5; ++j) {
				block3[B5].x = j * 120;
				block3Cpy[B5].x = j * 120;
				block3[B5].y = i * 120;
				block3Cpy[B5].y = i * 120;		
				B5++;
			}
		break;

	case WM_CHAR:
		if (wParam == 's' || wParam == 'S') {
			// 게임시작
			int arr3[9]{ 0 }; // 랜덤 값의 여부체크를 위함
			int arr4[16]{ 0 };
			int arr5[25]{ 0 };
			if (lineX == 3 && lineY == 3) {  // 복사 값을 랜덤한 원래위치에 넣음
				for (int i = 0; i < 8; ) {
					int randData3 = rand() % 8;
					if( arr3[randData3] == 0){
					block1[i].x = block1Cpy[randData3].x;
					block1[i].y = block1Cpy[randData3].y;
					arr3[randData3] = 1;
					++i;
					//printf("%d\n", i);
					}
				}
			}
			if (lineX == 4 && lineY == 4) {
				for (int i = 0; i < 15; ) {
					int randData4 = rand() % 15;
					if (arr4[randData4] == 0) {
						block2[i].x = block2Cpy[randData4].x;
						block2[i].y = block2Cpy[randData4].y;
						arr4[randData4] = 1;
						++i;
						//printf("%d\n", i);
					}
				}
			}
			if (lineX == 5 && lineY == 5) {
				for (int i = 0; i < 24; ) {
					int randData5 = rand() % 24;
					if (arr5[randData5] == 0) {
						block3[i].x = block3Cpy[randData5].x;
						block3[i].y = block3Cpy[randData5].y;
						arr5[randData5] = 1;
						++i;
						//printf("%d\n", i);
					}
				}
			}
		}
		if (wParam == 'f' || wParam == 'F') {
			// 그림전체보기
		}
		if (wParam == 'q' || wParam == 'Q') {
			// 종료
			PostQuitMessage(0);
		}
		if (wParam == '3') {
			// 그림 나누기 3 * 3
			lineX = 3;
			lineY = 3;
		}
		if (wParam == '4') {
			// 그림 나누기 4 * 4
			lineX = 4;
			lineY = 4;
		}
		if (wParam == '5') {
			// 그림 나누기 5 * 5
			lineX = 5;
			lineY = 5;
		}
		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		// 각 그림의 공간 체크 
		if (lineX == 3 && lineY == 3) {
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, 100, NULL);
			for (int i = 0; i < 8; ++i) {
				if (mx > block1[i].x && mx < block1[i].x + 200 && my > block1[i].y && my < block1[i].y + 200) {
					sel3[i] = 1;
				}
			}
		}
		if (lineX == 4 && lineY == 4) {
			KillTimer(hWnd, 2);
			SetTimer(hWnd, 2, 100, NULL);
			for (int i = 0; i < 15; ++i) {
				if (mx > block2[i].x && mx < block2[i].x + 150 && my > block2[i].y && my < block2[i].y + 150) {
					sel4[i] = 1;
				}
			}
		}
		if (lineX == 5 && lineY == 5) {
			KillTimer(hWnd, 3);
			SetTimer(hWnd, 3, 100, NULL);
			for (int i = 0; i < 24; ++i) {
				if (mx > block3[i].x && mx < block3[i].x + 120 && my > block3[i].y && my < block3[i].y + 120) {
					sel5[i] = 1;
				}
			}
		}
		break;

	case WM_RBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		// 각 ? * ? 의 빈공간 클릭 체크
		if (lineX == 3 && lineY == 3) {  
			for (int i = 0; i < 9; ++i) {
				if (mx > empty1[i].L && mx < empty1[i].R && my > empty1[i].T && my < empty1[i].B) {
					empsel3[i] = 1;
				}
			}
		}
		if (lineX == 4 && lineY == 4) {
			for (int i = 0; i < 16; ++i) {
				if (mx > empty2[i].L && mx < empty2[i].R && my > empty2[i].T && my < empty2[i].B) {
					empsel4[i] = 1;
				}
			}
		}
		if (lineX == 5 && lineY == 5) {
			for (int i = 0; i < 25; ++i) {
				if (mx > empty3[i].L && mx < empty3[i].R && my > empty3[i].T && my < empty3[i].B) {
					empsel5[i] = 1;
				}
			}
		}
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			for (int i = 0; i < 8;i++)
				for (int j = 0; j < 9; j++) {
					if (empsel3[j] == 1 && sel3[i] == 1) {
						if (block1[i].x < empty1[j].L) {
							//왼쪽이동
							block1[i].x += 5;
							if (block1[i].x >= empty1[j].L) {
								sel3[i] = 0;
								empsel3[j] = 0;
							}
						}
						else if (block1[i].y < empty1[j].T) {
							//아래이동
							block1[i].y += 5;
							if (block1[i].y >= empty1[j].T) {
								sel3[i] = 0;
								empsel3[j] = 0;
							}
						}
						else if (block1[i].x > empty1[j].L) {
							//오른쪽이동
							block1[i].x -= 5;
							if (block1[i].x <= empty1[j].L) {
								sel3[i] = 0;
								empsel3[j] = 0;
							}
						}
						else if (block1[i].y > empty1[j].T) {
							//위로이동
							block1[i].y -= 5;
							if (block1[i].y <= empty1[j].T) {
								sel3[i] = 0;
								empsel3[j] = 0;
							}
						}
					}
				}
			break;
		case 2:
			for (int i = 0; i < 15; i++)
				for (int j = 0; j < 16; j++) {
					if (empsel4[j] == 1 && sel4[i] == 1) {
						if (block2[i].x < empty2[j].L) {
							//왼쪽이동
							block2[i].x += 5;
							if (block2[i].x >= empty2[j].L) {
								sel4[i] = 0;
								empsel4[j] = 0;
							}
						}
						else if (block2[i].y < empty2[j].T) {
							//아래이동
							block2[i].y += 5;
							if (block2[i].y >= empty2[j].T) {
								sel4[i] = 0;
								empsel4[j] = 0;
							}
						}
						else if (block2[i].x > empty2[j].L) {
							//오른쪽이동
							block2[i].x -= 5;
							if (block2[i].x <= empty2[j].L) {
								sel4[i] = 0;
								empsel4[j] = 0;
							}
						}
						else if (block2[i].y > empty2[j].T) {
							//위로이동
							block2[i].y -= 5;
							if (block2[i].y <= empty2[j].T) {
								sel4[i] = 0;
								empsel4[j] = 0;
							}
						}
					}
				}
			break;
		case 3:
			for (int i = 0; i < 24; i++)
				for (int j = 0; j < 25; j++) {
					if (empsel5[j] == 1 && sel5[i] == 1) {
						if (block3[i].x < empty3[j].L) {
							//왼쪽이동
							block3[i].x += 5;
							if (block3[i].x >= empty3[j].L) {
								sel5[i] = 0;
								empsel5[j] = 0;
							}
						}
						else if (block3[i].y < empty3[j].T) {
							//아래이동
							block3[i].y += 5;
							if (block3[i].y >= empty3[j].T) {
								sel5[i] = 0;
								empsel5[j] = 0;
							}
						}
						else if (block3[i].x > empty3[j].L) {
							//오른쪽이동
							block3[i].x -= 5;
							if (block3[i].x <= empty3[j].L) {
								sel5[i] = 0;
								empsel5[j] = 0;
							}
						}
						else if (block3[i].y > empty3[j].T) {
							//위로이동
							block3[i].y -= 5;
							if (block3[i].y <= empty3[j].T) {
								sel5[i] = 0;
								empsel5[j] = 0;
							}
						}
					}
				}
			break;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_PAINT:

		hDC = BeginPaint(hWnd, &ps);
		MemDC = CreateCompatibleDC(hDC);
		SelectObject(MemDC, img);
		if (lineX == 3 && lineY == 3) {  // 3 * 3 그림 생성
			for (int i = 0; i < 8; ++i) {
				StretchBlt(hDC, block1[i].x, block1[i].y, 200, 200, MemDC, block1Cpy[i].x, block1Cpy[i].y, 200, 200, SRCCOPY);
				//printf("%d, %d\n", block1[i].x, block1[i].y);
			}
		}
		if (lineX == 4 && lineY == 4) { // 4 * 4 그림 생성
			for (int i = 0; i < 15; ++i) {
				StretchBlt(hDC, block2[i].x, block2[i].y, 150, 150, MemDC, block2Cpy[i].x, block2Cpy[i].y, 150, 150, SRCCOPY);
				//printf("%d, %d\n", block1[i].x, block1[i].y);
			}
		}
		if (lineX == 5 && lineY == 5) { // 5 * 5 그림 생성
			for (int i = 0; i < 24; ++i) {
				StretchBlt(hDC, block3[i].x, block3[i].y, 120, 120, MemDC, block3Cpy[i].x, block3Cpy[i].y, 120, 120, SRCCOPY);
				//printf("%d, %d\n", block1[i].x, block1[i].y);
			}
		}
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}