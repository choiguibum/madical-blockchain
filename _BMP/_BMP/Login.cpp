#include "Login.h"

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "libcrypto-1_1.dll")
#define DB_HOST "192.168.1.73"
#define DB_USER "medi"
#define DB_PASS "1234"
#define DB_NAME "medi"
#define HAVE_STRUCT_TIMESPEC

MYSQL mysql;
MYSQL_RES	*res;
MYSQL_ROW	row;

extern HINSTANCE g_hInst;
extern int UH;
BOOL CALLBACK UserCreateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {//ȸ������
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
	
	TCHAR query[500];
	int sex=0;
	static int check = 0;
	
	switch (iMessage) {
	case WM_INITDIALOG:
		

		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CFIND://�ߺ��˻�
			GetDlgItemText(hWnd, IDC_CID, id2, 50);
			check = 0;
			if (lstrcmp(TEXT(""), id2) == 0) {
				MessageBox(hWnd, TEXT("���̵� �Է��ϼ���"), NULL, MB_OK);
				return TRUE;
			}
			mysql_init(&mysql);
			if (!mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				exit(0);
			}
			wsprintf(query, TEXT("select * from user where id = '%s'"), (char*)id2);
			if (mysql_query(&mysql, (const char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				mysql_close(&mysql);
				return TRUE;
			}
			res = mysql_store_result(&mysql);
			if (res->row_count > 0) {
				wsprintf(test, TEXT("%d"), res->row_count);
				MessageBox(hWnd, test, NULL, MB_OK);
				MessageBox(hWnd, TEXT("�̹� �����ϴ� ���̵��Դϴ�."), NULL, MB_OK);
			}
			else {
				check = 1;
				MessageBox(hWnd, TEXT("��� ������ ���̵��Դϴ�."), NULL, MB_OK);
			}		
			mysql_free_result(res);
			mysql_close(&mysql);
			return TRUE;
		case CCANCLE:
			EndDialog(hWnd, CCANCLE);
			return TRUE;
		case IDCREATE://����
			GetDlgItemText(hWnd, IDC_CID, id, 50);
			GetDlgItemText(hWnd, IDC_CPW1, pw, 50);
			GetDlgItemText(hWnd, IDC_CPW2, pw1, 50);
			GetDlgItemText(hWnd, IDC_CNAME, name, 50);
			GetDlgItemText(hWnd, IDC_CDAY, birthday, 50);
			GetDlgItemText(hWnd, IDC_CPNUM, pnum, 50);
			
			if (IsDlgButtonChecked(hWnd, IDC_RS1) == BST_CHECKED) sex = 1;//1=��
			else if(IsDlgButtonChecked(hWnd, IDC_RS2) == BST_CHECKED)sex = 2;
			if (check == 0) {
				MessageBox(hWnd, TEXT("�ߺ�Ȯ�� �ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (sex == 0) {
				MessageBox(hWnd, TEXT("������ �����ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(id, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("���̵� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(id, id2) != 0) {
				MessageBox(hWnd, TEXT("�ߺ�Ȯ���� ���̵�� �ٸ��ϴ�."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(pw, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("��й�ȣ�� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(pw1, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("��й�ȣ Ȯ���� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(pw1, pw) != 0) {
				MessageBox(hWnd, TEXT("��й�ȣ Ȯ���� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(name, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("�̸��� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(birthday, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("���������  �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(pnum, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("��ȭ��ȣ�� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
		
			int i;
			unsigned char digest[16];
			char md5[50] = { 0, };

			MD5_CTX context;
			MD5_Init(&context);
			MD5_Update(&context, (char*)pw, lstrlen(pw));
			MD5_Final(digest, &context);

			for (i = 0; i < 16; ++i) {
				sprintf_s(md5 + (i * 2), sizeof(md5), "%02x", digest[i]);
			}
			wsprintf(test, TEXT("%s"), md5);
			MessageBox(hWnd, test, NULL, MB_OK);

			mysql_init(&mysql);
			if (!mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				exit(0);
			}
			wsprintf(query, TEXT("insert into user values('%s', '%s', '%s', '%s', '%d', '%s')"), (char*)name, (char*)id, (char*)md5, (char*)birthday, (char*)sex, (char *)pnum);
			if (mysql_query(&mysql, (char*)query)) {
				wsprintf(test, TEXT("%s"), mysql_error(&mysql));
				MessageBox(hWnd, test, NULL, MB_OK);
				return TRUE;
			}
			else {
				MessageBox(hWnd, TEXT("���ԿϷ�!!"), NULL, MB_OK);
			}
			MessageBox(hWnd, TEXT("1"), NULL, MB_OK);
			res = mysql_store_result(&mysql);
			MessageBox(hWnd, TEXT("2"), NULL, MB_OK);

			mysql_free_result(res);
			MessageBox(hWnd, TEXT("3"), NULL, MB_OK);

			mysql_close(&mysql);
			MessageBox(hWnd, TEXT("4"), NULL, MB_OK);

			EndDialog(hWnd, IDCREATE);
			MessageBox(hWnd, TEXT("5"), NULL, MB_OK);

			return TRUE;
		
		}
		return FALSE;
	}
	return FALSE;
}
BOOL CALLBACK LoginProc(HWND hWnd1, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	TCHAR tid[50];
	TCHAR tpw[50];
	TCHAR test[123];
		
	switch (iMessage) {
	case WM_INITDIALOG:

		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BLOGIN:
			GetDlgItemText(hWnd1, IDC_EID, tid, 50);
			GetDlgItemText(hWnd1, IDC_EPW, tpw, 50);
			if (IsDlgButtonChecked(hWnd1, IDC_R1) == BST_CHECKED) UH = 1;//1=ȯ��
			else if (IsDlgButtonChecked(hWnd1, IDC_R2) == BST_CHECKED)UH = 2;//����
			if (UH == 0) {
				MessageBox(hWnd1, TEXT("ȯ�� �Ǵ� ������ ���� �ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(tid, TEXT("")) == 0) {
				MessageBox(hWnd1, TEXT("���̵� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(tpw, TEXT("")) == 0) {
				MessageBox(hWnd1, TEXT("��й�ȣ�� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}

			// ��й�ȣ ��ȣȭ�ϴ°� ���⼭����������;



			////
			//���Ϳ��� ID�� ã�� ã�����̵� �׿������ϴ� ��й�ȣ�� ã�°� �������



			EndDialog(hWnd1, IDC_BLOGIN);
			return TRUE;
			//
			MessageBox(hWnd1, TEXT("���̵� �Ǵ� ��й�ȣ�� Ȯ���ϼ���"), NULL, MB_OK);
			return TRUE;
		case IDC_BCREATE:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_USERCREATE), HWND_DESKTOP, UserCreateProc);
			MessageBox(hWnd1, TEXT("6"), NULL, MB_OK);

			return TRUE;
		case IDC_BEXIT:
			exit(0);
			return TRUE;

		}
		return FALSE;


	}
	return FALSE;
}

