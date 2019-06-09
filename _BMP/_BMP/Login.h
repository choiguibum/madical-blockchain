#ifndef LOGIN_
#define LOGIN_
#include <windows.h>
#include <iostream>
#include"resource.h"
#include <string>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <my_global.h>
#include <mysql.h>
using namespace std;

class user {
public:
	string ID;
	string pw;
	string name;
	string kg;
	string len;
	string blood;
	string eye1, eye2;
	user() {};
	void setid(string id, string pw){
		this -> ID = id;
		this->pw = pw;
	}
	void setinfo(string ID, string name, string kg, string len, string blood, string eye1, string eye2) {
		this->ID = ID;
		this->name = name;
		this->kg = kg;
		this->len = len;
		this->blood = blood;
		this->eye1 = eye1;
		this->eye2 = eye2;
	}
};

BOOL CALLBACK LoginProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK UserCreateProc(HWND, UINT, WPARAM, LPARAM);

#endif