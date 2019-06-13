#include <windows.h>
#include "User.h"
#include "Hospital.h"
#include "Login.h"

#define DB_HOST "192.168.100.102" //주로 카페에서 진행해서 ip주소는 고정적이지않음.
#define DB_USER "medi"
#define DB_PASS "1234"
#define DB_NAME "medi"
#define DB_NAME1 "hospital"
#define HAVE_STRUCT_TIMESPEC

MYSQL mysql, mysql2 ;
MYSQL_RES	*res;
MYSQL_ROW	row;

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("BMP");
TCHAR *MedicalDepartment[5]{ TEXT("내과"), TEXT("외과"), TEXT("정형외과"), TEXT("치과"), TEXT("이비인후과") };
int UH=0;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszDcmparam, int nCmdShow) {

	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	TCHAR test[500];
	g_hInst = hInstance;

	mysql_init(&mysql2);
	if (!mysql_real_connect(&mysql2, DB_HOST, DB_USER, DB_PASS, DB_NAME1, 3306, NULL, 0)) {
		exit(0);
	}
	mysql_set_character_set(&mysql2, "euckr");
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0)) {
		exit(0);
	}
	mysql_set_character_set(&mysql, "euckr");

	//여기까지 db연결 /// db연결안되면 실행안됨
	if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_LOGIN), HWND_DESKTOP, LoginProc) == IDC_BEXIT) exit(0);//로그인창 띄우는
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadCursor(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	if (UH==1) {//환자로 로그인시
		WndClass.lpfnWndProc = WndUserProc;
	}
	else if(UH==2){//병원으로 로그인시
		WndClass.lpfnWndProc = WndHospitalProc;
	}
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 30, 10, 1200, 700, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return(int)Message.wParam;
}

