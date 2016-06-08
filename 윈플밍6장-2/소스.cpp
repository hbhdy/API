#include <Windows.h>
#include "resource.h"
#include <atlimage.h>  // 추가 추가 추가 

HINSTANCE g_hlnst;
LPCTSTR lpszClass = "파워 코딩";
static bool JumpKey = false;
static bool FrontKey = false;
static bool BackKey = false;
static int speed = 100;
static int xpos, ypos;
static bool enemyC = false;
static bool change = false;
static bool sizeDobule = false;
static bool sizeHalf = false;
static bool textView = false;
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM IParam);
BOOL CALLBACK Dlg1Proc(HWND, UINT, WPARAM, LPARAM);

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
		0, 0, 800, 600,
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
	static RECT rt;
	static HDC hDC, MemDC, Mem1DC, Mem2DC;
	static HBITMAP enemyRunBit[3], RunBit[8], DieBit[4], back1, back2, back1Copy, back2Copy, hBitmap, hBitmap2, RunBit2[2], enemyRunBit2[3];
	static HINSTANCE hInstance;
	//static int xpos, ypos;  // 캐릭터 좌표
	static int enemyXpos, enemyYpos;
	static int jumpState = 1;  // 1은 위로 0은 아래로
	static int jumpcount; // 점프 시간 측정
	static int glassX, copyglassX;
	static int skyX, copyskyX;
	static int count1, count2, count3, count4;
	static int Dietime = 0;
	static int Txpos, Typos;
	static bool collision = false;
	static int direction = 1;
	char harmar[100];
	GetClientRect(hWnd, &rt);

	switch (iMessage)
	{
	case WM_CREATE:
		glassX = 0;
		copyglassX = 800;
		skyX = 0;
		copyskyX = 800;
		xpos = 100;
		ypos = 425;
		Txpos = 300;
		Typos = 100;
		enemyXpos = 700;
		enemyYpos = 450;
		// 반대 캐릭터 이미지 ( 오른쪽으로 달림 )
		enemyRunBit[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP4));
		enemyRunBit[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP5));
		enemyRunBit[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP6));
		//반대 캐릭터 다른그림 ( 오른쪽으로 달림)
		enemyRunBit2[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP21));
		enemyRunBit2[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP22));
		enemyRunBit2[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP23));
		// 땅 배경
		back1 = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP1));
		back1Copy = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP1));
		// 하늘 배경
		back2 = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP2));
		back2Copy = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP3));


		// 주인공1 ( 왼쪽으로 달림)  
		RunBit[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP7));
		RunBit[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP8));
		RunBit[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP9));
		RunBit[3] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP10));
		RunBit[4] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP11));
		RunBit[5] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP12));
		RunBit[6] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP13));
		RunBit[7] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP14));
		// 주인공2 (왼쪽으로 달림)
		RunBit2[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP19));
		RunBit2[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP20));

		// 폭팔
		DieBit[0] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP15));
		DieBit[1] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP16));
		DieBit[2] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP17));
		DieBit[3] = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP18));

		SetTimer(hWnd, 1, speed, NULL);
		SetTimer(hWnd, 2, speed, NULL);
		SetTimer(hWnd, 3, speed, NULL);
		break;
	/*case WM_KEYDOWN:
		if (wParam == VK_RIGHT) {
			xpos += 10;
		}
		if (wParam == VK_LEFT) {
			xpos -= 10;
		}
		if (wParam == VK_SPACE) {
			SetTimer(hWnd, 2, 100, NULL);
			jumpState = 1;
		}
		InvalidateRect(hWnd, NULL, false);
		break;*/

	case WM_LBUTTONDOWN: // 마우스 클릭하면 대화상자 띄우기
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg1Proc);
		break;

	case WM_TIMER:
		KillTimer(hWnd, 1);
		SetTimer(hWnd, 1, speed, NULL);
		Txpos+=direction*5;
		if (Txpos >= 600)
			direction = -1;
		if (Txpos <= 200)
			direction = 1;
		
		
		count1++;
		count1 = count1 % 8;
		count2++;
		count2 = count2 % 3;
		count4++;
		count4 = count4 % 2;
		switch (wParam) {
		case 1: //  sky 및 glass
			glassX -= 50;
			copyglassX -= 50;
			skyX -= 20;
			copyskyX -= 20;
			if (skyX <= -800) {
				skyX = 0;
				copyskyX = 800;
			}
			if (glassX <= -800) {
				glassX = 0;
				copyglassX = 800;
			}
			enemyXpos -= 20;
			if (enemyXpos <= -10)
				enemyXpos = 700;
			break;
		case 2:  // 점프
			if (JumpKey == true) {
				if (jumpState == 1) {
					ypos -= 25;
					xpos += 15;
					jumpcount++;
					if (jumpcount == 5) {
						jumpState = 2;
					}
				}
				if (jumpState == 2) {
					ypos += 25;
					xpos += 15;
					jumpcount--;
					if (jumpcount == 0) {
						jumpState = 1;
						if (JumpKey == true) {
							JumpKey = false;
						}
					}
				}
			}
			break;
		case 3:
			if ((xpos + 70 >= enemyXpos && xpos < enemyXpos) && ypos + 80 >= enemyYpos) {
				collision = true;
			}
			if (collision == true) {
				Dietime++;
				count3++;
				count3 = count3 % 4;
				if (Dietime >= 8) {
					collision = false;
					Dietime = 0;
				}
			}
			break;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		MemDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, 800, 600);  // Mem1DC에 바탕그리기
		SelectObject(MemDC, hBitmap);  // 바탕 설정
		Mem1DC = CreateCompatibleDC(MemDC);
		Mem2DC = CreateCompatibleDC(MemDC); // 1DC를 기반으로 2DC 생성
		SelectObject(Mem2DC, back2); // 하늘
		SelectObject(Mem2DC, back2Copy);
		SelectObject(Mem1DC, back1); // 땅
		SelectObject(Mem1DC, back1Copy);

		BitBlt(MemDC, skyX, 0, 800, 600, Mem2DC, 0, 0, SRCCOPY); // 배경 옆으로  넘어감 밑에도 같음
		BitBlt(MemDC, copyskyX, 0, 800, 600, Mem2DC, 0, 0, SRCCOPY);

		TransparentBlt(MemDC, glassX, 500, 800, 60, Mem1DC, 0, 0, 800, 60, RGB(255, 255, 255));  // 투명화 후 1DC에 바닥그림 ( 첫번쨰 )
		TransparentBlt(MemDC, copyglassX, 500, 800, 60, Mem1DC, 0, 0, 800, 60, RGB(255, 255, 255)); // 투명화 후 1D에 바닥그림 ( 두번째 ) 즉 옆으로 넘어감
		if (sizeDobule == true) {
			if (change == false) {
				SelectObject(Mem1DC, RunBit[count1]); // 캐릭터 이미지
				TransparentBlt(MemDC, xpos, ypos - 90 , 200, 200, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));
			}
			if (change == true) {
				SelectObject(Mem1DC, RunBit2[count4]); // 캐릭터 이미지
				TransparentBlt(MemDC, xpos, ypos - 120, 200, 200, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));
			}
		}
		else if (sizeHalf == true) {
			if (change == false) {
				SelectObject(Mem1DC, RunBit[count1]); // 캐릭터 이미지
				TransparentBlt(MemDC, xpos, ypos+40, 50, 50, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));
			}
			if (change == true) {
				SelectObject(Mem1DC, RunBit2[count4]); // 캐릭터 이미지
				TransparentBlt(MemDC, xpos, ypos +20, 50, 50, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));
			}
		}
		else {
			if (change == false) {
				SelectObject(Mem1DC, RunBit[count1]); // 캐릭터 이미지
				TransparentBlt(MemDC, xpos, ypos, 100, 100, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));
			}
			if (change == true) {
				SelectObject(Mem1DC, RunBit2[count4]); // 캐릭터 이미지
				TransparentBlt(MemDC, xpos, ypos - 20, 100, 100, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));
			}
		}
		SelectObject(Mem1DC, enemyRunBit[count2]); // 적캐릭터 이미지
		TransparentBlt(MemDC, enemyXpos, enemyYpos, 60, 60, Mem1DC, 0, 0, 76, 100, RGB(255, 255, 255));
		if (enemyC == true) {
			SelectObject(Mem1DC, enemyRunBit2[count2]); // 적캐릭터 이미지
			TransparentBlt(MemDC, enemyXpos+50, enemyYpos, 60, 60, Mem1DC, 0, 0, 76, 100, RGB(255, 255, 255));
		}

		if (collision == true) {
			SelectObject(Mem1DC, DieBit[count3]);
			TransparentBlt(MemDC, enemyXpos - 150, 300, 300, 300, Mem1DC, 0, 0, 100, 100, RGB(255, 255, 255));
		}
		if (textView == true)
		{
			wsprintf(harmar, TEXT("조교님 굽신굽신~ ㅎㅎㅎ "));
			TextOut(MemDC,Txpos ,Typos, harmar, strlen(harmar));

		}

		BitBlt(hDC, 0, 0, 800, 600, MemDC, 0, 0, SRCCOPY);



		DeleteDC(Mem2DC);
		DeleteDC(Mem1DC);
		DeleteDC(MemDC);
		//DeleteDC(hDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK Dlg1Proc(HWND hWnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam) // 대화상자 메시지 처리함수
{
	HDC hDC;
	PAINTSTRUCT ps;
	static RECT rect;
	static HINSTANCE hInstance;

	switch (iMsg) {
	case WM_CREATE:
		g_hlnst = hInstance;
		break;
	
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: // 점프
			JumpKey = true;
			break;
		case IDC_BUTTON2: // 앞이동
			xpos += 30;
			break;
		case IDC_BUTTON3: // 뒤이동
			xpos -= 30;
			break;
		case IDC_BUTTON4: // 스피드업
			speed -= 10;
			break;
		case IDC_BUTTON5: //스피드다운
			speed += 10;
			break;
		case IDC_BUTTON6: //프로그램 종료
			PostQuitMessage(0);
			break;
		case IDC_CHECK1: // 화면에 글 좌우 이동
			if (textView == false)
				textView = true;
			else
				textView = false;
			break;
		case IDC_CHECK2: // 몬스터뒤 다른그림 생성
			if (enemyC == false)
				enemyC = true;
			else
				enemyC = false;
			break;
		case IDC_RADIO1: // 캐릭터 1
			change = false;
			break;
		case IDC_RADIO2: // 캐릭터 2
			change = true;
			break;
		case IDC_RADIO3: // 캐릭터 확대 2배
			sizeDobule = true;
			sizeHalf = false;
			break;
		case IDC_RADIO4: // 캐릭터 축소 0.5배
			sizeDobule = false;
			sizeHalf = true;
			break;
		case IDC_RADIO5: // 크기 제자리
			sizeDobule = false;
			sizeHalf = false;
			break;
		}
		break;
	}
	return 0;
}