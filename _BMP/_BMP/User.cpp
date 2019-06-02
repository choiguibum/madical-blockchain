#include "User.h"
#include "Login.h"

extern HINSTANCE g_hInst;
extern MYSQL mysql;
extern MYSQL mysql2;
extern MYSQL_RES	*res;
extern MYSQL_ROW	row;
extern TCHAR *MedicalDepartment[5];
extern user myself;
HWND uTab1,uList1,uList2;
HWND hStatic1,hStatic2;
HWND De, Ue, len,kg,bl,ey1,ey2, re;
HWND hState;
BOOL CALLBACK PhysicalProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	char tmp[50];
	TCHAR query[500];
	TCHAR test[500];
	switch (iMessage) {
	case WM_INITDIALOG:
		SetDlgItemText(hWnd, IDC_RE1, myself.len.c_str());
		SetDlgItemText(hWnd, IDC_RE2, myself.kg.c_str());
		SetDlgItemText(hWnd, IDC_RE3, myself.blood.c_str());
		SetDlgItemText(hWnd, IDC_RE4, myself.eye1.c_str());
		SetDlgItemText(hWnd, IDC_RE5, myself.eye2.c_str());
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case REIDOK:
			GetDlgItemText(hWnd, IDC_RE1, tmp, 50);
			myself.len = tmp;
			lstrcpy(tmp, "");
			GetDlgItemText(hWnd, IDC_RE2, tmp, 50);
			myself.kg = tmp;
			lstrcpy(tmp, "");
			GetDlgItemText(hWnd, IDC_RE3, tmp, 50);
			myself.blood = tmp;
			lstrcpy(tmp, "");
			GetDlgItemText(hWnd, IDC_RE4, tmp, 50);
			myself.eye1 = tmp;
			lstrcpy(tmp, "");
			GetDlgItemText(hWnd, IDC_RE5, tmp, 50);
			myself.eye2 = tmp;
			lstrcpy(tmp, "");

			wsprintf(query, TEXT("update info set len = %s, weight = %s, blood = %s, see1 = %s, see2 = %s where id = %s"), myself.len.c_str(), myself.kg.c_str(), myself.blood.c_str(), myself.eye1.c_str(), myself.eye2.c_str(), myself.ID.c_str());
			if (mysql_query(&mysql, (char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				mysql_free_result(res);
				return TRUE;
			}
			MessageBox(hWnd, TEXT("수정완료!"), NULL, MB_OK);
			EndDialog(hWnd,REIDOK);
			return TRUE;
		case RECANCEL:
			EndDialog(hWnd,RECANCEL);
			return TRUE;
			
		}
		return FALSE;
	
	}
	return FALSE;
}
void SetStateText(HWND hWnd, SYSTEMTIME st) {
	GetLocalTime(&st);
	RECT wrt;
	TCHAR str[128];
	wsprintf(str, TEXT("%d년 %d월 %d일 %d시 %d분 %d초"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	GetWindowRect(hWnd, &wrt);
	
	SendMessage(hState, SB_SETTEXT, 0, (LPARAM)str);
}
LRESULT CALLBACK WndUserProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static SYSTEMTIME st;
	TCITEM utie;
	LVCOLUMN COL;
	LVITEM LI;
	int SBPart[1];
	TCHAR test[5000];
	TCHAR query[500];
	string *info;

	switch (iMessage) {
	case WM_CREATE:
		mysql_free_result(res);
		wsprintf(query, TEXT("select * from info where id = '%s'"), myself.ID.c_str());
	//	MessageBox(hWnd, query, NULL, MB_OK);
		if (mysql_query(&mysql, (const char*)query)) {
			wsprintf(test, TEXT("%s"), mysql_error(&mysql));
			MessageBox(hWnd, test, NULL, MB_OK);
			mysql_free_result(res);
			return TRUE;
		}
		
		res = mysql_store_result(&mysql);
		wsprintf(test, TEXT("%d"), res->row_count);
		MessageBox(hWnd, test, NULL, MB_OK);
		if (res->row_count == 1) {
		//	wsprintf(test, TEXT("%d"), res->row_count);
		//	MessageBox(hWnd, test, NULL, MB_OK);
			row = mysql_fetch_row(res);
			myself.setinfo(row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
		}
		else {
			MessageBox(hWnd, TEXT("error."), NULL, MB_OK);
			exit(0);
		}
		
		mysql_free_result(res);
		
		InitCommonControls();
		
		hState = CreateStatusWindow(WS_CHILD | WS_VISIBLE, TEXT(""), hWnd, 0);
		uTab1 = CreateWindow(WC_TABCONTROL, TEXT(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 300, 30, 850, 600, hWnd, (HMENU)0, g_hInst, NULL);
		uList1 = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 310, 80, 300, 500, hWnd, NULL, g_hInst, NULL);
		uList2 = CreateWindow(WC_LISTBOX, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 700, 80, 400, 500, hWnd, NULL, g_hInst, NULL);
	//	hStatic1 = CreateWindow(TEXT("static"), TEXT("주치의"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)1, g_hInst, NULL);
	//	hStatic2 = CreateWindow(TEXT("static"), TEXT("123"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)2, g_hInst, NULL);
		Ue = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 120, 98, 100, 25, hWnd, (HMENU)100, g_hInst, NULL);
		len = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 120, 138, 100, 25, hWnd, (HMENU)101, g_hInst, NULL);
		kg = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 120, 178, 100, 25, hWnd, (HMENU)102, g_hInst, NULL);
		bl = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 120, 218, 100, 25, hWnd, (HMENU)103, g_hInst, NULL);
		ey1 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 120, 258, 48, 25, hWnd, (HMENU)104, g_hInst, NULL);
		ey2 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 172, 258, 48, 25, hWnd, (HMENU)105, g_hInst, NULL);
		re = CreateWindow(TEXT("button"), TEXT("등록/수정"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 330, 80, 25, hWnd, (HMENU)778, g_hInst, NULL);
	//	De = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 1200, 98, 100, 25, hWnd, (HMENU)100, g_hInst, NULL);
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

	//	SetWindowText(hStatic1, TEXT("주치의"));
	//	SetWindowText(hStatic2, TEXT("123"));
		SetWindowText(len, myself.len.c_str());
		SetWindowText(kg, myself.kg.c_str());
		SetWindowText(Ue, myself.name.c_str());
		SetWindowText(bl, myself.blood.c_str());
		SetWindowText(ey1, myself.eye1.c_str());
		SetWindowText(ey2, myself.eye2.c_str());

		SetTimer(hWnd, 1, 1000, NULL);
		
		return 0;
	case WM_COMMAND:
		switch (wParam) {
		case 778:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_PHYSICAL), hWnd, PhysicalProc);
			SetWindowText(len, myself.len.c_str());
			SetWindowText(kg, myself.kg.c_str());
			SetWindowText(bl, myself.blood.c_str());
			SetWindowText(ey1, myself.eye1.c_str());
			SetWindowText(ey2, myself.eye2.c_str());
			return TRUE;
		}
		return 0;
	case WM_TIMER:
		SetStateText(hWnd,st);
		return 0;
	case WM_SIZE:
//		MoveWindow(hStatic1, LOWORD(lParam) / 2+430 , HIWORD(lParam) / 2-260, 100, 25, TRUE);
//		MoveWindow(hStatic2, LOWORD(lParam) / 2 + 430, HIWORD(lParam) / 2 - 200, 100, 25, TRUE);
		SendMessage(hState, WM_SIZE, wParam, lParam);
		for (int i = 0; i < 1; i++) {
			SBPart[i] = LOWORD(lParam); // 1 * (i + 1);
		}
	
		SendMessage(hState, SB_GETRECT, 1, (LPARAM)SBPart);
		
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 30, 100, TEXT("이름"), 4);
		TextOut(hdc, 30, 140, TEXT("키"), 2);
		TextOut(hdc, 30, 180, TEXT("몸무게"), 6);
		TextOut(hdc, 30, 220, TEXT("혈액형"), 6);
		TextOut(hdc, 30, 260, TEXT("시력(좌,우)"), 11);
		return 0;
	case WM_DESTROY:
		mysql_close(&mysql);
		KillTimer(hWnd, 1);
		mysql_close(&mysql2);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}