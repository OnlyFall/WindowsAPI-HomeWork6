#include <Windows.h>
#include "resource.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>
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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 1600, 800, NULL, (HMENU)NULL, hinstance, NULL);
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










static BOOL JUMP = FALSE;
static int moveSkySpeed = 5;
static int moveGroundSpeed = 2;
static int zoom = 0;
static BOOL charter1 = FALSE;
static BOOL CharterText = FALSE;
static BOOL MobText = FALSE;
BOOL CALLBACK Dlg6_3Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hButton;
	HDC hdc;
	static int x = 0, y = 100;
	static int count = 0;
	static int dir = 0;
	static int xdir = 0;
	static BOOL DownCheck = FALSE;
	static HBITMAP hBitmap;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_STOP:
			EndDialog(hDlg, 0);
			break;
			
		case IDC_JUMP:
			JUMP = TRUE;
			break;
			
		case IDC_FLAT:
			if (DownCheck == FALSE) {
				DownCheck = TRUE;
				WndProc(hDlg, WM_KEYDOWN, VK_DOWN, lParam);
			}
			else if (DownCheck == TRUE) {
				DownCheck = FALSE;
				WndProc(hDlg, WM_KEYUP, VK_DOWN, lParam);
			}
			break;

		case IDC_FRONT:
			WndProc(hDlg, WM_KEYUP, VK_RIGHT, lParam);
			WndProc(hDlg, WM_KEYDOWN, VK_LEFT, lParam);
			break;

		case IDC_BACK:
			WndProc(hDlg, WM_KEYDOWN, VK_RIGHT, lParam);

			break;

		case IDC_SPEEDUP:
			moveSkySpeed += 2;
			moveGroundSpeed += 2;
			break;
			
		case IDC_SPEEDDOWN:
			moveSkySpeed -= 2;
			moveGroundSpeed -= 2;
			break;

		case IDC_ZOOM:
			zoom = 200 * 0.2;
			break;

		case IDC_RESIZE:
			zoom = -(200 * 0.2);
			break;

		case IDC_RESET:
			zoom = 0;
			break;

		case IDC_CHARTER1:
			charter1 = FALSE;
			break;

		case IDC_CHARTER2:
			charter1 = TRUE;
			break;

		case IDC_CHECK1:
			if (CharterText == FALSE)
				CharterText = TRUE;
			else
				CharterText = FALSE;
			break;
		case IDC_CHECK2:
			if (MobText == FALSE)
				MobText = TRUE;
			else
				MobText = FALSE;
			break;
		}
		break;

	case WM_TIMER:

		break;
	case WM_PAINT:

		break;
	}
	return 0;
}









LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(������ �ڵ�, ó���ؾ� �� �޽����� ��,�Է���ġ�� ��1,2)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc, memdc2, chardc; //ȭ�� ����� ���� DC�� chardc�� ĳ����dc�Դϴ�.
	 //���δ� ���ļ� ����� �غ��ϴ� DC��
	HBITMAP runBit[6], airPlane[4], cupBanana[4], boom[3];
//	CImage g_cimgTest;
//	HDC adddc = g_cimgTest; 
	HBITMAP backGroundSky, backGroundGround, groundBit, oldgroundBit, skyBit, oldSkyBit, charBit, oldCharBit, addBit;
	//HDC gdc = g_cimgTest.GetDC();
	static HBITMAP hbit, hbit2, oldBit1, oldBit2; 
	static RECT rtChar;                              //���ΰ��� ĳ���� ����
	static RECT rtplane;							//�浹üũ�� ����� �ٳ��� ����
	static RECT rtCup;								//�浹üũ�� �� �ٳ��� ����
	static int x = 1300;
	static int y = 500;
	static int yPos = 0;
	static int jump;
	static int size;
	char text[100] = "������ �ٿ��ּ���ФФФФ�";
	static int mx, my;
	static int planeX, planeY;
	static int cupX, cupY;
	static int count = 0;
	static int mobCount = 0;
	static int down = 0;
	static int upcount = 0;
	static BOOL jumpCheck = FALSE;
	static CString str;
	static BOOL right = FALSE;
	static BOOL crash = FALSE;
	static BOOL boomTrue = FALSE;
	static int boomCount = 0;
	static int boomC = 0;
	static int moveSky = 0, moveGround = 0;
	switch (iMessage) //�޽����� ��ȣ
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 1, NULL);
		SetTimer(hWnd, 2, 10, NULL);
		planeX = -150;
		cupX = -100;
		planeY = rand() % 550 + 150;
		cupY = rand() % 100 + 550;
		rtChar.left = x + 45;
		rtChar.top = y + 35;
		rtChar.right = x + 150;
		rtChar.bottom = y + 140;
	
		rtplane.left = planeX + 10;
		rtplane.top = planeY + 10;
		rtplane.right = planeX + 110;
		rtplane.bottom = planeY + 110;

		rtCup.left = cupX + 10;
		rtCup.top = cupY + 30;
		rtCup.right = cupX + 100;
		rtCup.bottom = cupY + 110;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT)
		{
			x -= 10;
			rtChar.left = x + 45;
			rtChar.top = y + 35;
			rtChar.right = x + 150;
			rtChar.bottom = y + 140;
		}
		else if (wParam == VK_RIGHT)
		{
			x += 10;
			rtChar.left = x + 45;
			rtChar.top = y + 35;
			rtChar.right = x + 150;
			rtChar.bottom = y + 140;
			right = TRUE;
		}
		else if (wParam == VK_DOWN)
		{
			down = 50;
		}
		else if (wParam == VK_UP)
		{
			if (jumpCheck == FALSE) {
				jumpCheck = TRUE;
				SetTimer(hWnd, 3, 10, NULL);
			}
		}
		break;

	case WM_KEYUP:
		if (wParam == VK_RIGHT)
			right = FALSE;
		else if (wParam == VK_DOWN)
			down = 0;
		break;

	case WM_CHAR:
		switch (wParam)
		{
			if (wParam == 'p' || wParam == 'P')
				DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_3Proc);
		}
		break;

	case WM_LBUTTONDOWN:
		DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_3Proc);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1: // �׳� �ִϸ��̼� ������ ����... ������....
			cupX += 20;
			planeX += 7;
			rtplane.left = planeX + 10;
			rtplane.top = planeY + 10;
			rtplane.right = planeX + 120;
			rtplane.bottom = planeY + 110;

			rtCup.left = cupX + 10;
			rtCup.top = cupY + 30;
			rtCup.right = cupX + 100;
			rtCup.bottom = cupY + 110;

			if (planeX >= 1750) {
				planeX = 0;
				planeX = planeX - rand() % 500;
				planeY = rand() % 150 + 150;
				rtplane.left = planeX + 10;
				rtplane.top = planeY + 10;
				rtplane.right = planeX + 120;
				rtplane.bottom = planeY + 110;
			}

			if (cupX >= 1750) {
				cupX = 0;
				cupX = cupX - rand() % 500;
				cupY = rand() % 100 + 550;
				rtCup.left = cupX + 10;
				rtCup.top = cupY + 30;
				rtCup.right = cupX + 100;
				rtCup.bottom = cupY + 110;
			}
			if (rtCup.left < rtChar.right && rtCup.right > rtChar.left &&
				rtCup.top < rtChar.bottom && rtCup.bottom > rtChar.top)
			{
				// �浹
				crash = TRUE;
				boomTrue = TRUE;
				SetTimer(hWnd, 5, 10, NULL);
				cupX = 0;
				cupX = cupX - rand() % 500;
				cupY = rand() % 100 + 550;
				rtCup.left = cupX + 10;
				rtCup.top = cupY + 30;
				rtCup.right = cupX + 100;
				rtCup.bottom = cupY + 110;
				SetTimer(hWnd, 4, 100, NULL);
			}
			else
			{
				crash = FALSE;
			}

			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 2: // �ڵ� �̵����� �ʵ�
			moveSky += moveSkySpeed;
			moveGround += moveGroundSpeed;
			if (moveSky >= 1600)
				moveSky = 0;
			if (moveGround >= 1600)
				moveGround = 0;
			break;

		case 3:
			if (upcount < 20) {
				yPos -= 10;
				rtChar.left = x + 35;
				rtChar.top = y + 35 - yPos;
				rtChar.right = x + 150;
				rtChar.bottom = y + 150 - yPos;
				upcount++;
			}
			else {
				yPos += 10;
				rtChar.left = x + 45;
				rtChar.top = y + 35 + yPos;
				rtChar.right = x + 150;
				rtChar.bottom = y + 150 + yPos;
				upcount++;
			}
			if (upcount == 40) {
				upcount = 0;
				jumpCheck = FALSE;
				JUMP = FALSE;
				KillTimer(hWnd, 3);

			}
			break;

		case 5:
			if (boomC > 6) {
				boomTrue = FALSE;
				boomC = 0;
				KillTimer(hWnd, 5);
			}
			else
				boomC++;

		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		memdc2 = CreateCompatibleDC(memdc);
	//	gdc = CreateCompatibleDC(memdc);

		if (JUMP == TRUE) {
			if (jumpCheck == FALSE) {
				jumpCheck = TRUE;
				SetTimer(hWnd, 3, 10, NULL);
			}
		}

		hbit2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(BackGroundSky));
		hbit = CreateCompatibleBitmap(hdc, 1600, 800);
		count++;
		mobCount++;
		boomCount++;
		boomCount = boomCount % 3;
		mobCount = mobCount % 4;
		count = count % 6;
		//str.Format("D:\5-5~1");

		//g_cimgTest.Load(_T("boom.gif"));
		if (right == FALSE && charter1 == FALSE) {
			runBit[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally1));
			runBit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally2));
			runBit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally3));
			runBit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally4));
			runBit[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally5));
			runBit[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Nomally6));
		}
		else if (right == TRUE && charter1 == FALSE) {
			runBit[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar1));
			runBit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar2));
			runBit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar3));
			runBit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar4));
			runBit[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar5));
			runBit[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(ChangeChar6));
		}

		else if(charter1 == TRUE) {
			runBit[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana1));
			runBit[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana2));
			runBit[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana3));
			runBit[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana4));
		}

		airPlane[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana1));
		airPlane[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana2));
		airPlane[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana3));
		airPlane[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(AirPlaneBanana4));

		cupBanana[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(CupBanana1));
		cupBanana[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(CupBanana2));
		cupBanana[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(CupBanana3));
		cupBanana[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(CupBanana4));

		if (boomTrue == TRUE) {
			boom[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Crash1));
			boom[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Crash2));
			boom[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(Crash3));
		}
		oldBit1 = (HBITMAP)SelectObject(memdc, hbit);
		oldBit2 = (HBITMAP)SelectObject(memdc2,NULL);
		skyBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(BackGroundSky));
		oldSkyBit = (HBITMAP)SelectObject(memdc2, skyBit);
		StretchBlt(memdc, 0 + moveSky, 0, 1600, 800, memdc2, 0, 0, 2000,1000, SRCCOPY);
		StretchBlt(memdc, -1600 + moveSky, 0, 1600, 800, memdc2, 0, 0, 2000,1000,SRCCOPY);
		SelectObject(memdc2, skyBit);
		groundBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(BackGroundGround));
		oldgroundBit = (HBITMAP)SelectObject(memdc2, groundBit);
		TransparentBlt(memdc, 0 + moveGround, 0, 1600, 800, memdc2, 0, 0, 2000, 1000, RGB(0, 0, 0)); //ũ��Ȯ��� ù�������� ������.
		TransparentBlt(memdc, -1600 + moveGround, 0, 1600, 800, memdc2, 0, 0, 2000, 1000, RGB(0, 0, 0));
		SelectObject(memdc2, oldgroundBit);
		
		if (charter1 == FALSE) {
			(HBITMAP)SelectObject(memdc2, runBit[count]);
			TransparentBlt(memdc, x, y + yPos + down, 200 + zoom, (200 - down) + zoom, memdc2, 0, 0, 220, 220, RGB(0, 255, 0)); //ũ��Ȯ��� ù�������� ������.
			SelectObject(memdc2, NULL);// ĳ����
		}
		else if (charter1 == TRUE) {
			(HBITMAP)SelectObject(memdc2, runBit[mobCount]);
			TransparentBlt(memdc, x, y + yPos + down, 200 + zoom, (200 - down) + zoom, memdc2, 0, 0, 150, 150, RGB(0, 0, 255)); //ũ��Ȯ��� ù�������� ������.
			SelectObject(memdc2, NULL);// ĳ����
		}

		if (CharterText == TRUE) {
			SetBkColor(memdc, RGB(255, 255, 0));
			TextOut(memdc, x, y - 30 + yPos + down, text, strlen(text));
		}

		
		
		(HBITMAP)SelectObject(memdc2, airPlane[mobCount]);  // �����ź �ٳ���
		TransparentBlt(memdc, planeX, planeY , 150, 150, memdc2, 0, 0, 150, 150, RGB(0, 0, 255)); //ũ��Ȯ��� ù�������� ������.
		SelectObject(memdc2, NULL);// ĳ����

		if (MobText == TRUE) {
			(HBITMAP)SelectObject(memdc2, airPlane[mobCount]);  // �����ź �ٳ���
			TransparentBlt(memdc, planeX - 200, planeY, 150, 150, memdc2, 0, 0, 150, 150, RGB(0, 0, 255)); //ũ��Ȯ��� ù�������� ������.
			SelectObject(memdc2, NULL);// ĳ����
		}
		
		(HBITMAP)SelectObject(memdc2, cupBanana[mobCount]); // ���ư��� ���ɟ� �ٳ���
		TransparentBlt(memdc, cupX, cupY, 150, 150 , memdc2, 0, 0, 150, 150, RGB(0, 0, 255)); //ũ��Ȯ��� ù�������� ������.
		SelectObject(memdc2, NULL);// ĳ����

		if (MobText == TRUE) {
			(HBITMAP)SelectObject(memdc2, cupBanana[mobCount]); // ���ư��� ���ɟ� �ٳ���
			TransparentBlt(memdc, cupX - 200, cupY, 150, 150, memdc2, 0, 0, 150, 150, RGB(0, 0, 255)); //ũ��Ȯ��� ù�������� ������.
			SelectObject(memdc2, NULL);// ĳ����
		}
		if (boomTrue == TRUE) {
			(HBITMAP)SelectObject(memdc2, boom[boomCount]);  // �����ź �ٳ���
			TransparentBlt(memdc, x, y, 150, 150, memdc2, 0, 0, 100, 100, RGB(0, 255, 0)); //ũ��Ȯ��� ù�������� ������.
			SelectObject(memdc2, NULL);// ĳ����
		}
	//	g_cimgTest.Load(str);
	//	g_cimgTest.Draw(adddc, 0, 0, 165, 217);
	//	g_cimgTest.TransparentBlt(memdc, x, y, 150, 150, memdc2, 0, 0, 168, 217,(RGB(0,0,0)));
		BitBlt(hdc, 0, 0, 1600, 800, memdc, 0, 0, SRCCOPY);



		SelectObject(memdc, oldBit1);

		for (int i = 0; i < 3; ++i)
			DeleteObject(boom[i]);

		for (int i = 0; i < 6; ++i)
		{
			DeleteObject(runBit[i]);
		}
		for (int k = 0; k < 4; ++k)
		{
			DeleteObject(cupBanana[k]);
			DeleteObject(airPlane[k]);
		}
		DeleteObject(skyBit);
		DeleteObject(groundBit);
		DeleteObject(hbit);
		DeleteDC(memdc);
		DeleteDC(memdc2);
		DeleteObject(hbit2);

		

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
