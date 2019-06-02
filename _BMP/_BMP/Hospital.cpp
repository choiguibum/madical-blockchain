#include "Hospital.h"

extern HINSTANCE g_hInst;
extern MYSQL mysql;
extern MYSQL mysql2;
extern MYSQL_RES	*res;
extern MYSQL_ROW	row;
extern TCHAR *MedicalDepartment[5];
HWND hState1;
HWND hTab1, hList1, hList2,hList3;
HWND hS1, hS2;
HWND fedit;
HWND fbu,wbu;
HWND ID, PW, wr;
vector<List> v;
TCHAR query[500];
TCHAR test[500];
static int checknumber = 999;
char md51[50] = { 0, };
//user table => 이름 / 아이디 / 암호화 비밀번호 / 생년월일 / 성별/
BOOL CALLBACK RecordProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
		case WM_INITDIALOG:
			SetDlgItemText(hWnd, IDC_EE1, v.at(checknumber).len.c_str());
			SetDlgItemText(hWnd, IDC_EE2, v.at(checknumber).kg.c_str());
			SetDlgItemText(hWnd, IDC_EE3, v.at(checknumber).bl.c_str());
			SetDlgItemText(hWnd, IDC_EE4, v.at(checknumber).e1.c_str());
			SetDlgItemText(hWnd, IDC_EE5, v.at(checknumber).e2.c_str());

			return TRUE;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDOKRECORD:
				EndDialog(hWnd, IDOKRECORD);
				return TRUE;
			case IDCANCELRECORD:
				EndDialog(hWnd, IDCANCELRECORD);
				return TRUE;
			}
			return FALSE;
	}
	return FALSE;
}

void SetStateText1(HWND hWnd, SYSTEMTIME st) {
	GetLocalTime(&st);
	RECT wrt;
	TCHAR str[128];
	wsprintf(str, TEXT("%d년 %d월 %d일 %d시 %d분 %d초"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	GetWindowRect(hWnd, &wrt);

	SendMessage(hState1, SB_SETTEXT, 0, (LPARAM)str);
}
LRESULT CALLBACK WndHospitalProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	TCITEM utie;
	LVCOLUMN COL,FOL;
	LVITEM LI;
	TCHAR name[50];
	static SYSTEMTIME st;

	switch (iMessage) {
	case WM_CREATE:
		//	MessageBox(hWnd, TEXT("병원꺼"), NULL, MB_OK);
		InitCommonControls();
		hState1 = CreateStatusWindow(WS_CHILD | WS_VISIBLE, TEXT(""), hWnd, 0);
		hTab1 = CreateWindow(WC_TABCONTROL, TEXT(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 300, 30, 850, 600, hWnd, (HMENU)10, g_hInst, NULL);
		hList1 = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 310, 80, 300, 500, hWnd, NULL, g_hInst, NULL);
		hList2 = CreateWindow(WC_LISTBOX, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 700, 80, 400, 500, hWnd, NULL, g_hInst, NULL);
		hList3 = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 50, 150, 200, 300, hWnd, NULL, g_hInst, NULL);
	//	hS1 = CreateWindow(TEXT("static"), TEXT("진단 추가"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)11, g_hInst, NULL);
		//	hStatic2 = CreateWindow(TEXT("static"), TEXT("123"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)12, g_hInst, NULL);
		fedit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 80, 98, 100, 25, hWnd, (HMENU)12, g_hInst, NULL);
		fbu = CreateWindow(TEXT("button"), TEXT("검색"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 190, 98, 75, 25, hWnd, (HMENU)13, g_hInst, NULL);
		wbu = CreateWindow(TEXT("button"), TEXT("진단기록추가"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1200, 98, 100, 25, hWnd, (HMENU)14, g_hInst, NULL);
		ID = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 50, 480, 100, 25, hWnd, (HMENU)15, g_hInst, NULL);
		PW = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 50, 520, 100, 25, hWnd, (HMENU)16, g_hInst, NULL);
		wr = CreateWindow(TEXT("button"), TEXT("검색"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 170, 520, 80, 25, hWnd, (HMENU)17, g_hInst, NULL);
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

		SetTimer(hWnd, 1, 1000, NULL);
		return 0;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMLISTVIEW nlv;
		LPNMITEMACTIVATE nia;
		hdr = (LPNMHDR)lParam;
		nlv = (LPNMLISTVIEW)lParam;
		if (hdr->hwndFrom == hList3) {
			switch (hdr->code) {
			case NM_CLICK:
				nia = (LPNMITEMACTIVATE)lParam;
				ListView_GetItemText(hList3, nia->iItem, 0, NULL, NULL);
				SetWindowText(ID, v.at(nia->iItem).id.c_str());
				checknumber=nia->iItem;
				break;
			}
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 17: //환자 비밀번호 입력받고 진단 기록하는 곳
			TCHAR pw[50];
			TCHAR id[50];
			char tmp1[50];
			lstrcpy(md51, "");
			unsigned char digest1[16];
			GetWindowText(ID, id, 50);
			GetWindowText(PW, pw, 50);
			if (lstrcmp(pw, TEXT(""))== 0) {
				MessageBox(hWnd, TEXT("비밀번호를 입력하세요."), NULL, MB_OK);
				return TRUE;
			}
			MD5_CTX context1;
			MD5_Init(&context1);
			MD5_Update(&context1, (char*)pw, lstrlen(pw));
			MD5_Final(digest1, &context1);
			for (int i = 0; i < 16; ++i) {
				wsprintf(tmp1, TEXT("%02x"), digest1[i]);
				lstrcat(md51, tmp1);
			}
			mysql_free_result(res);
			wsprintf(query, TEXT("select id,pw from user where id = '%s' and pw = '%s'"), (char*)id, md51);
			if (mysql_query(&mysql, (char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				mysql_free_result(res);
				return TRUE;
			}
			res = mysql_store_result(&mysql);
			if (res->row_count != 1) {
				MessageBox(hWnd, TEXT("비밀번호를 확인하세요11."), NULL, MB_OK);
			//	mysql_free_result(res);
				return TRUE;
			}
			else if (res->row_count == 1) {
				unsigned char digest4[SHA256_DIGEST_LENGTH];
				unsigned char digest5[SHA256_DIGEST_LENGTH];
				char sha256_1[100] = { 0, };
				char sha256_2[100] = { 0, };
				SHA256_CTX ctx3;
				SHA256_Init(&ctx3);
				SHA256_Update(&ctx3, (char*)pw, lstrlen(pw));
				SHA256_Final(digest4, &ctx3);
				lstrcpy(tmp1, TEXT(""));
				for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
					wsprintf(tmp1, TEXT("%02x"), digest4[i]);
					lstrcat(sha256_1, tmp1);
				}
				lstrcpy(tmp1, TEXT(""));
				lstrcpy(sha256_2, TEXT(""));
				SHA256_CTX ctx4;
				SHA256_Init(&ctx4);
				SHA256_Update(&ctx4, (char*)id, lstrlen(id));
				SHA256_Final(digest5, &ctx4);
				for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
					wsprintf(tmp1, TEXT("%02x"), digest5[i]);
					lstrcat(sha256_2, tmp1);
				}
				mysql_free_result(res);
				wsprintf(query, TEXT("select * from pw where id = '%s' and pw = '%s'"), (char*)sha256_2, (char*)sha256_1);
				if (mysql_query(&mysql2, (char*)query)) {
					wsprintf(test, TEXT("%s"), mysql_error(&mysql2));
					MessageBox(hWnd, test, NULL, MB_OK);
					mysql_free_result(res);
					return TRUE;
				}
				res = mysql_store_result(&mysql2);
				if (res->row_count != 1) {
					MessageBox(hWnd, TEXT("비밀번호를 확인하세요22."), NULL, MB_OK);
					mysql_free_result(res);
					return TRUE;
				}
				else if (res->row_count == 1) {
					mysql_free_result(res);
					DialogBox(g_hInst, MAKEINTRESOURCE(IDD_RECORD), hWnd, RecordProc);
					//



					///////////////////////

					//


				}

			}
			return TRUE;

		case 13:
			SetWindowText(ID, TEXT(""));
			v.clear();
			SendMessage(hList3, LVM_DELETEALLITEMS, 0, (LPARAM)&LI);
			List list;
			GetWindowText(fedit, name, 50);
			mysql_free_result(res);
			wsprintf(query, TEXT("select * from user where name = '%s'"), name);
			//	MessageBox(hWnd, query, NULL, MB_OK);
			if (mysql_query(&mysql, (const char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				return TRUE;
			}
			res = mysql_store_result(&mysql);
		//	wsprintf(test, TEXT("%d"), res->row_count);
		//	MessageBox(hWnd, test, NULL, MB_OK);
			while (row = mysql_fetch_row(res))
			{
				list.reset();
				list.name=row[0];
				list.id = row[1];
				list.birth = row[3];
				v.push_back(list);
			}
			mysql_free_result(res);
			wsprintf(query, TEXT("select * from info where name = '%s'"), name);
			//	MessageBox(hWnd, query, NULL, MB_OK);
			if (mysql_query(&mysql, (const char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				return TRUE;
			}
			res = mysql_store_result(&mysql);
		//	wsprintf(test, TEXT("%d"), res->row_count);
		//	MessageBox(hWnd, test, NULL, MB_OK);
			int count = 0;
			while (row = mysql_fetch_row(res))
			{
				v.at(count).kg = row[3];
				v.at(count).len = row[2];
				v.at(count).bl = row[4];
				v.at(count).e1 = row[5];
				v.at(count++).e2 = row[6];
			}


			LI.mask = LVIF_TEXT;
			char ttt[50];

			for (int i = 0; i < v.size(); i++) {
				LI.iItem = i;
				LI.iSubItem = 0;
				lstrcpy(ttt, v.at(i).name.c_str());
				LI.pszText = ttt;
				SendMessage(hList3, LVM_INSERTITEM, 0, (LPARAM)&LI);
				lstrcpy(ttt, "");
				LI.iSubItem = 1;
				lstrcpy(ttt, v.at(i).birth.c_str());
				
				LI.pszText = ttt;
				SendMessage(hList3, LVM_SETITEM, 0, (LPARAM)&LI);
				lstrcpy(ttt, "");
			}
			return TRUE;
		}
		return 0;
	case WM_TIMER:
		SetStateText1(hWnd, st);
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
		mysql_close(&mysql);
		mysql_close(&mysql2);
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}