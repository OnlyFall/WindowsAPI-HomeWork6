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

static char word[100];

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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 10, 10, 1100, 930, NULL, (HMENU)NULL, hinstance, NULL);
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

struct Map {
	int xpos;
	int ypos;
	int index = 0;
	int object = 0;
	BOOL check = FALSE;
};

static int size;
static int tile;
static int selectIndex;
static int BackGround = 0;
static int Object = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	HBITMAP block[6], back[2], object[2];
	static Map map[100][100];
	PAINTSTRUCT ps;
	HDC hdc, memdc;
	HWND hDlg = NULL;
	static int wnd_x, wnd_y;
	static int inputX, inputY;

	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		hDlg = CreateDialog(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg_1Proc);
		ShowWindow(hDlg, SW_SHOW);


		break;

	case WM_LBUTTONDOWN:
		inputX = LOWORD(lParam);
		inputY = HIWORD(lParam);
		
		for (int i = 0; i < tile; ++i) {
			for (int j = 0; j < tile; ++j) {
				if (inputX > j * (size / tile) && inputX < (j + 1) * (size / tile) && inputY > i * (size / tile) && inputY < (i + 1) * (size / tile)) {
					map[j][i].index = selectIndex;
					if(map[j][i].object != 1)
						map[j][i].object = Object;
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		Rectangle(hdc, 0, 0, size, size);
		back[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP7));
		back[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP8));
		if (BackGround != 0) {
			(HBITMAP)SelectObject(memdc, back[BackGround - 1]);
			if(BackGround == 1)
				StretchBlt(hdc, 0,0,size, size,memdc,0,0,246,246,SRCCOPY);
			else
				StretchBlt(hdc, 0, 0, size, size, memdc, 0, 0, 259, 194, SRCCOPY);

		}

		block[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(103));
		block[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(104));
		block[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(105));
		block[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(106));
		block[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(107));
		block[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(108));

		object[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP9));
		object[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP10));

	
		for (int i = 0; i < tile; i++) {
			MoveToEx(hdc, (size / tile) * (i + 1), 0, NULL);
			LineTo(hdc, (size / tile) * (i + 1), size);
		}
		for (int i = 0; i < tile; i++) {
			MoveToEx(hdc, 0, (size / tile) * (i + 1), NULL);
			LineTo(hdc, size, (size / tile) * (i + 1));
		}

		for (int i = 0; i < tile; ++i) {
			for (int j = 0; j < tile; ++j)
			{
				if (map[j][i].index != 0) {
					(HBITMAP)SelectObject(memdc, block[map[j][i].index - 1]);
					StretchBlt(hdc, j * (size / tile), i * (size / tile), size / tile, size / tile, memdc, 0, 0, 50, 50, SRCCOPY);
				}
			}
		}

		for (int i = 0; i < tile; ++i) {
			for (int j = 0; j < tile; ++j)
			{
				if (map[j][i].object != 0) {
					(HBITMAP)SelectObject(memdc, object[map[j][i].object - 1]);
					if(map[j][i].object == 1)
						TransparentBlt(hdc, j * (size / tile), i * (size / tile), size / tile, size / tile, memdc, 0, 0, 200, 200, RGB(0, 255, 0));
					else
						TransparentBlt(hdc, j * (size / tile), i * (size / tile), size / tile, size / tile, memdc, 0, 0, 150, 150, RGB(0, 0, 255));
				}
			}
		}
		DeleteDC(memdc);
		for(int i = 0; i < 6; ++i)
			DeleteObject(block[i]);
		for (int i = 0; i < 2; ++i) {
			DeleteObject(object[i]);
			DeleteObject(object[i]);
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}

BOOL CALLBACK Dlg_1Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HBITMAP hBit;
	HWND hButton;
	static HWND hCombo;
	static int cnt = 0;
	static char Sap[][10] = { "검은배경", "푸른배경" };



	
	switch (iMessage)
	{
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO1);
		for (int i = 0; i < 2; ++i) {
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)Sap[i]);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CREAT:  
			//GetDlgItemText(hDlg, IDC_EDIT1, word, 100);    // IDC_EDIT_SOURCE라는 id를 가진 콘트롤에 저장된 문자열을 word에 100만큼 읽어온다. 
			//SetDlgItemText (hDlg, IDC_EDIT1, word);    // IDC_EDIT_COPY라는 id를 가진 컨트롤에 word에 저장된 문자열을 출력한다.
			//Garo = word[0] - '0';
			//Sero = word[1] - '0';
			size = GetDlgItemInt(hDlg, IDC_SIZE_EDIT, NULL, TRUE);
			tile = GetDlgItemInt(hDlg, IDC_TILE, NULL, TRUE);
			if (tile > 20 || size > 1001) {
				//타일의 갯수가 지나치게 많거나 사이즈를 크게 주면 입력이 안되게 설정한다.
			}
			else
				WndProc(hDlg, wParam, InvalidateRect(NULL, NULL, TRUE), lParam);
			break;       

		case IDC_COMBO1:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				BackGround = SendMessage(hCombo, CB_GETCURSEL, 0, 0) + 1;
				WndProc(NULL, NULL, InvalidateRect(NULL, NULL, TRUE), NULL);
			}
			break;
		case IDC_CLOSE:
			PostQuitMessage(0);
			break;

		case IDC_MAP1:
			selectIndex = 1;
			break;

		case IDC_MAP2:
			selectIndex = 2;
			break;
			
		case IDC_MAP3:
			selectIndex = 3;
			break;

		case IDC_MAP4:
			selectIndex = 4;
			break;

		case IDC_MAP5:
			selectIndex = 5;
			break;
			
		case IDC_MAP6:
			selectIndex = 6;
			break;


		case IDC_WINDO:
			if (Object == 0 || Object == 2)
				Object = 1;
			else
				Object = 0;
			break;

		case IDC_FLYAIR:
			if (Object == 0 || Object == 1)
				Object = 2;
			else
				Object = 0;
			break;
		}

		break;

	case WM_PAINT:
		hBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1)); //비트맵 로드
		hButton = GetDlgItem(hDlg, IDC_MAP1); //버튼의 핸들 얻기
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
		hBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP2)); //비트맵 로드
		hButton = GetDlgItem(hDlg, IDC_MAP2); //버튼의 핸들 얻기
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
		hBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP3)); //비트맵 로드
		hButton = GetDlgItem(hDlg, IDC_MAP3); //버튼의 핸들 얻기
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
		hBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP4)); //비트맵 로드
		hButton = GetDlgItem(hDlg, IDC_MAP4); //버튼의 핸들 얻기
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
		hBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP5)); //비트맵 로드
		hButton = GetDlgItem(hDlg, IDC_MAP5); //버튼의 핸들 얻기
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
		hBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP6)); //비트맵 로드
		hButton = GetDlgItem(hDlg, IDC_MAP6); //버튼의 핸들 얻기
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);


		hBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP9)); //비트맵 로드
		hButton = GetDlgItem(hDlg, IDC_WINDO); //버튼의 핸들 얻기
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);

		hBit = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP10)); //비트맵 로드
		hButton = GetDlgItem(hDlg, IDC_FLYAIR); //버튼의 핸들 얻기
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);

		break;
	}

	return 0;
}


