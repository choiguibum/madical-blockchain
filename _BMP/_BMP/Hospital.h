#ifndef HOSPITAL__
#define HOSPITAL__
#include <windows.h>
#include <my_global.h>
#include <iostream>
#include <vector>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <mysql.h>
#include "commctrl.h"
#include "resource.h"


using namespace std;

class List {
public:
	string name;
	string birth;
	string id;
	string kg;
	string len;
	string e1;
	string e2;
	string bl;

	void reset() {
		this->name = "";
		this->birth = "";
		this->id = "";
		this->kg = "";
		this->len = "";
		this->e1 = "";
		this->e2 = "";
		this->bl = "";
	}
};

LRESULT CALLBACK WndHospitalProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK RecordProc(HWND, UINT, WPARAM, LPARAM);

#endif