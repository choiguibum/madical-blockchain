#include "Login.h"

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "libcrypto-1_1.dll")
#define DB_HOST "172.30.1.40"
#define DB_USER "medi"
#define DB_PASS "1234"
#define DB_NAME "medi"
#define HAVE_STRUCT_TIMESPEC

MYSQL mysql;
MYSQL_RES	*res;
MYSQL_ROW	row;

extern HINSTANCE g_hInst;
extern int UH;
BOOL CALLBACK UserCreateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {//회원가입
	TCHAR id[50];
	TCHAR id2[50];
	TCHAR pw[50];
	TCHAR pw1[50];
	TCHAR name[50];
	TCHAR birthday[50];
	TCHAR pnum[50];
	TCHAR test[20];
	int len;
	unsigned int fields=999;
	char buf[4096];
	MD5_CTX change;
	TCHAR query[500];
	int sex=0;
	int check = 0;
	
	switch (iMessage) {
	case WM_INITDIALOG:
		

		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CFIND://중복검사
			GetDlgItemText(hWnd, IDC_CID, id2, 50);
			check = 0;
			if (lstrcmp(TEXT(""), id2) == 0) {
				MessageBox(hWnd, TEXT("아이디를 입력하세요"), NULL, MB_OK);
				return TRUE;
			}
			wsprintf(query, TEXT("select count(*) from user where id = %s"),(char*)id2);
			if (!mysql_query(&mysql, (char*)query)) {
				MessageBox(hWnd, TEXT("mysql 추가오류"), NULL, MB_OK);
				return TRUE;
			}

			res = mysql_store_result(&mysql);
			MessageBox(hWnd, TEXT("3"), NULL, MB_OK);
			//fields = (int)mysql_num_fields(row);
			row = mysql_fetch_row(res);
			
			MessageBox(hWnd, TEXT("4"), NULL, MB_OK);
			if (row > 0) {
				MessageBox(hWnd, TEXT("종복되는 아이디가 있습니다."), NULL, MB_OK);
				return TRUE;
			}
			else {
					MessageBox(hWnd, TEXT("사용 가능한 아이디입니다."), NULL, MB_OK);
				}
		//	MessageBox(hWnd, TEXT("4"), NULL, MB_OK);
		//	wsprintf(test, TEXT("%d"),fields);
		//	MessageBox(hWnd, test, NULL, MB_OK);
		//	if(sql_result->row_count > 0){
		//		MessageBox(hWnd, TEXT("종복되는 아이디가 있습니다."), NULL, MB_OK);
		//		return TRUE;
		//	}
		//	else {
		//		MessageBox(hWnd, TEXT("사용 가능한 아이디입니다."), NULL, MB_OK);
		//	}
			
			

		
			return TRUE;
		case IDCREATE://가입
			GetDlgItemText(hWnd, IDC_CID, id, 50);
			GetDlgItemText(hWnd, IDC_CPW1, pw, 50);
			GetDlgItemText(hWnd, IDC_CPW2, pw1, 50);
			GetDlgItemText(hWnd, IDC_CNAME, name, 50);
			GetDlgItemText(hWnd, IDC_CDAY, birthday, 50);
			GetDlgItemText(hWnd, IDC_CPNUM, pnum, 50);
			
			if (IsDlgButtonChecked(hWnd, IDC_RS1) == BST_CHECKED) sex = 1;//1=남
			else if(IsDlgButtonChecked(hWnd, IDC_RS2) == BST_CHECKED)sex = 2;
			if (check == 0) {
				MessageBox(hWnd, TEXT("중복확인 하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (sex == 0) {
				MessageBox(hWnd, TEXT("성별을 선택하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(id, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("아이디를 입력하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(id, id2) != 0) {
				MessageBox(hWnd, TEXT("중복확인한 아이디와 다릅니다."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(pw, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("비밀번호를 입력하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(pw1, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("비밀번호 확인을 입력하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(pw1, pw) != 0) {
				MessageBox(hWnd, TEXT("비밀번호 확인을 입력하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(name, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("이름을 입력하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(birthday, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("생년월일을  입력하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(pnum, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("전화번호를 입력하세요."), NULL, MB_OK);
				return TRUE;
			}

			len = strlen(buf);
			MD5_Init(&change);
			MD5_Update(&change, pw, len);
			MD5_Final((unsigned char *)buf, &change);

			wsprintf(query, TEXT("insert into user values(%s, %s, %s, %s, %s)"), name,id,pw,birthday,sex);

			if (mysql_query(&mysql, (char*)query)) {
				MessageBox(hWnd, TEXT("mysql 추가오류"), NULL, MB_OK);
				return TRUE;
			}

			EndDialog(hWnd, IDCREATE);

			////
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
BOOL CALLBACK LoginProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	TCHAR tid[50];
	TCHAR tpw[50];
	TCHAR test[123];
		
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0)) {
		wsprintf(test, TEXT("%s"), mysql_error(&mysql));
		MessageBox(hWnd, test, NULL, MB_OK);
		exit(0);
	}
	
	switch (iMessage) {
	case WM_INITDIALOG:
		//여기에 DB에서 회원 아이디 비밀번호 받아와서 벡터에 저장하는 거 만들어야함
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BLOGIN:
			GetDlgItemText(hWnd, IDC_EID, tid, 50);
			GetDlgItemText(hWnd, IDC_EPW, tpw, 50);
			if (IsDlgButtonChecked(hWnd, IDC_R1) == BST_CHECKED) UH = 1;//1=환자
			else if (IsDlgButtonChecked(hWnd, IDC_R2) == BST_CHECKED)UH = 2;//병원
			if (UH == 0) {
				MessageBox(hWnd, TEXT("환자 또는 병원을 선택 하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(tid, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("아이디를 입력하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(tpw, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("비밀번호를 입력하세요."), NULL, MB_OK);
				return TRUE;
			}
			// 비밀번호 암호화하는거 여기서만들어줘야함;



			////
			//벡터에서 ID를 찾고 찾은아이디엣 그에대응하는 비밀번호를 찾는거 만들어함



			EndDialog(hWnd, IDC_BLOGIN);
			return TRUE;
			//
			MessageBox(hWnd, TEXT("아이디 또는 비밀번호를 확인하세요"), NULL, MB_OK);
			return TRUE;
		case IDC_BCREATE:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_USERCREATE), HWND_DESKTOP, UserCreateProc);
			return TRUE;
		case IDC_BEXIT:
			exit(0);
			return TRUE;

		}
		return FALSE;


	}
	return FALSE;
}

