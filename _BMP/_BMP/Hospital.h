#ifndef HOSPITAL__
#define HOSPITAL__
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <my_global.h>
#include <iostream>
#include <vector>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <string.h>
#include <mysql.h>

#include <stdio.h>
#include <stdlib.h>
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
class Record {
public :
	string dname;
	string time;
	string object;
	string symptol;
	string id;
	string record;
	void reset() {
		this->dname = "";
		this->time = "";
		this->object = "";
		this->symptol = "";
		this->id = "";
		this->record = "";
	}
	void set(string dname, string time, string object, string symptol, string id, string record) {
		this->dname = dname;
		this->time = time;
		this->object = object;
		this->symptol = symptol;
		this->id = id;
		this->record = record;
	}
};

LRESULT CALLBACK WndHospitalProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK RecordProc(HWND, UINT, WPARAM, LPARAM);


#endif