#include "User.h"
#include "Login.h"
#include "Hospital.h"

extern vector<Record> r;

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

			wsprintf(query, TEXT("update info set len = '%s', weight = '%s', blood = '%s', see1 = '%s', see2 = '%s' where id = '%s'"), myself.len.c_str(), myself.kg.c_str(), myself.blood.c_str(), myself.eye1.c_str(), myself.eye2.c_str(), myself.ID.c_str());
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
	Record Re_;
	PAINTSTRUCT ps;
	static SYSTEMTIME st;
	TCITEM utie;
	LVCOLUMN COL;
	LVITEM LI;
	int SBPart[1];
	TCHAR test[5000];
	TCHAR query[500];
	string *info;
	int num;
	char tmp__[10] = { 0 };
	char md5k[100] = { 0 };
	char shak_[100] = { 0 };
	char real[100] = { 0 };
	char ttt[50];
	char time_[100];
	unsigned char digest__[16];
	unsigned char digest___[16];
	unsigned char digest2__[SHA256_DIGEST_LENGTH];

	switch (iMessage) {
	case WM_CREATE:
		uTab1 = CreateWindow(WC_TABCONTROL, TEXT(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 300, 30, 850, 600, hWnd, (HMENU)0, g_hInst, NULL);
		uList1 = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 310, 80, 300, 500, hWnd, NULL, g_hInst, NULL);
		uList2 = CreateWindow(WC_LISTBOX, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 700, 80, 400, 500, hWnd, NULL, g_hInst, NULL);
		Ue = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 120, 98, 100, 25, hWnd, (HMENU)100, g_hInst, NULL);
		len = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 120, 138, 100, 25, hWnd, (HMENU)101, g_hInst, NULL);
		kg = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 120, 178, 100, 25, hWnd, (HMENU)102, g_hInst, NULL);
		bl = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 120, 218, 100, 25, hWnd, (HMENU)103, g_hInst, NULL);
		ey1 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 120, 258, 48, 25, hWnd, (HMENU)104, g_hInst, NULL);
		ey2 = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 172, 258, 48, 25, hWnd, (HMENU)105, g_hInst, NULL);
		re = CreateWindow(TEXT("button"), TEXT("등록/수정"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 330, 80, 25, hWnd, (HMENU)778, g_hInst, NULL);

		
		mysql_free_result(res);
		wsprintf(query, TEXT("select * from info where id = '%s'"), myself.ID.c_str());
		if (mysql_query(&mysql, (const char*)query)) {
			wsprintf(test, TEXT("%s"), mysql_error(&mysql));
			MessageBox(hWnd, test, NULL, MB_OK);
			mysql_free_result(res);
			return TRUE;
		}
		
		res = mysql_store_result(&mysql);
		if (res->row_count == 1) {
			row = mysql_fetch_row(res);
			myself.setinfo(row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
		}
		else {
			MessageBox(hWnd, TEXT("error."), NULL, MB_OK);
			exit(0);
		}
		
		mysql_free_result(res);
		InitCommonControls();
		
		
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

		COL.pszText = TEXT("주치의");
		COL.iSubItem = 1;
		SendMessage(uList1, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = TEXT("증상");
		COL.iSubItem = 2;
		SendMessage(uList1, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		SetWindowText(len, myself.len.c_str());
		SetWindowText(kg, myself.kg.c_str());
		SetWindowText(Ue, myself.name.c_str());
		SetWindowText(bl, myself.blood.c_str());
		SetWindowText(ey1, myself.eye1.c_str());
		SetWindowText(ey2, myself.eye2.c_str());


		r.clear();
		//md5
		MD5_CTX context___;
		MD5_Init(&context___);
		MD5_Update(&context___, (char*)myself.pw.c_str(), lstrlen(myself.pw.c_str()));
		MD5_Final(digest__, &context___);

		for (int i = 0; i < 16; ++i) {
			wsprintf(tmp__, TEXT("%02x"), digest__[i]);
			lstrcat(md5k, tmp__);
		}

		SHA256_CTX ctx2_1_;
		SHA256_Init(&ctx2_1_);
		SHA256_Update(&ctx2_1_, (char*)myself.pw.c_str(), lstrlen(myself.pw.c_str()));
		SHA256_Final(digest2__, &ctx2_1_);

		for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
			wsprintf(tmp__, TEXT("%02x"), digest2__[i]);
			lstrcat(shak_, tmp__);
		}

		lstrcat(md5k, shak_);

		MD5_CTX context____;
		MD5_Init(&context____);
		MD5_Update(&context____, (char*)md5k, lstrlen(md5k));
		MD5_Final(digest___, &context____);

		for (int i = 0; i < 16; ++i) {
			wsprintf(tmp__, TEXT("%02x"), digest___[i]);
			lstrcat(real, tmp__);
		}

		wsprintf(query, TEXT("select aes_decrypt(unhex(record),'%s','AAAAAAAAAAAAAAAA'),doctor,time,object,symptom,id from record where id = '%s'"), real, myself.ID.c_str());
		if (mysql_query(&mysql, (char*)query)) {
			wsprintf(test, TEXT("%s"), mysql_error(&mysql));
			MessageBox(hWnd, test, NULL, MB_OK);
			mysql_free_result(res);
			return TRUE;
		}
		res = mysql_store_result(&mysql);

		while (row = mysql_fetch_row(res))
		{
			Re_.reset();
			Re_.record = row[0];
			Re_.dname = row[1];
			Re_.time = row[2];
			Re_.object = row[3];
			Re_.symptol = row[4];
			Re_.id = row[5];

			r.push_back(Re_);
		}
		LI.mask = LVIF_TEXT;

		num = 0;
		for (int i = 0; i < r.size(); i++) {
			if (lstrcmp(r.at(i).object.c_str(), MedicalDepartment[TabCtrl_GetCurSel(uTab1)]) == 0) {
				LI.iItem = num++;
				LI.iSubItem = 0;
				lstrcpy(ttt, r.at(i).time.c_str());
				LI.pszText = ttt;
				SendMessage(uList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
				lstrcpy(ttt, "");

				LI.iSubItem = 1;
				lstrcpy(ttt, r.at(i).dname.c_str());
				LI.pszText = ttt;
				SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
				lstrcpy(ttt, "");

				LI.iSubItem = 2;
				lstrcpy(ttt, r.at(i).symptol.c_str());
				LI.pszText = ttt;
				SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
				lstrcpy(ttt, "");
			}
		}

		SetTimer(hWnd, 1, 1000, NULL);
		
		return 0;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMLISTVIEW nlv;
		LPNMITEMACTIVATE nia;

		hdr = (LPNMHDR)lParam;
		nlv = (LPNMLISTVIEW)lParam;

		if (hdr->hwndFrom == uList1) {
			switch (hdr->code) {

			case NM_CLICK:
				SendMessage(uList2, LB_DELETESTRING, 0, (LPARAM)&LI);
				nia = (LPNMITEMACTIVATE)lParam;
				ListView_GetItemText(uList1, nia->iItem, 0, time_, 100);
				for (int i = 0; i < r.size(); i++) {
					if (lstrcmp(time_, r.at(i).time.c_str()) == 0) {
						SendMessage(uList2, LB_ADDSTRING, 0, (LPARAM)r.at(i).record.c_str());
						break;
					}
				}
				break;
			}
		}
		switch (((LPNMHDR)lParam)->code) {
		case TCN_SELCHANGE:
			LI.mask = LVIF_TEXT;
			num = 0;
			SendMessage(uList1, LVM_DELETEALLITEMS, 0, (LPARAM)&LI);
			if (TabCtrl_GetCurSel(uTab1) == 0) {
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), "내과") == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
			}
			else if (TabCtrl_GetCurSel(uTab1) == 1) {
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), "외과") == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
			}
			else if (TabCtrl_GetCurSel(uTab1) == 2) {
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), "정형외과") == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
			}
			else if (TabCtrl_GetCurSel(uTab1) == 3) {
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), "치과") == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
			}
			else if (TabCtrl_GetCurSel(uTab1) == 4) {
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), "이비인후과") == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(uList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
			}

		}
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