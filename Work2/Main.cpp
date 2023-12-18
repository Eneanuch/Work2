#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#include "Main.h"

using namespace std;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS MainClass = NewWindowClass(
		(HBRUSH) COLOR_WINDOW, 
		LoadCursor(NULL, IDC_ARROW),
		hInst, 
		LoadIcon(NULL, IDI_APPLICATION),
		L"MainWNDClass",
		MainProcedure
	);

	if (!RegisterClass(&MainClass)) {
		return -1;
	}

	MSG MainMessage = { 0 };

	CreateWindow(
		L"MainWNDClass",
		L"Курсовая работа",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,   
		CW_USEDEFAULT,          
		CW_USEDEFAULT, WindowWidth, WindowHeight,
		NULL, NULL, NULL, NULL
	);

	while (GetMessage(&MainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&MainMessage);
		DispatchMessage(&MainMessage);
	}

	return 0;
}


WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {

	WNDCLASS NWC = { 0 };

	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}


LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

	switch (msg)
	{
		case WM_COMMAND: 
			
			switch (wp) {
				// Start Menu
				case OnMenuChooseFile:
					LoadFromFile(hEditField);
					break;
				case OnMenuDownloadFile:
					SaveToFileText(hEditField);
					break;
				case OnMenuExit:
					PostQuitMessage(0);
					break;
				case OnMenuGetTranslateTable:
					//MessageBoxA(hWnd, "Some", "some", 0);
					LoadFromFileTable(hEditField);
					break;
				case OnMenuSetTranslateTable:
					SaveToFileTable(hEditField, translator.getTranslationTable());
					break;
				case OnMenuEditTranslateTable:
					CreateTableWindow();
					break;
				// End Menu 

				// Start Buttons
				case OnButtonDown:
					TextEditSetSettings();
					SetWindowTextA(hEditField, (LPSTR) textEdit.textChange(LOWER).c_str());
					break;
				case OnButtonUpper:
					TextEditSetSettings();
					SetWindowTextA(hEditField, (LPSTR) textEdit.textChange(UPPER).c_str());
					break;
				case OnButtonRevert:
					TextEditSetSettings();
					SetWindowTextA(hEditField, (LPSTR) textEdit.textChange(REVERT).c_str());
					break;
				case OnButtonToKir: 
					TextEditSetSettings();
					SetWindowTextA(hEditField, (LPSTR) textEdit.textChange(KIRILLICA).c_str());
					break;
				case OnButtonToLat:
					TextEditSetSettings();
					SetWindowTextA(hEditField, (LPSTR) textEdit.textChange(LATINICA).c_str());
					break;
				case OnButtonPasteAscii:
					TextEditSetSettings();
					SetWindowTextA(hEditField,
						(LPSTR) (GetWindowTextAsString(hEditField) + static_cast<char>(GetWindowTextAsInt(hEditAsciiSymbol))).c_str());
				
				// End Buttons
				default: break;
			}

			break;
		case WM_CREATE:

			MainWndLoadFont(hWnd);
			MainWndAddMenus(hWnd);
			MainWndAddWidgets(hWnd);

			setlocale(LC_CTYPE, "");			

			textEdit = TextEdit();
			translator = Translator();

			textEdit.setTranslator(translator);

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default: 
			return DefWindowProc(hWnd, msg, wp, lp);
			break;
	}

}


HWND CreateWindowPreload(
	LPCSTR lpClassName,  
	LPCSTR lpWindowName,  
	DWORD dwStyle,       
	int x,            
	int y,             
	int nWidth,       
	int nHeight,  
	HWND hWndParent,     
	HMENU hMenu,         
	HINSTANCE hInstance,  
	LPVOID lpParam
) {
	// Установка изначальных настроек для каждого окна 
	HWND newElement = CreateWindowA(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	MainWndLoadFont(newElement);

	return newElement;
}

int GetWindowTextAsInt(HWND textField) {
	string text = GetWindowTextAsString(textField);

	try {
		int result = stoi(text);
		return result;
	}
	catch (const exception& e) {
		MessageBox(NULL, L"Ошибка преобразования в число", L"Ошибка", MB_OK | MB_ICONERROR);
		return 0;
	}
}

string GetWindowTextAsString(HWND hWnd) {
	const int bufferSize = 5096;
	char buffer[bufferSize];

	int length = GetWindowTextA(hWnd, buffer, bufferSize);

	if (length > 0) {
		return string(buffer, length);
	}
	else {
		return string();
	}
}

int getSelectedIndexComboBox(HWND hComboBox) {
	int selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	return selectedIndex;
}

void TextEditSetSettings() {

	textEdit.setFrom(GetWindowTextAsInt(hEditTypeFieldFrom));
	textEdit.setTo(GetWindowTextAsInt(hEditTypeFieldTo));
	textEdit.setType(getSelectedIndexComboBox(hComboBoxType));
	textEdit.setInputText(GetWindowTextAsString(hEditField));

}


void MainWndAddMenus(HWND hWnd) {

	HMENU RootMenu = CreateMenu();

	HMENU FileMenu = CreateMenu();

	AppendMenu(FileMenu, MF_STRING, OnMenuChooseFile, L"Загрузить текстовый файл");
	AppendMenu(FileMenu, MF_STRING, OnMenuDownloadFile, L"Выгрузить текстовый файл");
	AppendMenu(FileMenu, MF_SEPARATOR, NULL, NULL);

	AppendMenu(FileMenu, MF_STRING, OnMenuExit, L"Выход");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR) FileMenu, L"Файл");


	HMENU TranslateMenu = CreateMenu();
	AppendMenu(TranslateMenu, MF_STRING, OnMenuEditTranslateTable, L"Редактировать таблицу перевода");
	AppendMenu(TranslateMenu, MF_STRING, OnMenuGetTranslateTable, L"Загрузить таблицу перевода");
	AppendMenu(TranslateMenu, MF_STRING, OnMenuSetTranslateTable, L"Выгрузить таблицу перевода");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR) TranslateMenu, L"Перевод");


	SetMenu(hWnd, RootMenu);

}


void MainWndLoadFont(HWND hWnd) {
	HFONT hFont = CreateFont(
		-MulDiv(9, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72),
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Segoe UI"
	);

	if (hFont) {
		SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);
	}
}

void MainWndAddWidgets(HWND hWnd) {
	MainWndAddLabels(hWnd);
	MainWndAddButtons(hWnd);
	MainWndAddFields(hWnd);
	MainWndAddOther(hWnd);
}

void MainWndAddButtons(HWND hWnd) {
	CreateWindowPreload(
		"button", "Нижний",
		WS_VISIBLE | WS_CHILD | ES_CENTER,
		WidgetsDest, WidgetsDest,
		WidgetsWidth, WidgetsHeight,
		hWnd, (HMENU) OnButtonDown, NULL, NULL
	);
	CreateWindowPreload("button", "Верхний",
		WS_VISIBLE | WS_CHILD,
		WidgetsDest, WidgetsHeight + WidgetsDest * 2,
		WidgetsWidth, WidgetsHeight,
		hWnd, (HMENU) OnButtonUpper, NULL, NULL
	);
	CreateWindowPreload(
		"button", "Противоположный",
		WS_VISIBLE | WS_CHILD,
		WidgetsDest, WidgetsHeight * 2 + WidgetsDest * 3,
		WidgetsWidth, WidgetsHeight,
		hWnd, (HMENU) OnButtonRevert, NULL, NULL
	);
	CreateWindowPreload("button", "На латиницу",
		WS_VISIBLE | WS_CHILD,
		WidgetsDest, WidgetsHeight * 9 + WidgetsDest * 10,
		WidgetsWidth, WidgetsHeight,
		hWnd, (HMENU)OnButtonToKir, NULL, NULL
	);
	CreateWindowPreload("button", "На кириллицу",
		WS_VISIBLE | WS_CHILD,
		WidgetsDest, WidgetsHeight * 10 + WidgetsDest * 11,
		WidgetsWidth, WidgetsHeight,
		hWnd, (HMENU)OnButtonToLat, NULL, NULL
	);
	CreateWindowPreload("button", "Вставить",
		WS_VISIBLE | WS_CHILD,
		WidgetsDest, WidgetsHeight * 13 + WidgetsDest * 14,
		WidgetsWidth, WidgetsHeight,
		hWnd, (HMENU)OnButtonPasteAscii, NULL, NULL
	);

}

void MainWndAddFields(HWND hWnd) {
	hEditTypeFieldFrom = CreateWindowPreload(
		"edit", "",
		WS_VISIBLE | WS_CHILD | ES_NUMBER,
		WidgetsDest, WidgetsHeight * 4 + WidgetsDest * 4, 
		WidgetsWidth, WidgetsHeight,
		hWnd, NULL, NULL, NULL);
	hEditTypeFieldTo = CreateWindowPreload(
		"edit", "",
		WS_VISIBLE | WS_CHILD | ES_NUMBER,
		WidgetsDest, WidgetsHeight * 6 + 7 * WidgetsDest,
		WidgetsWidth, WidgetsHeight,
		hWnd, NULL, NULL, NULL);

	hEditField = CreateWindowPreload(
		"edit", "",
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL,
		130, 5,
		350, 400,
		hWnd, NULL, NULL, NULL);

	hEditAsciiSymbol = CreateWindowPreload(
		"edit", "",
		WS_VISIBLE | WS_CHILD | ES_NUMBER,
		WidgetsDest, WidgetsHeight * 12 + 13 * WidgetsDest,
		WidgetsWidth, WidgetsHeight,
		hWnd, NULL, NULL, NULL
	);

	SetWindowTextA(hEditTypeFieldFrom, "0");
	SetWindowTextA(hEditTypeFieldTo, "0");

}

void MainWndAddLabels(HWND hWnd) {
	CreateWindowPreload(
		"static", "От:",
		WS_VISIBLE | WS_CHILD,
		WidgetsDest, WidgetsHeight * 3 + 4 * WidgetsDest,
		WidgetsWidth, WidgetsHeight,
		hWnd, NULL, NULL, NULL);
	CreateWindowPreload(
		"static", "До:",
		WS_VISIBLE | WS_CHILD,
		WidgetsDest, WidgetsHeight * 5 + 6 * WidgetsDest,
		WidgetsWidth, WidgetsHeight,
		hWnd, NULL, NULL, NULL);

	CreateWindowPreload(
		"static", "Единица измерения:",
		WS_VISIBLE | WS_CHILD,
		WidgetsDest, WidgetsHeight * 7 + 8 * WidgetsDest,
		WidgetsWidth, WidgetsHeight,
		hWnd, NULL, NULL, NULL);
	CreateWindowPreload(
		"static", "Код символа ASCII:",
		WS_VISIBLE | WS_CHILD,
		WidgetsDest, WidgetsHeight * 11 + 12 * WidgetsDest,
		WidgetsWidth, WidgetsHeight,
		hWnd, NULL, NULL, NULL
	);

}

void MainWndAddOther(HWND hWnd) {
	hComboBoxType = CreateWindowPreload(
		"combobox", NULL,
		CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
		WidgetsDest, WidgetsHeight * 8 + WidgetsDest * 9, 
		WidgetsWidth, WidgetsHeight,
		hWnd, (HMENU) OnComboBoxChange, NULL, NULL);


	SendMessage(hComboBoxType, CB_ADDSTRING, 0, (LPARAM) L"Весь текст");
	SendMessage(hComboBoxType, CB_ADDSTRING, 0, (LPARAM) L"Буквы");
	SendMessage(hComboBoxType, CB_ADDSTRING, 0, (LPARAM) L"Слова");
	SendMessage(hComboBoxType, CB_ADDSTRING, 0, (LPARAM) L"Строка");
	SendMessage(hComboBoxType, CB_ADDSTRING, 0, (LPARAM) L"Предложения");
	SendMessage(hComboBoxType, CB_ADDSTRING, 0, (LPARAM) L"Абзац");

	SendMessage(hComboBoxType, CB_SETCURSEL, 0, 5);
}


void LoadFromFileTable(HWND hEditField) {
	OPENFILENAME ofn = {};
	wchar_t szFile[MAX_PATH] = L"";

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetParent(hEditField);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//cout << "Uploading file" << endl;
	if (GetOpenFileName(&ofn)) {
		unordered_map<string, string> translationTable;

		ifstream file(szFile, ios::binary);
		if (file) {
			string line;
			while (std::getline(file, line)) {
				size_t delimiterPos = line.find(":::");
				if (delimiterPos != string::npos) {
					string key = line.substr(0, delimiterPos);
					string value = line.substr(delimiterPos + 3);
					//MessageBoxA(hEditField, key.c_str(), value.c_str(), 0);
					//cout << key << " " << value << endl;
					translationTable[key] = value;
				}
			}
			translator.setTranslationTable(translationTable);
			textEdit.setTranslator(translator);
			file.close();
		}
	}
}

void SaveToFileTable(HWND hEdit, unordered_map<string, string> translationTable) {
	OPENFILENAME ofn = {};
	wchar_t szFile[MAX_PATH] = L"";

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetParent(hEdit);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn)) {

		ofstream myfile;
		myfile.open(szFile);
		for (const auto& entry : translationTable) {
			myfile << entry.first << ":::" << entry.second << std::endl;
		}
		myfile.close();
	}
}


void SaveToFileText(HWND hEdit) {
	OPENFILENAME ofn = {};
	wchar_t szFile[MAX_PATH] = L"";

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetParent(hEdit);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn)) {
		string text = GetWindowTextAsString(hEditField);

		ofstream myfile;
		myfile.open(szFile);
		myfile << text;
		myfile.close();
	}
}

void LoadFromFile(HWND hEditField) {
	OPENFILENAME ofn = {};
	wchar_t szFile[MAX_PATH] = L"";

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetParent(hEditField);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) {
		std::ifstream file(szFile, std::ios::binary);
		if (file) {
			file.seekg(0, std::ios::end);
			std::streamsize fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			std::vector<char> buffer(fileSize);

			if (file.read(buffer.data(), fileSize)) {
				int size_needed = MultiByteToWideChar(CP_UTF8, 0, buffer.data(), static_cast<int>(buffer.size()), NULL, 0);
				std::wstring wbuffer(size_needed, 0);
				MultiByteToWideChar(CP_UTF8, 0, buffer.data(), static_cast<int>(buffer.size()), &wbuffer[0], size_needed);

				SetWindowTextW(hEditField, wbuffer.c_str());
			}
		}
	}
}