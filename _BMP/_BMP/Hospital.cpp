#include "Hospital.h"

extern HINSTANCE g_hInst;
extern TCHAR *MedicalDepartment[5];
HWND hTab1, hList1, hList2,hList3;
HWND hS1, hS2;
HWND fedit;
HWND fbu,wbu;

LRESULT CALLBACK WndHospitalProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	TCITEM utie;
	LVCOLUMN COL,FOL;
	LVITEM LI;

	switch (iMessage) {
	case WM_CREATE:
		//	MessageBox(hWnd, TEXT("병원꺼"), NULL, MB_OK);
		InitCommonControls();
		hTab1 = CreateWindow(WC_TABCONTROL, TEXT(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 300, 30, 1120, 680, hWnd, (HMENU)10, g_hInst, NULL);
		hList1 = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 310, 80, 300, 600, hWnd, NULL, g_hInst, NULL);
		hList2 = CreateWindow(WC_LISTBOX, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 700, 80, 400, 600, hWnd, NULL, g_hInst, NULL);
		hList3 = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 50, 150, 200, 300, hWnd, NULL, g_hInst, NULL);
	//	hS1 = CreateWindow(TEXT("static"), TEXT("진단 추가"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)11, g_hInst, NULL);
		//	hStatic2 = CreateWindow(TEXT("static"), TEXT("123"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)12, g_hInst, NULL);
		fedit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 80, 98, 100, 25, hWnd, (HMENU)12, g_hInst, NULL);
		fbu = CreateWindow(TEXT("button"), TEXT("검색"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 190, 98, 75, 25, hWnd, (HMENU)13, g_hInst, NULL);
		wbu = CreateWindow(TEXT("button"), TEXT("진단기록추가"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1200, 98, 100, 25, hWnd, (HMENU)14, g_hInst, NULL);

		utie.mask = TCIF_TEXT;
		for (int i = 0; i < 5; i++) {
			utie.pszText = MedicalDepartment[i];
			TabCtrl_InsertItem(hTab1, i, &utie);
		}
		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 100;
		COL.pszText = TEXT("날짜");
		COL.iSubItem = 0;
		SendMessage(hList1, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = TEXT("진단이력");
		COL.iSubItem = 1;
		SendMessage(hList1, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = TEXT("처방이력");
		COL.iSubItem = 2;
		SendMessage(hList1, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		FOL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		FOL.fmt = LVCFMT_LEFT;
		FOL.cx = 100;
		FOL.pszText = TEXT("이름");
		FOL.iSubItem = 0;
		SendMessage(hList3, LVM_INSERTCOLUMN, 0, (LPARAM)&FOL);

		FOL.pszText = TEXT("생년월일");
		FOL.iSubItem = 1;
		SendMessage(hList3, LVM_INSERTCOLUMN, 1, (LPARAM)&FOL);
		return 0;
	case WM_SIZE:
	//	MoveWindow(hS1, LOWORD(lParam) / 2 + 430, HIWORD(lParam) / 2 - 260, 100, 25, TRUE);
		//MoveWindow(hStatic2, LOWORD(lParam) / 2 + 430, HIWORD(lParam) / 2 - 200, 100, 25, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 30, 100, TEXT("이름"), 4);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}