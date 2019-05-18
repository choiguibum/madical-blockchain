#include "Login.h"

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "libcrypto-1_1.dll")
#define DB_HOST "192.168.0.12"
#define DB_USER "medi"
#define DB_PASS "1234"
#define DB_NAME "medi"
#define HAVE_STRUCT_TIMESPEC

MYSQL mysql;
MYSQL_RES	*res;
MYSQL_ROW	row;

int i;
unsigned char digest[16];
char md5[50] = { 0, };
char tmp[10];


TCHAR query[500];

extern HINSTANCE g_hInst;
extern int UH;
BOOL CALLBACK UserCreateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {//회원가입
	TCHAR id[50];
	static TCHAR id2[50];
	TCHAR pw[50];
	TCHAR pw1[50];
	TCHAR name[50];
	TCHAR birthday[50];
	TCHAR pnum[50];
	TCHAR test[5000];
	int len;
	int fields;
	
	
	int sex=0;
	static int check = 0;
	
	switch (iMessage) {
	case WM_INITDIALOG:
		mysql_init(&mysql);
		if (!mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0)) {
			wsprintf(test, TEXT("%s"), mysql_error(&mysql));
			MessageBox(hWnd, test, NULL, MB_OK);
			exit(0);
		}
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
			wsprintf(query, TEXT("select * from user where id = '%s'"), (char*)id2);
			if (mysql_query(&mysql, (const char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				mysql_free_result(res);
				return TRUE;
			}
			res = mysql_store_result(&mysql);
			if (res->row_count > 0) {
				wsprintf(test, TEXT("%d"), res->row_count);
				MessageBox(hWnd, test, NULL, MB_OK);
				MessageBox(hWnd, TEXT("이미 존재하는 아이디입니다."), NULL, MB_OK);
			}
			else {
				check = 1;
				MessageBox(hWnd, TEXT("사용 가능한 아이디입니다."), NULL, MB_OK);
			}		
			mysql_free_result(res);
			return TRUE;
		case CCANCLE:
			EndDialog(hWnd, CCANCLE);
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
		
			MD5_CTX context;
			MD5_Init(&context);
			MD5_Update(&context, (char*)pw, lstrlen(pw));
			MD5_Final(digest, &context);

			for (i = 0; i < 16; ++i) {
				wsprintf(tmp,TEXT("%02x"), digest[i]);
				lstrcat(md5, tmp);
			}
		//	wsprintf(test, TEXT("%s"), md5);
		//	MessageBox(hWnd, test, NULL, MB_OK);
			wsprintf(query, TEXT("insert into user values('%s', '%s', '%s', '%s', '%d', '%s')"), (char*)name, (char*)id, (char*)md5, (char*)birthday, sex, (char *)pnum);
			
			if (mysql_query(&mysql, (char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				mysql_free_result(res);
				return TRUE;
			}
			else {
				MessageBox(hWnd, TEXT("가입완료!!"), NULL, MB_OK);
			}

			mysql_close(&mysql);
			EndDialog(hWnd, IDCREATE);
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
		
	switch (iMessage) {
	case WM_INITDIALOG:
		mysql_init(&mysql);
		if (!mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0)) {
			wsprintf(test, TEXT("%s"), mysql_error(&mysql));
			MessageBox(hWnd, test, NULL, MB_OK);
			exit(0);
		}
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
			
			if (UH == 1) {  //유저 로그인;
				MD5_CTX context;
				MD5_Init(&context);
				MD5_Update(&context, (char*)tpw, lstrlen(tpw));
				MD5_Final(digest, &context);

				for (i = 0; i < 16; ++i) {
					wsprintf(tmp, TEXT("%02x"), digest[i]);
					lstrcat(md5, tmp);
				}

				wsprintf(query, TEXT("select * from user where id ='%s' and pw = '%s'"),(char *)tid,(char *)md5);
				
				if (mysql_query(&mysql, (char*)query)) {
					wsprintf(test, TEXT("%s"), mysql_error(&mysql));
					MessageBox(hWnd, test, NULL, MB_OK);
					mysql_free_result(res);
					return TRUE;
				}
				res = mysql_store_result(&mysql);
				if (res->row_count == 0) {
					MessageBox(hWnd, TEXT("아이디 또는 비밀번호를 확인하세요."), NULL, MB_OK);
					mysql_free_result(res);
					return TRUE;
				}
				else if (res->row_count == 1) {
					mysql_close(&mysql);
					EndDialog(hWnd, IDC_BLOGIN);
					return TRUE;
				}
			}
			else if (UH == 2) { // 병원 로그인 ;
				wsprintf(query, TEXT("select * from hospital where id ='%s' and pw = '%s'"), (char *)tid, (char *)tpw);

				if (mysql_query(&mysql, (char*)query)) {
					wsprintf(test, TEXT("%s"), mysql_error(&mysql));
					MessageBox(hWnd, test, NULL, MB_OK);
					mysql_free_result(res);
					return TRUE;
				}
				res = mysql_store_result(&mysql);
				if (res->row_count == 0) {
					MessageBox(hWnd, TEXT("아이디 또는 비밀번호를 확인하세요."), NULL, MB_OK);
					mysql_free_result(res);
					return TRUE;
				}
				else if (res->row_count == 1) {
					mysql_close(&mysql);
					EndDialog(hWnd, IDC_BLOGIN);
					return TRUE;
				}
			}
			EndDialog(hWnd, IDC_BLOGIN);
			return TRUE;
		case IDC_BCREATE:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_USERCREATE), HWND_DESKTOP, UserCreateProc);
			return TRUE;
		case IDC_BEXIT:
			mysql_close(&mysql);
			exit(0);
			return TRUE;

		}
		return FALSE;


	}
	return FALSE;
}

