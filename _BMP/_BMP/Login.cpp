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
BOOL CALLBACK UserCreateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {//ȸ������
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
		case IDC_CFIND://�ߺ��˻�
			//����ִ� ������ -> ���ͷ� ���߿������Ұ���
			//�����ϸ鼭 check �� 1�̸� �ߺ����°� �ߺ������� 0 �������� 0����

			return TRUE;
		case IDCREATE://����
			GetDlgItemText(hWnd, IDC_CNAME, id, 50);
			GetDlgItemText(hWnd, IDC_CID, pw, 50);
			GetDlgItemText(hWnd, IDC_CPW1, pw1, 50);
			GetDlgItemText(hWnd, IDC_CPW2, name, 50);
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
			if (lstrcmp(pw, TEXT("")) == 0) {
				MessageBox(hWnd, TEXT("��й�ȣ�� �Է��ϼ���."), NULL, MB_OK);
				return TRUE;
			}
			if (lstrcmp(pw1, TEXT("")) == 0) {
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

//			if()
			//���Ϳ� �����ϰ� ��񿡵� ������Ѿ���
			int len;
			char buf[4096];
			/*while (1) {
				len = strlen(buf);
				MD5((unsigned char *)buf, len, pw); // ���ۿ� �ؽ��� ���� , pw�� unsigned char�� 
			}*///���۰����� id�� ��� �����ؾߵ�

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
	//mysql ���� ���н� ����
	if (connptr == NULL) {
		MessageBox(hWnd, TEXT("����"), NULL, MB_OK);
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

