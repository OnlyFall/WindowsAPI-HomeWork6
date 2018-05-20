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

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 10, 10, 1600, 800, NULL, (HMENU)NULL, hinstance, NULL);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(������ �ڵ�, ó���ؾ� �� �޽����� ��,�Է���ġ�� ��1,2)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int wnd_x, wnd_y;

	switch (iMessage) //�޽����� ��ȣ
	{
	case WM_CREATE:
		DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg_1Proc);


		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
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
	//����Ʈ �ڼ� ���� �Լ�
	static Member mem[10]; //�ִ� 10���� �����
	static char member[10];
	static char year[][10] = {"1900","1910","1920","1930","1940","1950","2000","2010" ,"2015","2017" };
	static int member_count;
	int save_year;
	static char x[2];

	static int Check[2],Radio;
	char gender[][30] = { "����","����" };
	char output[10][200];
	//����Ʈ �ٳ����� ���� ����
	switch (iMessage)
	{
	case WM_INITDIALOG:
		//�������� create�� ���� ����
		hList = GetDlgItem(hDlg, IDC_LIST);
		hCombo = GetDlgItem(hDlg, IDC_YEAR);
		//�ϴ� �����ϱ� ���� �������ش�.
		member_count = 0;
		Radio = 0; // �ʱⰪ�� ����
		CheckRadioButton(hDlg, IDC_WOMAN, IDC_MAN, IDC_WOMAN);
		
		//���� ���
		for (int i = 0; i < 10; i++) {
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)year[i]);
		}

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		//����Ʈ �ڽ� ���� �Լ�
		//-ȸ�� �̸�
		case IDC_MEMBER:
			//x[0] = wParam;
			//SetDlgItemText(hDlg, IDC_MEMBER, x);
			GetDlgItemText(hDlg, IDC_MEMBER, mem[member_count].name, 100);
			break;
			//��ȭ ��ȣ
		case IDC_TEL:
			GetDlgItemText(hDlg, IDC_TEL, mem[member_count].phone, 100);
			break;
		//���� ��ư
		case IDC_WOMAN:
			Radio = 0;
			break;
		case IDC_MAN:
			Radio = 1;
			break;
		//����Ʈ �ڽ�
		case IDC_LIST:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = SendMessage(hList, LB_GETCURSEL,0,0);
			break;
		//�޺��ڽ�
		case IDC_YEAR:
			if (HIWORD(wParam) == CBN_SELCHANGE)
				year_sel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);

			//��Ʈ����....�׳� ���ҽ� ������ ���� ���� �ǵ鿴���ϴ� �Ф�
			wsprintf(mem[member_count].birth_day, "%s", year[year_sel]);
			
			break;
		//�׳� ��ư
		//-ȸ������
		case IDC_IN:
			
			//�̸��̶� ��ȭ��ȣ ����
			wsprintf(output[member_count], "�̸�: %s . ��ȭ��ȣ: %s , ����:%s ����: %s",
				mem[member_count].name, mem[member_count].phone, gender[Radio],mem[member_count].birth_day
				);			
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)output[member_count]);
			member_count++;
			break;
		//-��ȸ�� ��ư
		case IDC_NEW_MEMBER:
			SetDlgItemText(hDlg, IDC_MEMBER, "");
			SetDlgItemText(hDlg, IDC_TEL, "");
			Radio = 0;
			year_sel = 0;
			break;

		//-�ٲٱ�
		case IDC_CHANGE:
			//�ٲٱ⸦ �ϱ� ���ؼ��� �켱 �ش� ��ġ ���� �����ϰ�
			//�ٽ� ����ִ´�.
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			//�̸��̶� ��ȭ��ȣ ����
			wsprintf(output[member_count], "�̸�: %s . ��ȭ��ȣ: %s , ����:%s ����: %s",
				mem[member_count].name, mem[member_count].phone, gender[Radio], mem[member_count].birth_day
			);
		
			SendMessage(hList, LB_ADDSTRING, selection, (LPARAM)output[member_count]);
			member_count++;

			break;

		//-Ż��
		case IDC_OUT:
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			break;
		//--------�ý��� â
		case IDCANCEL:
			PostQuitMessage(0);
			break;
		}
		break;
	}

	return 0;
}


