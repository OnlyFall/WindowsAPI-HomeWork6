//델몬트 바나나는 세계 제일

#include <Windows.h>
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <ctype.h>
#include <iostream>

using namespace std;
char *X;

char Oper(const char *o);
double Term();
double MultiplyDivide();
double PlusMinus();

void skipSpace() {
	while (isspace(*X)) X++;
}

char Oper(const char *op) {
	skipSpace();
	if (*X && strchr(op, *X))
		return *X++;
	return 0;
}

double Term() {
	double n = 0;
	skipSpace();
	if (isdigit(*X) || *X == '.')
	{
		char N[32];
		int dot;
		char *x = N;
		do {
			dot = (*X == '.');
			*x++ = *X++;
		} while (isdigit(*X) || (!dot && *X == '.')); *x = '\0'; n = atof(N);
	}
	else if (*X == '-' || *X == '+') {
		int minus = (*X == '-'); X++; n = Term();
		if (minus)
			n = -n;
	}
	return n;
}

double MultiplyDivide() {
	double v1, v2;
	char op;
	v1 = Term();
	while (op = Oper("*/%()"))
	{
		v2 = Term();
		switch (op) {
		case '*':
			v1 = v1 * v2;
			break;

		case '/':
			if (v2 == 0.)
			{
				printf("0 으로 나눌 수 없습니다.\n");
				exit(-1);
			}
			v1 = v1 / v2;
			break;

		case '%':
			if (v2 == 0.)
			{
				printf("0 으로 나눌 수 없습니다.\n");
				exit(-1);
			}
			v1 = fmod(v1, v2);
			break;
		case '(':
			if (op + 1 == '+')
				v1 = v1 + v2;
			else if (op + 1 == '-')
				v1 = v1 - v2;
			else if (op + 1 == '*')
				v1 = v1 * v2;
			else if (op + 1 == '/')
				v1 = v1 / v2;
		}
	}
	return v1;
}

double PlusMinus() {
	double v1, v2;
	char op; v1 = MultiplyDivide();
	while (op = Oper("+-")) {
		v2 = MultiplyDivide();
		switch (op) {
		case '+': v1 = v1 + v2;
			break;
		case '-':
			v1 = v1 - v2;
			break;
		}
	}
	return v1;
}

double CalcX(char *x) {
	X = x;
	return PlusMinus();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg6_1Proc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;

static int isStartMove = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hbrBackground = CreateSolidBrush(RGB(51, 181, 229));
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name", "Window Title Name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	//ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT CALLBACK KeyHookProc(int code, WPARAM wParam, LPARAM lParam);
HHOOK hhk;
static char curculate[256] = " ";
static int keyDowbcount = 0;

// ㅇㅅㅇ
BOOL CALLBACK Dlg6_1Proc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInstance;
	static HWND hCombo = (HWND)IDC_EDIT1;
	static float x, y;
	static int plusCount = 0;
	static char m, n;
	static float r;
	static double re;
	static int res;
	
	static char reverse[100];
	static char result[100];
	static char Change[100];
	static int changeCount = 0;
	static BOOL reverseCheck = FALSE;
	static char temP[100];
	static int temPcount = 0;
	static int count = 0;
	static int ree = 0;
	static int total = 0;
	static int bCount = 0;
	static BOOL CHANGE = FALSE;
	static int zinCount = 0;
	static int temp = 0;
	static int binary;
	static BOOL Result = FALSE;
	switch (iMsg) {
	case WM_CREATE:
		hInstance = hInst;
		break;

	case WM_INITDIALOG:
		hhk = SetWindowsHookEx(WH_KEYBOARD, KeyHookProc, NULL, GetCurrentThreadId());
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CLOSE:
			PostQuitMessage(0);
			break;

		case IDC_PLUS:
			if (curculate[count - 1] != '-' && curculate[count - 1] != '+' && curculate[count - 1] != '/' && curculate[count - 1] != 'x')
				curculate[count++] = '+';
			break;
		case IDC_CE:
			if (count > 0) {
				curculate[count - 1] = ' ';
				count--;
			}
			break;

		case IDC_CHANGE:
			for (int i = 0; i <= count; ++i) {
				if (zinCount == 0)
					zinCount = 1;
				else
					zinCount *= 10;
				
				if (curculate[i] == '+' || curculate[i] == '-' || curculate[i] == '*' || curculate[i] == '/' || curculate[i] == '\0')
				{
					zinCount /= 10;
					for (int j = i - bCount; j < i; ++j)
					{
						temp += (curculate[j] - '0') * zinCount;
						zinCount /= 10;
					}
					while (temp >= 2) {
						binary = temp % 2;
						if (binary == 0)
							temP[temPcount++] = '0';
						else if(binary == 1)
							temP[temPcount++] = '1';
						temp = temp / 2;
						if (temp == 1)	
							temP[temPcount] = '1';
					}
	//우빈이 왔쪄염 뿌우우우			
					for (int k = 0; k <= temPcount; ++k)
						Change[total + k] = temP[temPcount - k];
					total += temPcount + 1;
					Change[total++] = curculate[i];
					temp = 0;
					temPcount = 0;
					zinCount = 0;
					bCount = 0;
				}
				else
					bCount++;
			}
			
			if (CHANGE == FALSE)
				CHANGE = TRUE;
			else
				CHANGE = FALSE;
			break;

		case IDC_MULTI10:
			
			for (int i = 0; i <= count; ++i) {
				if (curculate[i] == '+' || curculate[i] == '-' || curculate[i] == '*' || curculate[i] == '/' || curculate[i] == ' ')
				{
					count++;
					for (int j = count; j > i; --j)
						curculate[j] = curculate[j - 1];
					curculate[i] = '0';
					i++;
				}
			}
			curculate[count++] = '0';
			break;

		case IDC_DELETEREAR:
			for (int i = 0; i < count; ++i) {
				if (curculate[i] == '+' || curculate[i] == '-' || curculate[i] == '*' || curculate[i] == '/' || curculate[i] == ' ')
				{
					for (int j = i - 1; j < count; ++j)
						curculate[j] = curculate[j + 1];
					count--;

					i++;
				}
			}
			curculate[count - 1] = ' ';
			count--;
			break;

		case IDC_CLEAR:
			for (int i = 0; i < count; ++i) {
				curculate[i] = ' ';
			}
			Result = FALSE;
			count = 0;
			break;

		case IDC_REVERSE:
			for (int i = 0; i < count; ++i) {
				reverse[i] = curculate[count - i - 1];
			}
			if (reverseCheck == FALSE)
				reverseCheck = TRUE;
			else
				reverseCheck = FALSE;
			break;

		case IDC_MINUS:
			if (curculate[count - 1] != '-' && curculate[count - 1] != '+' && curculate[count - 1] != '/' && curculate[count - 1] != 'x')
				curculate[count++] = '-';
			break;

		case IDC_RESULT:
			//infix_to_postfix(curculate, 0);
			////int x;
			////x = calc_postfix(curculate);
			curculate[count] = '\0';
		
			int x;
			x = CalcX(curculate);
			wsprintf(result, TEXT("%d"), x);
			Result = TRUE;
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case IDC_GOBHAGI:
			if (curculate[count - 1] != '-' && curculate[count - 1] != '+' && curculate[count - 1] != '/' && curculate[count - 1] != 'x')
				curculate[count++] = '*';
			break;

		case IDC_NANUGI:
			if (curculate[count - 1] != '-' && curculate[count - 1] != '+' && curculate[count - 1] != '/' && curculate[count - 1] != 'x')
				curculate[count++] = '/';
			break;

		case IDC_NUMBER0:
			curculate[count++] = '0';
			//SetDlgItemText(hwnd, IDC_EDIT1, curculate - 48);
			break;

		case IDC_NUMBER1:
			curculate[count++] = '1';
			//SetDlgItemInt(hwnd, IDC_EDIT1, curculate[count - 1] - 48, TRUE);
			break;

		case IDC_NUMBER2:
			curculate[count++] = '2';
			//SetDlgItemInt(hwnd, IDC_EDIT1, curculate[count - 1] - 48, TRUE);
			break;

		case IDC_NUMBER3:
			curculate[count++] = '3';
			//SetDlgItemInt(hwnd, IDC_EDIT1, curculate[count - 1] - 48, TRUE);
			break;

		case IDC_NUMBER4:
			curculate[count++] = '4';
			//SetDlgItemInt(hwnd, IDC_EDIT1, curculate[count - 1] - 48, TRUE);
			break;

		case IDC_NUMBER5:
			curculate[count++] = '5';
			//SetDlgItemInt(hwnd, IDC_EDIT1, curculate[count - 1] - 48, TRUE);
			break;

		case IDC_NUMBER6:
			curculate[count++] = '6';
			//SetDlgItemInt(hwnd, IDC_EDIT1, curculate[count - 1] - 48, TRUE);
			break;

		case IDC_NUMBER7:
			curculate[count++] = '7';
			//SetDlgItemInt(hwnd, IDC_EDIT1, curculate[count - 1] - 48, TRUE);
			break;

		case IDC_NUMBER8:
			curculate[count++] = '8';
			//SetDlgItemInt(hwnd, IDC_EDIT1, curculate[count - 1] - 48, TRUE);
			break;

		case IDC_NUMBER9:
			curculate[count++] = '9';
			//SetDlgItemInt(hwnd, IDC_EDIT1, curculate[count - 1] - 48, TRUE);
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_KEYUP:
		if (wParam == '1')
			curculate[count++] = '1';
		else if (wParam == '2')
			curculate[count++] = '2';
		else if (wParam == '3')
			curculate[count++] = '3';
		else if (wParam == '4')
			curculate[count++] = '4';
		else if (wParam == '5')
			curculate[count++] = '5';
		else if (wParam == '6')
			curculate[count++] = '6';
		else if (wParam == '7')
			curculate[count++] = '7';
		else if (wParam == '8')
			curculate[count++] = '8';
		else if (wParam == '9')
			curculate[count++] = '9';
		else if (wParam == '0')
			curculate[count++] = '0';
		else if (wParam == '+')
			curculate[count++] = '+';
		else if (wParam == '-')
			curculate[count++] = '-';
		else if (wParam == '*')
			curculate[count++] = '*';
		else if (wParam == '/')
			curculate[count++] = '/';

		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_PAINT:
		if(reverseCheck == FALSE && Result == FALSE && CHANGE == FALSE)
			SetDlgItemText(hwnd, IDC_EDIT1, curculate);
		else if(reverseCheck == TRUE && Result == FALSE && CHANGE == FALSE)
			SetDlgItemText(hwnd, IDC_EDIT1, reverse);
		else if (CHANGE == TRUE, Result == FALSE)
			SetDlgItemText(hwnd, IDC_EDIT1, Change);
		else if (Result == TRUE) {
			SetDlgItemText(hwnd, IDC_EDIT1, result);
		}

		break;
	}
	return 0;
}

LRESULT CALLBACK KeyHookProc (int code, WPARAM wParam, LPARAM lParam)
{
	if ((lParam & 0x80000000) == 0) {
		switch (wParam)
		{
		case '1':
			Dlg6_1Proc(NULL, WM_KEYUP, '1', NULL);
			break;

		case '2':
			Dlg6_1Proc(NULL, WM_KEYUP, '2', NULL);
			break;
		case '3':
			Dlg6_1Proc(NULL, WM_KEYUP, '3', NULL);
			break;
		case '4':
			Dlg6_1Proc(NULL, WM_KEYUP, '4', NULL);
			break;
		case '5':
			Dlg6_1Proc(NULL, WM_KEYUP, '5', NULL);
			break;
		case '6':
			Dlg6_1Proc(NULL, WM_KEYUP, '6', NULL);
			break;
		case '7':
			Dlg6_1Proc(NULL, WM_KEYUP, '7', NULL);
			break;
		case '8':
			Dlg6_1Proc(NULL, WM_KEYUP, '8', NULL);
			break;
		case'9':
			Dlg6_1Proc(NULL, WM_KEYUP, '9', NULL);
			break;
		case '0':
			Dlg6_1Proc(NULL, WM_KEYUP, '0', NULL);
			break;
		case '+':
			Dlg6_1Proc(NULL, WM_KEYUP, '+', NULL);
			break;
		case '-':
			Dlg6_1Proc(NULL, WM_KEYUP, '-', NULL);
			break;
		case '*':
			Dlg6_1Proc(NULL, WM_KEYUP, '*', NULL);
			break;
		case '/':
			Dlg6_1Proc(NULL, WM_KEYUP, '/', NULL);
			break;
		}
	}
	return CallNextHookEx(hhk, code, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam)
{
	static HINSTANCE hInstance;
	switch (iMsg)
	{
	case WM_CREATE:
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Dlg6_1Proc);
		//PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:


		break;
	case WM_PAINT:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}



