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
BOOL CALLBACK UserCreateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {//ȸ������
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
		case IDC_CFIND://�ߺ��˻�
			GetDlgItemText(hWnd, IDC_CID, id2, 50);
			check = 0;
			if (lstrcmp(TEXT(""), id2) == 0) {
				MessageBox(hWnd, TEXT("���̵� �Է��ϼ���"), NULL, MB_OK);
				return TRUE;
			}
			wsprintf(query, TEXT("select count(*) from user where id = %s"),(char*)id2);
			if (!mysql_query(&mysql, (char*)query)) {
				MessageBox(hWnd, TEXT("mysql �߰�����"), NULL, MB_OK);
				return TRUE;
			}

			res = mysql_store_result(&mysql);
			MessageBox(hWnd, TEXT("3"), NULL, MB_OK);
			//fields = (int)mysql_num_fields(row);
			row = mysql_fetch_row(res);
			
			MessageBox(hWnd, TEXT("4"), NULL, MB_OK);
			if (row > 0) {
				MessageBox(hWnd, TEXT("�����Ǵ� ���̵� �ֽ��ϴ�."), NULL, MB_OK);
				return TRUE;
			}
			else {
					MessageBox(hWnd, TEXT("��� ������ ���̵��Դϴ�."), NULL, MB_OK);
				}
		//	MessageBox(hWnd, TEXT("4"), NULL, MB_OK);
		//	wsprintf(test, TEXT("%d"),fields);
		//	MessageBox(hWnd, test, NULL, MB_OK);
		//	if(sql_result->row_count > 0){
		//		MessageBox(hWnd, TEXT("�����Ǵ� ���̵� �ֽ��ϴ�."), NULL, MB_OK);
		//		return TRUE;
		//	}
		//	else {
		//		MessageBox(hWnd, TEXT("��� ������ ���̵��Դϴ�."), NULL, MB_OK);
		//	}
			
			

		
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

			len = strlen(buf);
			MD5_Init(&change);
			MD5_Update(&change, pw, len);
			MD5_Final((unsigned char *)buf, &change);

			wsprintf(query, TEXT("insert into user values(%s, %s, %s, %s, %s)"), name,id,pw,birthday,sex);

			if (mysql_query(&mysql, (char*)query)) {
				MessageBox(hWnd, TEXT("mysql �߰�����"), NULL, MB_OK);
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
		//���⿡ DB���� ȸ�� ���̵� ��й�ȣ �޾ƿͼ� ���Ϳ� �����ϴ� �� ��������
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BLOGIN:
			GetDlgItemText(hWnd, IDC_EID, tid, 50);
			GetDlgItemText(hWnd, IDC_EPW, tpw, 50);
			if (IsDlgButtonChecked(hWnd, IDC_R1) == BST_CHECKED) UH = 1;//1=ȯ��
			else if (IsDlgButtonChecked(hWnd, IDC_R2) == BST_CHECKED)UH = 2;//����
			if (UH == 0) {
				MessageBox(hWnd, TEXT("ȯ�� �Ǵ� ������ ���� �ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(tid, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("���̵� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(tpw, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("��й�ȣ�� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			// ��й�ȣ ��ȣȭ�ϴ°� ���⼭����������;



			////
			//���Ϳ��� ID�� ã�� ã�����̵� �׿������ϴ� ��й�ȣ�� ã�°� �������



			EndDialog(hWnd, IDC_BLOGIN);
			return TRUE;
			//
			MessageBox(hWnd, TEXT("���̵� �Ǵ� ��й�ȣ�� Ȯ���ϼ���"), NULL, MB_OK);
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

