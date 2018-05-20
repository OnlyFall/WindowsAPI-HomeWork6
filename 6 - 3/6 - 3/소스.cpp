//델몬트 바나나는 세계 제일
//아 move 못해먹겠어요 ㅠㅠ

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");
BOOL CALLBACK Dlg_1Proc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //h의 의미? = 핸들
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hinstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //배경색
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //프로그램 내에서 보이는 커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘 
	WndClass.hInstance = hinstance; //현재 실행되고 있는 객체의 핸들
	WndClass.lpfnWndProc = WndProc; //프로시저함수의 이름 
	WndClass.lpszClassName = lpszClass; //윈도우 클래스 내용의 이름 
	WndClass.lpszMenuName = NULL; // 메뉴바 이름
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //윈도우 출력 스타일
											  // 여기까지 WNDCLASS구조체의 변수들에 값을 대입

	RegisterClass(&WndClass); // 윈도우 클래스를 운영체제에 등록

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 10, 10, 1000, 800, NULL, (HMENU)NULL, hinstance, NULL);
	//CreateWindow(클래스 이름,타이틀 이름,윈도우 스타일, 윈도우 위치좌표x , y,윈도우 가로크기, 윈도우 세로크기, 부모 윈도우 핸들, 메뉴 핸들, 응용프로그램 인스턴스, 생성 윈도우 정보(NULL))
	ShowWindow(hWnd, nCmdShow); //윈도우의 화면 출력
	UpdateWindow(hWnd); //OS에 WM_PAINT메시지 전송
	while (GetMessage(&Message, NULL, 0, 0))
	{
		//윈도우 프로시저에서 PostQuitMessage()를 호출할때 종료됨.
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}//메시지루프를 돌림.
	return (int)Message.wParam;
}

//전역 함수 모음----
static BOOL Start, Shape;
static double spring_x, move_xx , move_yy;
static int Sh_count = 0;
HWND hWnd2;
static int color_R, color_G, color_B; //색깔 지정
static int zig_count, zigzag = 0;
static BOOL Left , Move , Stop ,Sin, Zigzag, Spring = FALSE; //버튼
static int posX, posY;
static double f;
static double y;
static BOOL moveCheck = FALSE;
//-----------
#define PIE 3.141592

//수학 함수
double rad(double degree) {
	return PIE * degree / 180.0;
}//라디안 변환 함수

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	hWnd2 = hWnd;
	PAINTSTRUCT ps;
	HDC hdc;
	static int id;
	HWND hButton;
	HPEN hPen, holdPen;
	HBRUSH hBrush, oldBrush;
	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		posX = 500;
		posY = 400;
		spring_x = 0;
		break;

		//이동하거나 뒤집어 지는거 구현을 위한 타이머 입니다.
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
				move_xx += 10;
				if (move_xx > 100) {
					move_xx = 0;
				}//초기화 해준다.
			break;
		case 2:

			if (move_yy > 0)
				moveCheck = FALSE;
			if (move_yy < -200)
				moveCheck = TRUE;

			if (moveCheck == FALSE)
				move_yy -= 2;
			else
				move_yy += 2;
			
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_CHAR:
		if (wParam == 'A' || wParam == 'a') {
			DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg_1Proc);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		//이동 시 타이머 설정
		if (Left)
		{
			SetTimer(hWnd, 1, 100, NULL);
		}
		else if (Move) {
			SetTimer(hWnd, 2, 100, NULL);
		}
		else if(Stop) {
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
		}

		//가운데 격자 무늬------------------
		MoveToEx(hdc, 500, 800, NULL);
		LineTo(hdc, posX, 0);

		MoveToEx(hdc, 1000, 400, NULL);
		LineTo(hdc, 0, 400);
		//----------------------------------
		
		hPen = CreatePen(PS_SOLID, 4 ,RGB(color_R, color_G, color_B));
		holdPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		//도형 선택하기
		if (Zigzag) {
			//지그 재그 시작
			MoveToEx(hdc, posX - 500 - move_xx, posY - 100 - move_yy, NULL);
			for (int i = 1; i < 100; i++) {
				if (i % 2 == 1) {
					Zigzag = 1; //지그재그 위 아래 방향
				}
				else {
					Zigzag = (-1);
				}
				LineTo(hdc, posX - 500 - move_xx + 50 * i, posY + Zigzag * 100 + Zigzag * move_yy);
			}
		}
		if (Sin) {
			MoveToEx(hdc, -2000, 0, NULL);
			LineTo(hdc, 0, 2000);
			//출처 네이버 블로그
			for (f = -500; f < 1000; f++) {
				if (Move) {
					y = (int)(sin(f*3.14 / 180) * 100 * (move_yy / 100) ) + 400;
					SetPixel(hdc, (int)f - move_xx, y, RGB(color_R, color_G, color_B));
					}
				else {
					y = (int)(sin(f*3.14 / 180) * 100) + 400;
					SetPixel(hdc, (int)f - move_xx, y, RGB(color_R, color_G, color_B));
				}
			}
		}
		if (Spring) {
			//스프링이 이상해 ㅠㅠ
			spring_x = 0;
			MoveToEx(hdc, 100 - move_xx, posY, NULL);
			ArcTo(hdc, 200 + spring_x - move_xx, 200, 400 + spring_x - move_xx, 400,
				posX + 400, 400 + spring_x, 400 + spring_x - move_xx, 400 + spring_x);
			for (int i = 0; i < 10; i++) {
				spring_x += 60;
				ArcTo(hdc, 200 + spring_x - move_xx, 200, 400 + spring_x - move_xx, 400, 
					posX + 400 - move_xx, 400 + spring_x, 400 + spring_x - move_xx, 400 + spring_x);
				//Arc 함수 차례대로 left right top bottom x y start ,, s y end
			}

		}
		//	Arc(hdc, posX- 100 + 10, posY - 100 , posX + 100 + 10, posY + 100,
		//	50, 0, 800, 800);
		
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		SelectObject(hdc, holdPen);
		DeleteObject(hPen);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}
//이것도 나름 윈도우 창
BOOL CALLBACK Dlg_1Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HWND hButton;
	hWnd2 = hDlg;
	PAINTSTRUCT d_ps;
	HBRUSH hBrush, oldBrush;
	//체크박스 용
	static int check_red, check_blue, check_green, check_mix =0;
	
	static int posX, posY, plusX;
	switch (iMessage)
	{
	case WM_INITDIALOG:
		//윈도우의 create랑 같은 원리
		posX = 100; posY = 100;
		plusX = 0;
		Start = FALSE;
		Shape = TRUE;
		Stop = TRUE;
		color_G = 0;
		color_R = 0;
		color_B = 0;
		break;
	
	case WM_COMMAND:
		//hdc = GetDC(hDlg);
		switch (LOWORD(wParam)) {

		//체크 버튼_색깔 변환--------
		case IDC_GREEN:
			check_green++;
			if (check_green % 2 == 1) {
				color_G = 255;
			}
			else {
				color_G = 0;
			}
			break;
		case IDC_RED:
			check_red++;
			if (check_red % 2 == 1) {
				color_R = 255;
			}
			else {
				color_R = 0;
			}
			break;
		case IDC_BLUE:
			check_blue++;
			if (check_blue % 2 == 1) {
				color_B = 255;
			}
			else {
				color_B = 0;
			}
			break;
		case IDC_MIX:
			//믹스 일때 주의점 :: 믹스를 끄면 원래 색으로 돌아오기
			check_mix++;
			if (check_mix % 2 == 1) {
				color_R = 0;
				color_G = 0;
				color_B = 0;
			}
			else {
				if (check_green % 2 == 1) {
					color_G = 255;
				}
				if (check_red % 2 == 1) {
					color_R = 255;
				}
				 if (check_blue % 2 == 1) {
					color_B = 255;
				}
			}
			break;
		//--------------

		//버튼------------
		case IDC_RESET:
			//초기화 해주기
			color_R = 0;
			color_G = 0;
			color_B = 0;
			Sin = FALSE;
			Zigzag = FALSE;
			Spring = FALSE;
			Stop = TRUE;
			move_xx = 0;
			move_yy = 0;
			break;

		case IDC_LEFT:
			Left = TRUE;

			Stop = FALSE;
			Move = FALSE;
			//	move_xx+= 10;
			break;

		case IDC_MOVE:
			Move = TRUE;

			Stop = FALSE;
			Left = FALSE;
			break;

		case IDC_STOP:
			Stop = TRUE;
			Left = FALSE;
			Move = FALSE;
			break;

		case IDC_SIN:
			Sin = TRUE;

			Zigzag = FALSE;
			Spring = FALSE;
			break;

		case IDC_Zigzag:
			Zigzag = TRUE;
			
			Sin = FALSE;
			Spring = FALSE;
			break;

		case IDC_Spring:
			Spring = TRUE;

			Sin = FALSE;
			Zigzag = FALSE;
			break;
		//--------------

		//시스템-----------------
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;

		//지정 값
		}


		WndProc(hDlg, wParam, InvalidateRect(NULL, NULL, TRUE), lParam);
		
		break;
	


	}
	return 0;
}


