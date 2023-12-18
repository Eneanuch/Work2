#pragma once

using namespace std;

#include "TextEdit.h"
#include <Windows.h>
#include <CommCtrl.h>


extern HWND g_mainWindow;
extern HWND g_tableWindow;
extern HWND g_listView;

extern TextEdit textEdit;
extern Translator translator;

void CreateTableWindow();
void CreateListView(HWND hwnd);
void SaveChanges();

LRESULT CALLBACK TableWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LPWSTR CharArrayToLPWSTR(const char* charArray);
