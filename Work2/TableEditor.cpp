#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "TableEditor.h"

using namespace std;

HWND g_tableWindow;
HWND g_listView;
vector<HWND> g_editControls;

TextEdit textEdit;
Translator translator;

int g_scrollPosition = 0;  // Текущая позиция скролла

string GetWindowTextAsString2(HWND hWnd) {
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

LPWSTR CharArrayToLPWSTR(const char* charArray) {
    int bufferSize = MultiByteToWideChar(CP_ACP, 0, charArray, -1, nullptr, 0);
    if (bufferSize > 0) {
        LPWSTR result = new WCHAR[bufferSize];
        MultiByteToWideChar(CP_ACP, 0, charArray, -1, result, bufferSize);
        return result;
    }
    return nullptr;
}

void CreateTableWindow() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = TableWndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"TableWindowClass";
    RegisterClass(&wc);

    g_tableWindow = CreateWindow(
        L"TableWindowClass",
        L"Редактирование таблицы перевода",
        WS_OVERLAPPEDWINDOW | WS_VSCROLL,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, (70 + textEdit.getTranslator().getTranslationTable().size() * 20),
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    ShowWindow(g_tableWindow, SW_SHOWNORMAL);
}

void MainWndLoadFont2(HWND hWnd) {
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
HWND CreateWindowPreload2(
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
    MainWndLoadFont2(newElement);

    return newElement;
}


void CreateListView(HWND hwnd) {
    int yPos = 10;

    for (const auto& pair : textEdit.getTranslator().getTranslationTable()) {
        HWND hEditKey = CreateWindowPreload2(
            "Edit", pair.first.c_str(),
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            10, yPos, 100, 20,
            hwnd, nullptr, GetModuleHandle(nullptr), nullptr
        );
        g_editControls.push_back(hEditKey);

        HWND hEditValue = CreateWindowPreload2(
            "Edit",pair.second.c_str(),
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            120, yPos, 100, 20,
            hwnd, nullptr, GetModuleHandle(nullptr), nullptr
        );
        g_editControls.push_back(hEditValue);

        yPos += 20;
    }

    CreateWindowPreload2(
        "button", "Сохранить",
        WS_VISIBLE | WS_CHILD | ES_CENTER,
        WidgetsDest, yPos,
        WidgetsWidth, WidgetsHeight,
        hwnd, (HMENU) OnButtonSave, NULL, NULL
    );
}

void SaveChanges() {
    int index = 0;
    unordered_map<string, string> translationTable = textEdit.getTranslator().getTranslationTable();
    for (auto& pair : textEdit.getTranslator().getTranslationTable()) {
        translationTable[GetWindowTextAsString2(g_editControls[index])] = GetWindowTextAsString2(g_editControls[index + 1]);
        index += 2;
    }
    Translator translator2 = Translator();
    translator2.setTranslationTable(translationTable);
    textEdit.setTranslator(translator2);
    MessageBoxA(g_tableWindow, "Saved", "Saved", 0);
}


LRESULT CALLBACK TableWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        switch (wp) {
        case OnButtonSave:
            SaveChanges();
            break;
        }
         break;
    case WM_CREATE:
        CreateListView(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lParam);
    }
    return 0;
}