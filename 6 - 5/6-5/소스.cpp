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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 10, 10, 1600, 800, NULL, (HMENU)NULL, hinstance, NULL);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int wnd_x, wnd_y;

	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg_1Proc);


		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}
struct Member
{
	char name[20];
	char gender[10];
	char birth_day[20];
	char phone[20];

};
BOOL CALLBACK Dlg_1Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static HWND hButton,hCombo, hList;
	static int selection, year_sel;
	//에디트 박수 관련 함수
	static Member mem[10]; //최대 10명의 사람들
	static char member[10];
	static char year[][10] = {"1900","1910","1920","1930","1940","1950","2000","2010" ,"2015","2017" };
	static int member_count;
	int save_year;
	static char x[2];

	static int Check[2],Radio;
	char gender[][30] = { "여성","남성" };
	char output[10][200];
	//델몬트 바나나는 세계 제일
	switch (iMessage)
	{
	case WM_INITDIALOG:
		//윈도우의 create랑 같은 원리
		hList = GetDlgItem(hDlg, IDC_LIST);
		hCombo = GetDlgItem(hDlg, IDC_YEAR);
		//일단 시작하기 전에 설정해준다.
		member_count = 0;
		Radio = 0; // 초기값은 여성
		CheckRadioButton(hDlg, IDC_WOMAN, IDC_MAN, IDC_WOMAN);
		
		//연도 출력
		for (int i = 0; i < 10; i++) {
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)year[i]);
		}

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		//에디트 박스 관련 함수
		//-회원 이름
		case IDC_MEMBER:
			//x[0] = wParam;
			//SetDlgItemText(hDlg, IDC_MEMBER, x);
			GetDlgItemText(hDlg, IDC_MEMBER, mem[member_count].name, 100);
			break;
			//전화 번호
		case IDC_TEL:
			GetDlgItemText(hDlg, IDC_TEL, mem[member_count].phone, 100);
			break;
		//라디오 버튼
		case IDC_WOMAN:
			Radio = 0;
			break;
		case IDC_MAN:
			Radio = 1;
			break;
		//리스트 박스
		case IDC_LIST:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = SendMessage(hList, LB_GETCURSEL,0,0);
			break;
		//콤보박스
		case IDC_YEAR:
			if (HIWORD(wParam) == CBN_SELCHANGE)
				year_sel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);

			//스트레스....그냥 리소스 편집기 가서 직접 건들였습니다 ㅠㅠ
			wsprintf(mem[member_count].birth_day, "%s", year[year_sel]);
			
			break;
		//그냥 버튼
		//-회원가입
		case IDC_IN:
			
			//이름이랑 전화번호 결합
			wsprintf(output[member_count], "이름: %s . 전화번호: %s , 성별:%s 연도: %s",
				mem[member_count].name, mem[member_count].phone, gender[Radio],mem[member_count].birth_day
				);			
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)output[member_count]);
			member_count++;
			break;
		//-새회원 버튼
		case IDC_NEW_MEMBER:
			SetDlgItemText(hDlg, IDC_MEMBER, "");
			SetDlgItemText(hDlg, IDC_TEL, "");
			Radio = 0;
			year_sel = 0;
			break;

		//-바꾸기
		case IDC_CHANGE:
			//바꾸기를 하기 위해서는 우선 해당 위치 값을 삭제하고
			//다시 집어넣는다.
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			//이름이랑 전화번호 결합
			wsprintf(output[member_count], "이름: %s . 전화번호: %s , 성별:%s 연도: %s",
				mem[member_count].name, mem[member_count].phone, gender[Radio], mem[member_count].birth_day
			);
		
			SendMessage(hList, LB_ADDSTRING, selection, (LPARAM)output[member_count]);
			member_count++;

			break;

		//-탈퇴
		case IDC_OUT:
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			break;
		//--------시스템 창
		case IDCANCEL:
			PostQuitMessage(0);
			break;
		}
		break;
	}

	return 0;
}


