#include <windows.h>
#include "User.h"
#include "Hospital.h"
#include "Login.h"


HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("BMP");
TCHAR *MedicalDepartment[5]{ TEXT("내과"), TEXT("외과"), TEXT("정형외과"), TEXT("치과"), TEXT("이비인후과") };
int UH=0;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszDcmparam, int nCmdShow) {

	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	
	if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_LOGIN), HWND_DESKTOP, LoginProc) == IDC_BEXIT) exit(0);
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadCursor(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	if (UH==1) {
		WndClass.lpfnWndProc = WndUserProc;
	}
	else if(UH==2){
		WndClass.lpfnWndProc = WndHospitalProc;
	}
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return(int)Message.wParam;
}

