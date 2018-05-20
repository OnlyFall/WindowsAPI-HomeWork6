#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>
#include <time.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

BOOL CollisionCheck(int x, int y, int mx, int my);


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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 1920, 1080, NULL, (HMENU)NULL, hinstance, NULL);
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

static int n;
static int RectangleCheck;
static int color_R, color_G, color_B;

BOOL CALLBACK Dlg6_3Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hButton;
	HDC hdc;
	static int x = 0, y = 100;
	static int count = 0;
	static int dir = 0;
	static int xdir = 0;
	static HBITMAP hBitmap;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		hButton = GetDlgItem(hDlg, ID_STOP);
		EnableWindow(hButton, FALSE); // 최초 정지 버튼은 비활성화
		hButton = GetDlgItem(hDlg, ID_MOTHER_STOP);
		EnableWindow(hButton, FALSE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_START: // 시작 버튼
			hButton = GetDlgItem(hDlg, ID_START);
			EnableWindow(hButton, FALSE); // 시작버튼 비활성화
			hButton = GetDlgItem(hDlg, ID_STOP);
			EnableWindow(hButton, TRUE); // 정지버튼 활성화
			SetTimer(hDlg, 1, 100, NULL);
			hdc = GetDC(hDlg);
			Ellipse(hdc, x, y, x + 10, y + 10);
			ReleaseDC(hDlg, hdc);
			break;
		case ID_STOP: // 정지 버튼
			hButton = GetDlgItem(hDlg, ID_START);
			EnableWindow(hButton, TRUE); // 시작버튼 활성화
			hButton = GetDlgItem(hDlg, ID_STOP);
			KillTimer(hDlg, 1);
			EnableWindow(hButton, FALSE); // 정지버튼 비활성화
			break;
		case ID_CLOSE: // 종료 버튼
			EndDialog(hDlg, 0);
			break;

		case ID_MOTHER_START:
			n = 1;
			hButton = GetDlgItem(hDlg, ID_MOTHER_STOP);
			EnableWindow(hButton, TRUE);
			hButton = GetDlgItem(hDlg, ID_MOTHER_START);
			EnableWindow(hButton, FALSE);
			WndProc(hDlg, wParam, InvalidateRect(NULL, NULL, TRUE), lParam);
			break;

		case ID_MOTHER_STOP:
			hButton = GetDlgItem(hDlg, ID_MOTHER_STOP);
			EnableWindow(hButton, FALSE);
			hButton = GetDlgItem(hDlg, ID_MOTHER_START);
			EnableWindow(hButton, TRUE);
			n = 0;
			WndProc(hDlg, wParam, InvalidateRect(NULL, NULL, TRUE), lParam);
			break;

		case PROGRAM_CLOSE:
			PostQuitMessage(0);
			break;

		case IDC_RADIO1:
			RectangleCheck = 1;
			WndProc(hDlg, wParam, InvalidateRect(NULL, NULL, TRUE), lParam);
			InvalidateRect(hDlg, NULL, TRUE);
			break;
		case IDC_RADIO2:
			RectangleCheck = 0;
			WndProc(hDlg, wParam, InvalidateRect(NULL, NULL, TRUE), lParam);
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDC_RADIO3:
			color_R = 255;
			color_G = 0;
			WndProc(hDlg, wParam, InvalidateRect(NULL, NULL, TRUE), lParam);
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDC_RADIO4:
			color_R = 0;
			color_G = 255;
			WndProc(hDlg, wParam, InvalidateRect(NULL, NULL, TRUE), lParam);
			InvalidateRect(hDlg, NULL, TRUE);
			break;
		}
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			
			count++;
			if (count % 3 == 0) {
				if (dir == 0)
					dir = 1;
				else if (dir == 1)
					dir = 0;
			}

			if (dir == 0 && xdir == 0) {
				x += 3;
				y -= 5;
				if (x >= 200)
					xdir = 1;
			}
			else if (dir == 1 && xdir == 0) {
				x += 3;
				y += 5;
				if (x >= 200)
					xdir = 1;
			}

			if (dir == 0 && xdir == 1) {
				x -= 3;
				y -= 5;
				if (x <= 0)
					xdir = 0;
			}
			else if (dir == 1 && xdir == 1) {
				x -= 3;
				y += 5;
				if (x <= 0)
					xdir = 0;
			}
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		}
		
		break;
	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);

		hBrush = CreateSolidBrush(RGB(color_R, color_G, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		if (RectangleCheck == 0)
			Ellipse(hdc, x, y, x + 50, y + 50);
		else if (RectangleCheck == 1)
			Rectangle(hdc, x, y, x + 50, y + 50);
		ReleaseDC(hDlg, hdc);
		break;
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	HDC hdc;
	HBRUSH hBrush, oldBrush;
	PAINTSTRUCT ps;
	static int x = 800;
	static int y = 300;
	static int count = 0;
	static int dir = 0;
	static int xdir = 0;

		switch (iMessage) //메시지의 번호
		{
		case WM_CREATE:

			break;

		case WM_LBUTTONDOWN:
			DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_3Proc);
			break;

		case WM_KEYDOWN:

			break;

		case WM_KEYUP:
			break;

		case WM_CHAR:

			break;

		case WM_TIMER:
			switch (wParam) {
			case 1:
				count++;
				if (count % 3 == 0) {
					if (dir == 0)
						dir = 1;
					else if (dir == 1)
						dir = 0;
				}

				if (dir == 0 && xdir == 0) {
					x += 3;
					y -= 5;
					if (x >= 1200)
						xdir = 1;
				}
				else if (dir == 1 && xdir == 0) {
					x += 3;
					y += 5;
					if (x >= 1200)
						xdir = 1;
				}

				if (dir == 0 && xdir == 1) {
					x -= 3;
					y -= 5;
					if (x <= 800)
						xdir = 0;
				}
				else if (dir == 1 && xdir == 1) {
					x -= 3;
					y += 5;
					if (x <= 800)
						xdir = 0;
				}
				break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

			if (n == 1)
				SetTimer(hWnd, 1, 100, NULL);
			else if (n == 0)
				KillTimer(hWnd, 1);
			hBrush = CreateSolidBrush(RGB(color_R, color_G, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			if (RectangleCheck == 0)
				Ellipse(hdc, x, y, x + 50, y + 50);
			else if (RectangleCheck == 1)
				Rectangle(hdc, x, y, x + 50, y + 50);
		

			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}




BOOL CollisionCheck(int x, int y, int mx, int my)
{
	if (mx > x&&mx<x + 150 && my>y&&my < y + 150)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
