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


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //h�� �ǹ�? = �ڵ�
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hinstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //���α׷� ������ ���̴� Ŀ��
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ 
	WndClass.hInstance = hinstance; //���� ����ǰ� �ִ� ��ü�� �ڵ�
	WndClass.lpfnWndProc = WndProc; //���ν����Լ��� �̸� 
	WndClass.lpszClassName = lpszClass; //������ Ŭ���� ������ �̸� 
	WndClass.lpszMenuName = NULL; // �޴��� �̸�
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //������ ��� ��Ÿ��
											  // ������� WNDCLASS����ü�� �����鿡 ���� ����

	RegisterClass(&WndClass); // ������ Ŭ������ �ü���� ���

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 1920, 1080, NULL, (HMENU)NULL, hinstance, NULL);
	//CreateWindow(Ŭ���� �̸�,Ÿ��Ʋ �̸�,������ ��Ÿ��, ������ ��ġ��ǥx , y,������ ����ũ��, ������ ����ũ��, �θ� ������ �ڵ�, �޴� �ڵ�, �������α׷� �ν��Ͻ�, ���� ������ ����(NULL))
	ShowWindow(hWnd, nCmdShow); //�������� ȭ�� ���
	UpdateWindow(hWnd); //OS�� WM_PAINT�޽��� ����
	while (GetMessage(&Message, NULL, 0, 0))
	{
		//������ ���ν������� PostQuitMessage()�� ȣ���Ҷ� �����.
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}//�޽��������� ����.
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
		EnableWindow(hButton, FALSE); // ���� ���� ��ư�� ��Ȱ��ȭ
		hButton = GetDlgItem(hDlg, ID_MOTHER_STOP);
		EnableWindow(hButton, FALSE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_START: // ���� ��ư
			hButton = GetDlgItem(hDlg, ID_START);
			EnableWindow(hButton, FALSE); // ���۹�ư ��Ȱ��ȭ
			hButton = GetDlgItem(hDlg, ID_STOP);
			EnableWindow(hButton, TRUE); // ������ư Ȱ��ȭ
			SetTimer(hDlg, 1, 100, NULL);
			hdc = GetDC(hDlg);
			Ellipse(hdc, x, y, x + 10, y + 10);
			ReleaseDC(hDlg, hdc);
			break;
		case ID_STOP: // ���� ��ư
			hButton = GetDlgItem(hDlg, ID_START);
			EnableWindow(hButton, TRUE); // ���۹�ư Ȱ��ȭ
			hButton = GetDlgItem(hDlg, ID_STOP);
			KillTimer(hDlg, 1);
			EnableWindow(hButton, FALSE); // ������ư ��Ȱ��ȭ
			break;
		case ID_CLOSE: // ���� ��ư
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(������ �ڵ�, ó���ؾ� �� �޽����� ��,�Է���ġ�� ��1,2)
{
	HDC hdc;
	HBRUSH hBrush, oldBrush;
	PAINTSTRUCT ps;
	static int x = 800;
	static int y = 300;
	static int count = 0;
	static int dir = 0;
	static int xdir = 0;

		switch (iMessage) //�޽����� ��ȣ
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
	//case���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
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
