// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <math.h>

#define MAX_LOADSTRING 100
#define TMR_1 1

HINSTANCE hInst;							
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			
INT value;
HWND h_gStatic1;
HWND h_gStatic2;
HWND hwndButton;

int skala = 1;
const int odrzucone = 150;
const int wierszy = 840;
const int wys_wykr_1 = 120;
const int wys_wykr_2 = 400;
const int od_krawedzi = 70;
int predkosc = 4;
int licznik = 0;
bool reset = FALSE;

std::vector <float> tab;
std::vector <float> tab2;

int col = 0;
std::vector<Point> data1;
std::vector<Point> data2;
RECT drawArea1 = { 0, 0, 1500, 2000 };
RECT drawArea2 = { 71 + skala*licznik, 50, 2000, 800};

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);

void tworzenie_wektorow (){

	for (int i = odrzucone; i < wierszy; i++){
		int zmienna1 = (int) 3* tab[i];
		data1.push_back(Point(skala*(i - odrzucone) + od_krawedzi, skala*((-1)*zmienna1)+skala*wys_wykr_1));
		int zmienna2 = (int) tab2[i];
		data2.push_back(Point(skala*(i - odrzucone) + od_krawedzi, skala*((-1)*tab2[i])+skala*wys_wykr_2));
	}
}

void MyOnPaint(HDC hdc)	
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));
	Pen pen2(Color(255, 25*col, 0, 255));
	Font czcionka (L"Times New Roman", 12);
	SolidBrush kolor (Color (250, 0, 0, 0) );
	
	if (licznik < wierszy - odrzucone - 1){
		int i = licznik + 1;
		graphics.DrawLine(&pen2, data1[i - 1].X, data1[i - 1].Y, data1[i].X, data1[i].Y);	//wykres funkcji odchylenia (f1)
		graphics.DrawLine(&pen2, data2[i - 1].X, data2[i - 1].Y, data2[i].X, data2[i].Y);	//wykres funkcji wysokosci (f2)
	}

	graphics.DrawLine(&pen, od_krawedzi, skala * wys_wykr_1, skala * (wierszy - odrzucone) + od_krawedzi, skala * wys_wykr_1);	//os x f1
	graphics.DrawLine(&pen, od_krawedzi, skala * wys_wykr_2, skala * (wierszy - odrzucone) + od_krawedzi, skala * wys_wykr_2);	//os x f2
	graphics.DrawLine(&pen, od_krawedzi, skala * wys_wykr_1 - skala * 70, od_krawedzi, skala * wys_wykr_1 + skala * 60);	//os y f1
	graphics.DrawLine(&pen, od_krawedzi, skala * wys_wykr_2, od_krawedzi, skala * wys_wykr_2-skala * 170);	//os y f2
	graphics.DrawString(L"odchylenie", 10, &czcionka, PointF(0, skala * 20), &kolor);
	graphics.DrawString(L"wysokoœæ", 8, &czcionka, PointF(0, skala * 200), &kolor);
	graphics.DrawString(L"Obecne wychylenie:", 18, &czcionka, PointF(760, 340), &kolor);
	graphics.DrawString(L"[stopni]", 9, &czcionka, PointF(957, 340), &kolor);
	graphics.DrawString(L"Wysokoœæ:", 18, &czcionka, PointF(760, 360), &kolor);
	graphics.DrawString(L"[mm]", 4, &czcionka, PointF(957, 360), &kolor);
	graphics.DrawString(L"0 -", 3, &czcionka, PointF(20, skala * wys_wykr_1 - 12), &kolor);	
	graphics.DrawString(L"0 -", 3, &czcionka, PointF(20, skala * wys_wykr_2 - 12), &kolor);	
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea==NULL){
		InvalidateRect(hWnd, NULL, reset); 
		reset = FALSE;}
	else
		InvalidateRect(hWnd, drawArea, FALSE); 
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc);
	EndPaint(hWnd, &ps);
}

void inputData()
{	
	tworzenie_wektorow ();
}

int OnCreate(HWND window)
{
	inputData();
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::ifstream plik;
	plik.open( "outputPendulum01.log" );
	if( !plik.good() ){
		MessageBox( NULL, TEXT("Nie mo¿na otworzyæ pliku."), TEXT("A to pech!"), MB_ICONEXCLAMATION );
    PostQuitMessage( 0 );
	}

	int wartosc;
	int zmiennai;
	float zmiennaf;
	float x;
	while ( !plik.eof() ){
		plik >> zmiennaf >> x >> x >> x >> x >> x >> x >> x >> x >> x >> x >> x;
		tab.push_back (zmiennaf);
		wartosc = (5*500*(1-cos(zmiennaf*3.14/180.0)));
		tab2.push_back (wartosc);
	}
	plik.close();

	MSG msg;
	HACCEL hAccelTable;

	value = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) 
{
	HWND hWnd;
	HWND hStatic;

	hInst = hInstance; 

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
                                                     
	hwndButton = CreateWindow(TEXT("button"), TEXT("Zwieksz skalê"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		150, 410, 120, 50, hWnd, (HMENU)ID_BUTTON1, hInstance, NULL);                              

	hwndButton = CreateWindow(TEXT("button"), TEXT("Zmniejsz skalê"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		290, 410, 120, 50, hWnd, (HMENU)ID_BUTTON2, hInstance, NULL); 
		
	hwndButton = CreateWindow(TEXT("button"), TEXT("Szybciej"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		430, 410, 120, 50, hWnd, (HMENU)ID_BUTTON3, hInstance, NULL); 

	hwndButton = CreateWindow(TEXT("button"), TEXT("Wolniej"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		570, 410, 120, 50, hWnd, (HMENU)ID_BUTTON4, hInstance, NULL); 

	hwndButton = CreateWindow(TEXT("button"), TEXT("Resetuj ca³oœæ"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		710, 410, 120, 50, hWnd, (HMENU)ID_BUTTON5, hInstance, NULL); 

	h_gStatic1 = CreateWindow(TEXT("static"), TEXT("0"), WS_VISIBLE | WS_CHILD,
		930, 343, 30, 20, hWnd, NULL, hInstance, NULL);

	h_gStatic2 = CreateWindow(TEXT("static"), TEXT("0"), WS_VISIBLE | WS_CHILD,
		930, 363, 30, 20, hWnd, NULL, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Timer ON"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		850, 410, 100, 30, hWnd, (HMENU)ID_BUTTON6, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Timer OFF"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		850, 450, 100, 30, hWnd, (HMENU)ID_BUTTON7, hInstance, NULL);

	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON1 :	// zwieksz skale  
			KillTimer(hWnd, TMR_1);
			skala=2;
			data1.clear(); 
			data2.clear(); 
			tworzenie_wektorow ();
			reset = TRUE;
			licznik = 0;
			repaintWindow(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON2 :	//zmniejsz skale
			KillTimer(hWnd, TMR_1);
			skala=1;
			data1.clear(); 
			data2.clear(); 
			tworzenie_wektorow ();
			reset = TRUE;
			licznik = 0;
			repaintWindow(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON3 :	//zwolnij
			repaintWindow(hWnd, hdc, ps, NULL);
			KillTimer(hWnd, TMR_1);
			predkosc /= 2;
			if (predkosc < 1)
				predkosc = 1;
			SetTimer(hWnd, TMR_1, predkosc, 0);
			break;
		case ID_BUTTON4 :	//przyspiesz
			repaintWindow(hWnd, hdc, ps, NULL);
			KillTimer(hWnd, TMR_1);
			predkosc *= 2;
			if (predkosc > 200)
				predkosc = 200;
			SetTimer(hWnd, TMR_1, predkosc, 0);
			break;
		case ID_BUTTON5 :	//reset
			KillTimer(hWnd, TMR_1);
			reset = TRUE;
			licznik = 0;
			repaintWindow(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON6:	//timer on
			repaintWindow(hWnd, hdc, ps, NULL);
			SetTimer(hWnd, TMR_1, predkosc, 0);
			break;
		case ID_BUTTON7:	//timer off
			repaintWindow(hWnd, hdc, ps, NULL);
			KillTimer(hWnd, TMR_1);
			break;
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		repaintWindow(hWnd, hdc, ps, NULL);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			repaintWindow(hWnd, hdc, ps, &drawArea2);
			value++;
			licznik++;
			if (licznik < wierszy - odrzucone){
				int zmienna = (int) tab[licznik+odrzucone];
				wchar_t buf[500];
				swprintf_s(buf, L"%d", zmienna);
				SetWindowText(h_gStatic1, buf);
				int zmienna2 = (int)tab2[licznik + odrzucone]/5;
				wchar_t buf2[500];
				swprintf_s(buf2, L"%d", zmienna2);
				SetWindowText(h_gStatic2, buf2);
			}
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
