#include "Login.h"
#include <my_global.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")
#define DB_HOST "192.168.0.11"
#define DB_USER "medi"
#define DB_PASS "1234"
#define DB_NAME "medi"
#define HAVE_STRUCT_TIMESPEC
extern HINSTANCE g_hInst;
extern int UH;
BOOL CALLBACK UserCreateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {//회원가입
	TCHAR id[50];
	TCHAR pw[50];
	TCHAR pw1[50];
	TCHAR name[50];
	TCHAR birthday[50];
	TCHAR pnum[50];
	int sex=0;
	int check = 0;

	switch (iMessage) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CFIND://중복검사
			//디비에있는 데이터 -> 백터로 나중에구현할거임
			//구현하면서 check 가 1이면 중복없는거 중복있으면 0 기존값도 0유지

			return TRUE;
		case IDCREATE://가입
			GetDlgItemText(hWnd, IDC_CNAME, id, 50);
			GetDlgItemText(hWnd, IDC_CID, pw, 50);
			GetDlgItemText(hWnd, IDC_CPW1, pw1, 50);
			GetDlgItemText(hWnd, IDC_CPW2, name, 50);
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
			if (lstrcmp(pw, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("비밀번호를 입력하세요."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(pw1, TEXT("")) == 0) {
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

//			if()
			//벡터에 저장하고 디비에도 저장시켜야함
			int len;
			char buf[4096];
			/*while (1) {
				len = strlen(buf);
				MD5((unsigned char *)buf, len, pw); // 버퍼에 해쉬값 저장 , pw는 unsigned char형 
			}*///버퍼값을과 id를 디비에 저장해야됨

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
	
	

	MYSQL		conn;
	MYSQL*		connptr = NULL;
	MYSQL_RES	*sql_result;
	MYSQL_ROW	sql_row;

	mysql_init(&conn);
	connptr = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	//mysql 연결 실패시 에러
	if (connptr == NULL) {
		MessageBox(hWnd, TEXT("오류"), NULL, MB_OK);
		return -1;
	}
	mysql_close(connptr);
	/*MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0))
		MessageBox(hWnd, TEXT("error"), NULL, MB_OK);
	else
		MessageBox(hWnd, TEXT("success"), NULL, MB_OK);*/
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

