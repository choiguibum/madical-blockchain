#include "User.h"

extern HINSTANCE g_hInst;
extern TCHAR *MedicalDepartment[5];
HWND uTab1,uList1,uList2;
HWND hStatic1,hStatic2;
HWND De, Ue;

LRESULT CALLBACK WndUserProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	TCITEM utie;
	LVCOLUMN COL;
	LVITEM LI;

	switch (iMessage) {
	case WM_CREATE:
	//	MessageBox(hWnd, TEXT("환자꺼"), NULL, MB_OK);
		InitCommonControls();
		

		uTab1 = CreateWindow(WC_TABCONTROL, TEXT(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 300, 30, 1120, 680, hWnd, (HMENU)0, g_hInst, NULL);
		uList1 = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 310, 80, 300, 600, hWnd, NULL, g_hInst, NULL);
		uList2 = CreateWindow(WC_LISTBOX, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 700, 80, 400, 600, hWnd, NULL, g_hInst, NULL);
		hStatic1 = CreateWindow(TEXT("static"), TEXT("주치의"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)1, g_hInst, NULL);
	//	hStatic2 = CreateWindow(TEXT("static"), TEXT("123"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)2, g_hInst, NULL);
		Ue = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 80, 98, 100, 25, hWnd, (HMENU)100, g_hInst, NULL);
		De = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 1200, 98, 100, 25, hWnd, (HMENU)100, g_hInst, NULL);
		utie.mask = TCIF_TEXT;
		for (int i = 0; i < 5; i++) {
			utie.pszText = MedicalDepartment[i];
			TabCtrl_InsertItem(uTab1, i, &utie);
		}
		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 100;
		COL.pszText = TEXT("날짜");
		COL.iSubItem = 0;
		SendMessage(uList1, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = TEXT("진단이력");
		COL.iSubItem = 1;
		SendMessage(uList1, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = TEXT("처방이력");
		COL.iSubItem = 2;
		SendMessage(uList1, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		SetWindowText(hStatic1, TEXT("주치의"));
	//	SetWindowText(hStatic2, TEXT("123"));


		return 0;
	case WM_SIZE:
		MoveWindow(hStatic1, LOWORD(lParam) / 2+430 , HIWORD(lParam) / 2-260, 100, 25, TRUE);
//		MoveWindow(hStatic2, LOWORD(lParam) / 2 + 430, HIWORD(lParam) / 2 - 200, 100, 25, TRUE);
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