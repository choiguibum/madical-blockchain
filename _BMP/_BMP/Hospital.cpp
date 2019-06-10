#include "Hospital.h"

extern HINSTANCE g_hInst;
extern MYSQL mysql;
extern MYSQL mysql2;
extern MYSQL_RES	*res;
extern MYSQL_ROW	row;
extern TCHAR *MedicalDepartment[5];

static SYSTEMTIME st2;
HWND hState1;
HWND hTab1, hList1, hList2, hList3;
HWND hS1, hS2;
HWND fedit;
HWND fbu, wbu;
HWND ID, PW, wr;
HWND hWndMain;
vector<List> v;
vector<Record> r;
TCHAR query[500];
TCHAR test[500];
TCHAR uID[50], uPW[50];
char key1[100] = { 0 }, key2[100] = { 0 };
static int checknumber = 999;
char md51[50] = { 0, };
unsigned char digest_[16];

TCHAR dname[50];
TCHAR combo[50];
TCHAR recorde3[50];

BOOL CALLBACK RecordProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	char time[100];
	hWndMain = hWnd;
	switch (iMessage) {
	case WM_INITDIALOG:
		SetDlgItemText(hWnd, IDC_EE1, v.at(checknumber).len.c_str());
		SetDlgItemText(hWnd, IDC_EE2, v.at(checknumber).kg.c_str());
		SetDlgItemText(hWnd, IDC_EE3, v.at(checknumber).bl.c_str());
		SetDlgItemText(hWnd, IDC_EE4, v.at(checknumber).e1.c_str());
		SetDlgItemText(hWnd, IDC_EE5, v.at(checknumber).e2.c_str());
		for (int i = 0; i < sizeof(MedicalDepartment) / sizeof(MedicalDepartment[0]); i++) {
			SendDlgItemMessage(hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)MedicalDepartment[i]);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOKRECORD:
			TCHAR recorde2[1000];
			char crecorde[1000];
			GetDlgItemText(hWnd, IDC_RECORD1, dname, 50);
			GetDlgItemText(hWnd, IDC_COMBO1, combo, 50);
			GetDlgItemText(hWnd, IDC_RECORD2, recorde2, 1000);
			GetDlgItemText(hWnd, IDC_RECORD3, recorde3, 50);
			if (lstrcmp(dname, "") == 0) {
				MessageBox(hWnd, TEXT("주치의를 입력해주세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(combo, "") == 0) {
				MessageBox(hWnd, TEXT("진료과를 선택해주세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(recorde3, "") == 0) {
				MessageBox(hWnd, TEXT("증상을 입력해주세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(recorde2, "") == 0) {
				MessageBox(hWnd, TEXT("진단내용을 입력해주세요."), NULL, MB_OK);
				return TRUE;
			}

			unsigned char digest1_1[16];
			unsigned char digest1_2[16];
			unsigned char digest5_1[SHA256_DIGEST_LENGTH];
			char tmp1_1[100];
			MD5_CTX context1_1;
			MD5_Init(&context1_1);
			MD5_Update(&context1_1, (char*)uPW, lstrlen(uPW));
			MD5_Final(digest1_1, &context1_1);
			lstrcpy(key1, "");
			lstrcpy(tmp1_1, TEXT(""));
			for (int i = 0; i < 16; ++i) {
				wsprintf(tmp1_1, TEXT("%02x"), digest1_1[i]);
				lstrcat(key1, tmp1_1);
			}

			SHA256_CTX ctx3_1;
			SHA256_Init(&ctx3_1);
			SHA256_Update(&ctx3_1, (char*)uPW, lstrlen(uPW));
			SHA256_Final(digest5_1, &ctx3_1);
			lstrcpy(key2, "");
			lstrcpy(tmp1_1, TEXT(""));
			for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
				wsprintf(tmp1_1, TEXT("%02x"), digest5_1[i]);
				lstrcat(key2, tmp1_1);
			}

			lstrcat(key1, key2);

			lstrcpy(key2, "");
			lstrcpy(tmp1_1, TEXT(""));
			MD5_CTX context1_12;
			MD5_Init(&context1_12);
			MD5_Update(&context1_12, (char*)key1, lstrlen(key1));
			MD5_Final(digest1_2, &context1_12);
			for (int i = 0; i < 16; ++i) {
				wsprintf(tmp1_1, TEXT("%02x"), digest1_2[i]);
				lstrcat(key2, tmp1_1);
			}


			//////////////////////////////////////////////////////////////////////////////////
			wsprintf(time, TEXT("%d-%d-%d-%d:%d"), st2.wYear, st2.wMonth, st2.wDay, st2.wHour, st2.wMinute);

			wsprintf(query, TEXT("insert into record values(HEX(AES_ENCRYPT('%s','%s','AAAAAAAAAAAAAAAA')), '%s', '%s', '%s', '%s', '%s')"), recorde2, key2, dname, time, combo, recorde3, v.at(checknumber).id.c_str());
			if (mysql_query(&mysql, (char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWndMain, test, NULL, MB_OK);
				mysql_free_result(res);
				return TRUE;
			}


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
	st2 = st;
	RECT wrt;
	TCHAR str[128];
	wsprintf(str, TEXT("%d년 %d월 %d일 %d시 %d분 %d초"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	GetWindowRect(hWnd, &wrt);

	SendMessage(hState1, SB_SETTEXT, 0, (LPARAM)str);
}
LRESULT CALLBACK WndHospitalProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	Record Re;
	PAINTSTRUCT ps;
	TCITEM utie;
	LVCOLUMN COL, FOL;
	LVITEM LI;
	TCHAR name[50];
	static SYSTEMTIME st;
	char ttt[50];
	int num;
	char md5k[100];
	char shak[100];
	char realkey[100] = { 0 };
	char sha256_[100] = { 0, };
	char time_[100];
	unsigned char digest4[SHA256_DIGEST_LENGTH];
	char tmp_[10];
	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hState1 = CreateStatusWindow(WS_CHILD | WS_VISIBLE, TEXT(""), hWnd, 0);
		hTab1 = CreateWindow(WC_TABCONTROL, TEXT(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 300, 30, 850, 600, hWnd, (HMENU)10, g_hInst, NULL);
		hList1 = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 310, 80, 300, 500, hWnd, NULL, g_hInst, NULL);
		hList2 = CreateWindow(WC_LISTBOX, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 700, 80, 400, 500, hWnd, NULL, g_hInst, NULL);
		hList3 = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 50, 150, 200, 300, hWnd, NULL, g_hInst, NULL);
		fedit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 80, 98, 100, 25, hWnd, (HMENU)12, g_hInst, NULL);
		fbu = CreateWindow(TEXT("button"), TEXT("검색"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 190, 98, 75, 25, hWnd, (HMENU)13, g_hInst, NULL);
		wbu = CreateWindow(TEXT("button"), TEXT("진단기록추가"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1200, 98, 100, 25, hWnd, (HMENU)14, g_hInst, NULL);
		ID = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 50, 480, 100, 25, hWnd, (HMENU)15, g_hInst, NULL);
		PW = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD, 50, 520, 100, 25, hWnd, (HMENU)16, g_hInst, NULL);
		wr = CreateWindow(TEXT("button"), TEXT("확인"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 170, 520, 80, 25, hWnd, (HMENU)17, g_hInst, NULL);
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

		COL.pszText = TEXT("주치의");
		COL.iSubItem = 1;
		SendMessage(hList1, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = TEXT("증상");
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
		if (hdr->hwndFrom == hList1) {
			switch (hdr->code) {

			case NM_CLICK:
				SendMessage(hList2, LB_DELETESTRING, 0, (LPARAM)&LI);
				nia = (LPNMITEMACTIVATE)lParam;
				ListView_GetItemText(hList1, nia->iItem, 0, time_, 100);
				for (int i = 0; i < r.size(); i++) {
					if (lstrcmp(time_, r.at(i).time.c_str()) == 0) {
						SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)r.at(i).record.c_str());
						break;
					}
				}
				break;
			}
		}
		if (hdr->hwndFrom == hList3) {
			switch (hdr->code) {
			case NM_CLICK:
				SendMessage(hList2, LB_DELETESTRING, 0, (LPARAM)&LI);
				SendMessage(hList1, LVM_DELETEALLITEMS, 0, (LPARAM)&LI);
				r.clear();
				nia = (LPNMITEMACTIVATE)lParam;
				ListView_GetItemText(hList3, nia->iItem, 0, NULL, NULL);
				SetWindowText(ID, v.at(nia->iItem).id.c_str());
				checknumber = nia->iItem;


				//md5
				wsprintf(query, TEXT("select pw from user where id = '%s'"), v.at(nia->iItem).id.c_str());
				if (mysql_query(&mysql, (char*)query)) {
					wsprintf(test, TEXT("%s"), mysql_error(&mysql));
					MessageBox(hWnd, test, NULL, MB_OK);
					mysql_free_result(res);
					return TRUE;
				}

				res = mysql_store_result(&mysql);
				row = mysql_fetch_row(res);
				lstrcpy(md5k, row[0]);
				//	MessageBox(hWnd, md5k, NULL, MB_OK);

				SHA256_CTX ctx2_1;
				SHA256_Init(&ctx2_1);
				SHA256_Update(&ctx2_1, (char*)v.at(nia->iItem).id.c_str(), lstrlen(v.at(nia->iItem).id.c_str()));
				SHA256_Final(digest4, &ctx2_1);

				for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
					wsprintf(tmp_, TEXT("%02x"), digest4[i]);
					lstrcat(sha256_, tmp_);
				}

				//	mysql_free_result(res);
				wsprintf(query, TEXT("select pw from pw where id = '%s'"), sha256_);
				if (mysql_query(&mysql2, (char*)query)) {
					wsprintf(test, TEXT("%s"), mysql_error(&mysql2));
					MessageBox(hWnd, test, NULL, MB_OK);
					mysql_free_result(res);
					return TRUE;
				}
				res = mysql_store_result(&mysql2);
				row = mysql_fetch_row(res);
				lstrcpy(shak, row[0]);
				//	MessageBox(hWnd, shak, NULL, MB_OK);
				//	mysql_free_result(res);
				//md5+sha=md5;
				lstrcat(md5k, shak);
				//	MessageBox(hWnd, md5k, NULL, MB_OK);
				MD5_CTX context__;
				MD5_Init(&context__);
				MD5_Update(&context__, (char*)md5k, lstrlen(md5k));
				MD5_Final(digest_, &context__);

				for (int i = 0; i < 16; ++i) {
					wsprintf(tmp_, TEXT("%02x"), digest_[i]);
					lstrcat(realkey, tmp_);
				}

				//	MessageBox(hWnd, realkey, NULL, MB_OK);
				wsprintf(query, TEXT("select aes_decrypt(unhex(record),'%s','AAAAAAAAAAAAAAAA'),doctor,time,object,symptom,id from record where id = '%s'"), realkey, v.at(nia->iItem).id.c_str());
				if (mysql_query(&mysql, (char*)query)) {
					wsprintf(test, TEXT("%s"), mysql_error(&mysql));
					MessageBox(hWnd, test, NULL, MB_OK);
					mysql_free_result(res);
					return TRUE;
				}

				res = mysql_store_result(&mysql);

				while (row = mysql_fetch_row(res))
				{
					//	MessageBox(hWnd, (char*)row[0], NULL, MB_OK);
					Re.reset();
					//	MessageBox(hWnd, TEXT("0"), NULL, MB_OK);
					Re.record = row[0];
					//	MessageBox(hWnd, TEXT("1"), NULL, MB_OK);
					Re.dname = row[1];
					Re.time = row[2];
					Re.object = row[3];
					Re.symptol = row[4];
					Re.id = row[5];

					r.push_back(Re);
				}
				/////////////////////////////
				LI.mask = LVIF_TEXT;

				num = 0;
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), MedicalDepartment[TabCtrl_GetCurSel(hTab1)]) == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
				break;
			}
			return 0;
		}
		switch (((LPNMHDR)lParam)->code) {
		case TCN_SELCHANGE:
			LI.mask = LVIF_TEXT;
			num = 0;
			SendMessage(hList1, LVM_DELETEALLITEMS, 0, (LPARAM)&LI);
			if (TabCtrl_GetCurSel(hTab1) == 0) {
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), "내과") == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
			}
			else if (TabCtrl_GetCurSel(hTab1) == 1) {
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), "외과") == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
			}
			else if (TabCtrl_GetCurSel(hTab1) == 2) {
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), "정형외과") == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
			}
			else if (TabCtrl_GetCurSel(hTab1) == 3) {
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), "치과") == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
			}
			else if (TabCtrl_GetCurSel(hTab1) == 4) {
				for (int i = 0; i < r.size(); i++) {
					if (strcmp(r.at(i).object.c_str(), "이비인후과") == 0) {
						LI.iItem = num++;
						LI.iSubItem = 0;
						lstrcpy(ttt, r.at(i).time.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 1;
						lstrcpy(ttt, r.at(i).dname.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");

						LI.iSubItem = 2;
						lstrcpy(ttt, r.at(i).symptol.c_str());
						LI.pszText = ttt;
						SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
						lstrcpy(ttt, "");
					}
				}
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
			lstrcpy(uID, id);
			GetWindowText(PW, pw, 50);
			lstrcpy(uPW, pw);
			if (lstrcmp(pw, TEXT("")) == 0) {
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

					SendMessage(hList1, LVM_DELETEALLITEMS, 0, (LPARAM)&LI);
					r.clear();

					wsprintf(query, TEXT("select aes_decrypt(unhex(record),'%s','AAAAAAAAAAAAAAAA'),doctor,time,object,symptom,id from record where id = '%s'"), key2, v.at(checknumber).id);
					if (mysql_query(&mysql, (char*)query)) {
						wsprintf(test, TEXT("%s"), mysql_error(&mysql));
						MessageBox(hWnd, test, NULL, MB_OK);
						mysql_free_result(res);
						return TRUE;
					}
					res = mysql_store_result(&mysql);

					while (row = mysql_fetch_row(res))
					{
						Re.reset();
						Re.record = row[0];
						Re.dname = row[1];
						Re.time = row[2];
						Re.object = row[3];
						Re.symptol = row[4];
						Re.id = row[5];
						r.push_back(Re);
					}
					LI.mask = LVIF_TEXT;

					num = 0;
					for (int i = 0; i < r.size(); i++) {
						if (strcmp(r.at(i).object.c_str(), MedicalDepartment[TabCtrl_GetCurSel(hTab1)]) == 0) {
							LI.iItem = num++;
							LI.iSubItem = 0;
							lstrcpy(ttt, r.at(i).time.c_str());
							LI.pszText = ttt;
							SendMessage(hList1, LVM_INSERTITEM, 0, (LPARAM)&LI);
							lstrcpy(ttt, "");

							LI.iSubItem = 1;
							lstrcpy(ttt, r.at(i).dname.c_str());
							LI.pszText = ttt;
							SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
							lstrcpy(ttt, "");

							LI.iSubItem = 2;
							lstrcpy(ttt, r.at(i).symptol.c_str());
							LI.pszText = ttt;
							SendMessage(hList1, LVM_SETITEM, 0, (LPARAM)&LI);
							lstrcpy(ttt, "");
						}
					}
				}

			}
			return TRUE;

		case 13://이름검색
			r.clear();
			SendMessage(hList1, LVM_DELETEALLITEMS, 0, (LPARAM)&LI);
			SetWindowText(ID, TEXT(""));
			v.clear();
			SendMessage(hList3, LVM_DELETEALLITEMS, 0, (LPARAM)&LI);
			List list;
			GetWindowText(fedit, name, 50);
			mysql_free_result(res);
			wsprintf(query, TEXT("select * from user where name = '%s'"), name);

			if (mysql_query(&mysql, (const char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				return TRUE;
			}
			res = mysql_store_result(&mysql);

			while (row = mysql_fetch_row(res))
			{
				list.reset();
				list.name = row[0];
				list.id = row[1];
				list.birth = row[3];
				v.push_back(list);
			}
			mysql_free_result(res);
			wsprintf(query, TEXT("select * from info where name = '%s'"), name);

			if (mysql_query(&mysql, (const char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				return TRUE;
			}
			res = mysql_store_result(&mysql);

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
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 30, 100, TEXT("이름"), 4);
		TextOut(hdc, 20, 480, TEXT("ID"), 2);
		TextOut(hdc, 20, 520, TEXT("PW"), 2);
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