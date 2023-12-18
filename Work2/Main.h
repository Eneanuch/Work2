#pragma once

#include <iostream>
#include <Windows.h>

#include <string>
#include <tchar.h>
#include <fstream>

#include "Definitions.h"
#include "TextEdit.h"
#include "Translator.h"
#include "TableEditor.h"

HWND hEditField;
HWND hEditTypeFieldFrom;
HWND hEditTypeFieldTo;
HWND hEditTypeFieldOnly;
HWND hEditAsciiSymbol;

HWND hComboBoxType;
HWND hComboBoxTranslate;

OPENFILENAME ofn;
TCHAR szFile[MAX_PATH];

LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

int comboBoxSelectedIndex;

string GetWindowTextAsString(HWND hWnd);

int GetWindowTextAsInt(HWND hWnd);

void MainWndAddMenus(HWND hWnd);

void MainWndAddWidgets(HWND hWnd);

void MainWndAddButtons(HWND hWnd);

void MainWndAddFields(HWND hWnd);

void MainWndAddOther(HWND hWnd);

void MainWndLoadFont(HWND hWnd);

void MainWndLoadFont(HWND hWnd);

void MainWndAddLabels(HWND hWnd);

void TextEditSetSettings();

void SaveToFileTable(HWND hEdit, unordered_map<string, string> translationTable);
void LoadFromFileTable(HWND hEdit);
void SaveToFileText(HWND hEdit);

void LoadFromFile(HWND hEditField);

// Конец прототипов


