#ifndef LOGIN_
#define LOGIN_
#include <windows.h>
#include <iostream>
#include"resource.h"
#include <string>
#include <openssl/md5.h>
#include <my_global.h>
#include <mysql.h>
using namespace std;

class user {
private:
	string ID;
	string PW;
public:
	void setID();
	void setPW();
};

BOOL CALLBACK LoginProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK UserCreateProc(HWND, UINT, WPARAM, LPARAM);

#endif