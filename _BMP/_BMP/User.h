#ifndef USER__
#define USER__
#include <windows.h>
#include <my_global.h>
#include <mysql.h>
#include <time.h>
#include "commctrl.h"
#include "resource.h"

LRESULT CALLBACK WndUserProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK PhysicalProc(HWND, UINT, WPARAM, LPARAM);

#endif