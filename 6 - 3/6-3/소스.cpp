//����Ʈ �ٳ����� ���� ����
//�� move ���ظ԰ھ�� �Ф�

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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 10, 10, 1000, 800, NULL, (HMENU)NULL, hinstance, NULL);
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

//���� �Լ� ����----
static BOOL Start, Shape;
static double spring_x, move_xx , move_yy;
static int Sh_count = 0;
HWND hWnd2;
static int color_R, color_G, color_B; //���� ����
static int zig_count, zigzag = 0;
static BOOL Left , Move , Stop ,Sin, Zigzag, Spring = FALSE; //��ư
static int posX, posY;
static double f;
static double y;
static BOOL moveCheck = FALSE;
//-----------
#define PIE 3.141592

//���� �Լ�
double rad(double degree) {
	return PIE * degree / 180.0;
}//���� ��ȯ �Լ�

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(������ �ڵ�, ó���ؾ� �� �޽����� ��,�Է���ġ�� ��1,2)
{
	hWnd2 = hWnd;
	PAINTSTRUCT ps;
	HDC hdc;
	static int id;
	HWND hButton;
	HPEN hPen, holdPen;
	HBRUSH hBrush, oldBrush;
	switch (iMessage) //�޽����� ��ȣ
	{
	case WM_CREATE:
		posX = 500;
		posY = 400;
		spring_x = 0;
		break;

		//�̵��ϰų� ������ ���°� ������ ���� Ÿ�̸� �Դϴ�.
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
				move_xx += 10;
				if (move_xx > 100) {
					move_xx = 0;
				}//�ʱ�ȭ ���ش�.
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

		//�̵� �� Ÿ�̸� ����
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

		//��� ���� ����------------------
		MoveToEx(hdc, 500, 800, NULL);
		LineTo(hdc, posX, 0);

		MoveToEx(hdc, 1000, 400, NULL);
		LineTo(hdc, 0, 400);
		//----------------------------------
		
		hPen = CreatePen(PS_SOLID, 4 ,RGB(color_R, color_G, color_B));
		holdPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		//���� �����ϱ�
		if (Zigzag) {
			//���� ��� ����
			MoveToEx(hdc, posX - 500 - move_xx, posY - 100 - move_yy, NULL);
			for (int i = 1; i < 100; i++) {
				if (i % 2 == 1) {
					Zigzag = 1; //������� �� �Ʒ� ����
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
			//��ó ���̹� ��α�
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
			//�������� �̻��� �Ф�
			spring_x = 0;
			MoveToEx(hdc, 100 - move_xx, posY, NULL);
			ArcTo(hdc, 200 + spring_x - move_xx, 200, 400 + spring_x - move_xx, 400,
				posX + 400, 400 + spring_x, 400 + spring_x - move_xx, 400 + spring_x);
			for (int i = 0; i < 10; i++) {
				spring_x += 60;
				ArcTo(hdc, 200 + spring_x - move_xx, 200, 400 + spring_x - move_xx, 400, 
					posX + 400 - move_xx, 400 + spring_x, 400 + spring_x - move_xx, 400 + spring_x);
				//Arc �Լ� ���ʴ�� left right top bottom x y start ,, s y end
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
	//case���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}
//�̰͵� ���� ������ â
BOOL CALLBACK Dlg_1Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HWND hButton;
	hWnd2 = hDlg;
	PAINTSTRUCT d_ps;
	HBRUSH hBrush, oldBrush;
	//üũ�ڽ� ��
	static int check_red, check_blue, check_green, check_mix =0;
	
	static int posX, posY, plusX;
	switch (iMessage)
	{
	case WM_INITDIALOG:
		//�������� create�� ���� ����
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

		//üũ ��ư_���� ��ȯ--------
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
			//�ͽ� �϶� ������ :: �ͽ��� ���� ���� ������ ���ƿ���
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

		//��ư------------
		case IDC_RESET:
			//�ʱ�ȭ ���ֱ�
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

		//�ý���-----------------
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;

		//���� ��
		}


		WndProc(hDlg, wParam, InvalidateRect(NULL, NULL, TRUE), lParam);
		
		break;
	


	}
	return 0;
}


