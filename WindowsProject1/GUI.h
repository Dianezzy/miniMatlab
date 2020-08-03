#pragma once
#include "framework.h"
#include "resource.h"
#include "numcalc.h"
#include "expr.h"
#include "filemanager.h"

#define TEXTBOX_WIDTH       300      // Text box width
#define TEXTBOX_HEIGHT      20       // Text box Height
#define MAX_LOADSTRING 100
#define TEXTBOX_MAXLENGTH   100000 // The maximum length of text in a text box
#define TEXTBOX_MAXLINE 100

extern HINSTANCE hInst;                                // Current instance
extern WCHAR szTitle[MAX_LOADSTRING];                  // Title bar text
extern WCHAR szWindowClass[MAX_LOADSTRING];            // Main window class name
extern TCHAR _String[TEXTBOX_MAXLENGTH + 10];
extern TCHAR *_HisString[TEXTBOX_MAXLINE + 10];
extern int    _StringPosition;
extern int curLine;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

ATOM  _RegisterTextBoxClass();
HWND _CreateTextBoxWindow(HWND hParentWnd);
void _UpdateWindow(HWND hWnd);