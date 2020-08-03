#include "GUI.h"
#include <sstream>
#include <iostream>
#include <string>

extern Command C;
Command C;
extern FileManager F;
FileManager F;

int flag = 0;
int limit = 0;


/**
 * @brief	Define registered classes
 * @param	HINSTANCE hInstance
 * @return	egisterClassExW(&wcex)
 */
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}



/**
 * @brief	Save the instance handle and create the main window. In this 
 *          function, we save the instance handle in the global variable 
 *          and create and display the main program window.
 * @param	HINSTANCE hInstance
 * @param	int nCmdShow
 * @return	Boolean, whether the execution was successful.
 */
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store the instance handle in a global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}



/**
 * @brief	Process messages for the main window.
 *          WM_ Command - process application menu
 *          WM_ Paint - draws the main window
 *          WM_ Destroy - send exit message and return
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hTextBoxWnd;
	switch (message)
	{
	case WM_CREATE: {
		_RegisterTextBoxClass();    // The class that registers the text box
		hTextBoxWnd = _CreateTextBoxWindow(hWnd); // Create a text box
	} break;

	case WM_ACTIVATE:                // When the window is activated, set 
		                             // the focus to the text box
		::SetFocus(hTextBoxWnd);
		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Analysis menu selection:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/**
 * @brief	Output the text.
 */
void _DrawText(HDC hDC)
{
	int len = _tcslen(_String);
	int tmp = 0;
	for (int i = max(curLine-limit+1, 1);i <= curLine;i++) {
		int curLen = _tcslen(_HisString[i]);
		TextOut(hDC, 4, 2 + tmp * TEXTBOX_HEIGHT, _HisString[i], curLen);
		++tmp;
	}
	TextOut(hDC, 4, 2 + tmp * TEXTBOX_HEIGHT, _String, len);
	return;
}

/**
 * @brief	Set the position of the cursor.
 */
void _SetCaretPos(HWND hWnd) {
	HDC hDC = ::GetDC(hWnd);
	SIZE size;
	GetTextExtentPoint(hDC, _String, _StringPosition, &size);
	SetCaretPos(4 + size.cx, 3 + curLine * TEXTBOX_HEIGHT);
	ReleaseDC(hWnd, hDC);
}


/**
 * @brief	Message handler for the about box.
 */
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


/**
 * @brief	Corresponding key.
 */
LRESULT CALLBACK _TextBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		case WM_PAINT: {
			static PAINTSTRUCT ps;
			static RECT rect;
			HDC hDC = ::BeginPaint(hWnd, &ps);  // Start drawing operation
			::GetClientRect(hWnd, &rect);        // Get the size of the customer area
			//::DrawEdge(hDC, &rect, EDGE_SUNKEN, BF_RECT);  // Draw border, edge_Sunken means that the drawing style is embedded style, 
															 //BF_ Rect means to draw a rectangular border
			_DrawText(hDC);                      // Draw text
			::EndPaint(hWnd, &ps);               // Ends the drawing operation
		}
		break;


		case WM_SETFOCUS: {    // Get focus
			::CreateCaret(hWnd, (HBITMAP)NULL, 1, TEXTBOX_HEIGHT - 5);     // create a cursor
			_SetCaretPos(hWnd);                            // Set cursor position
			::ShowCaret(hWnd);                   // Show cursor
		}
		break;

		case WM_KILLFOCUS: // Lose focus
			::HideCaret(hWnd);                   // hide cursor
			::DestroyCaret();                    // Destroy cursor
		break;

		case WM_SETCURSOR: {  // Set cursor shape
			static HCURSOR hCursor = ::LoadCursor(NULL, IDC_IBEAM);
			::SetCursor(hCursor);
		}
		break;

		case WM_CHAR: {    // Character message
			TCHAR code = (TCHAR)wParam;
			int len = ::_tcslen(_String);
			if (code < (TCHAR)' ' || len >= TEXTBOX_MAXLENGTH)
				return 0;
			::MoveMemory(_String + _StringPosition + 1, _String + _StringPosition, (len - _StringPosition + 1) * sizeof(TCHAR));
			_String[_StringPosition++] = code;
			_UpdateWindow(hWnd);
			_SetCaretPos(hWnd);
		}
		break;

		case WM_KEYDOWN: {  // Key press message
			TCHAR code = (TCHAR)wParam;
			switch (code)
			{
			case VK_RETURN: {
				int len = _tcslen(_String);
				_HisString[++curLine] = (TCHAR*)malloc((len + 1) * sizeof(TCHAR));
				memset(_HisString[curLine], 0, (len + 1) * sizeof(TCHAR));
				for (int i = 0;i <= _StringPosition;i++) _HisString[curLine][i] = _String[i];

			
				std::string tmp = "", ret = "";
				Matrix matret;
				bool flag = 0;

				for (int i = 4;i <= _StringPosition;i++) tmp = tmp+(char)_String[i];
				try {
					F.Command(tmp, matret);
				}
				catch (std::string errMsg) {
					tmp = errMsg;
					flag = 1;
				}

				if (!flag) {
					int num_size = matret.GetSize();
					int num_row = matret.GetRow();
					int num_col = matret.GetCol();

					std::stringstream sstream;
					for (int i = 0;i < num_size;i++) {
						sstream << std::fixed << std::setprecision(6) << matret.GetData(i) << " ";

						if ((i + 1) % matret.GetCol() == 0) {
							tmp = sstream.str();
							len = (int)tmp.length();
							_HisString[++curLine] = (TCHAR*)malloc((len + 1) * sizeof(TCHAR));
							memset(_HisString[curLine], 0, (len + 1) * sizeof(TCHAR));
							for (int j = 0;j < len;j++) _HisString[curLine][j] = tmp[j];
							sstream.str("");
						}
					}
				}
				else {
					len = (int)tmp.length();
					_HisString[++curLine] = (TCHAR*)malloc((len + 1) * sizeof(TCHAR));
					memset(_HisString[curLine], 0, (len + 1) * sizeof(TCHAR));
					for (int j = 0;j < len;j++) _HisString[curLine][j] = tmp[j];
				}



			
				memset(_String, 0, sizeof(_String));
				_StringPosition = 0;
				for (int i = 1;i <= 3;i++)
					_String[_StringPosition++] = '>';
				_String[_StringPosition++] = ' ';


				_UpdateWindow(hWnd);
				_SetCaretPos(hWnd);
			}
							break;

			case VK_LEFT: // Left cursor key
				if (_StringPosition > 0) _StringPosition--;
				break;

			case VK_RIGHT:     // Right cursor key
				if (_StringPosition < (int)::_tcslen(_String)) _StringPosition++;
				break;

			case VK_HOME: // HOME
				_StringPosition = 0;
				break;

			case VK_END:  // END
				_StringPosition = ::_tcslen(_String);
				break;

			case VK_BACK: // Backspace key
				if (_StringPosition > 0) {
					::MoveMemory(_String + _StringPosition - 1, _String + _StringPosition, (::_tcslen(_String) - _StringPosition + 1) * sizeof(TCHAR));
					_StringPosition--;
					_UpdateWindow(hWnd);
				}
				break;

			case VK_DELETE: {  // Delete key
				int len = ::_tcslen(_String);
				if (_StringPosition < len) {
					::MoveMemory(_String + _StringPosition, _String + _StringPosition + 1, (::_tcslen(_String) - _StringPosition + 1) * sizeof(TCHAR));
					_UpdateWindow(hWnd);
				}
			}
							break;
			}
			_SetCaretPos(hWnd);
		} break;

		case WM_LBUTTONDOWN: {  // Click the mouse to set the cursor position
			int x = LOWORD(lParam);
			HDC hDc = ::GetDC(hWnd);
			int strLen = ::_tcslen(_String), strPos = 0;
			SIZE size;
			for (strPos = 0; strPos < strLen; strPos++) {
				::GetTextExtentPoint(hDc, _String, strPos, &size);
				if (size.cx + 4 >= x) break;
			}
			_StringPosition = strPos;
			::GetTextExtentPoint(hDc, _String, strPos, &size);
			::SetCaretPos(size.cx + 4, 3);
			::ReleaseDC(hWnd, hDc);
		} break;

		default:
			return ::DefWindowProc(hWnd, message, wParam, lParam);
	}
	return (LRESULT)0;
}


/**
 * @brief	RegisterTextBoxClass
 */
ATOM  _RegisterTextBoxClass()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;   // Specifies that the window is redrawn when
													   // the window size changes, and responds to a 
													   // mouse double-click event.
	wc.hInstance = hInst;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);     // Specify the window background color as the system 
												       // color "window background"
	wc.lpszClassName = TEXT("TEXTBOX");                // Specifies the name of the window class to be registered,
											           // which is used as an identifier when creating a window
	wc.lpfnWndProc = _TextBoxWndProc;                  // Functions for handling window messages
	return ::RegisterClassEx(&wc);                     // Call API function to register text box window
}


/**
 * @brief	CreateTextBoxWindow
 */
HWND _CreateTextBoxWindow(HWND hParentWnd)
{
	RECT parentWndRect;
	::GetClientRect(hParentWnd, &parentWndRect);       // Gets the location of the client area of the parent window
	int left = 0, top = 0;

	// Create a text box
	HWND hWnd = CreateWindowW(TEXT("TEXTBOX"), NULL, WS_CHILDWINDOW | WS_VISIBLE,
		left, top, parentWndRect.right, parentWndRect.bottom,
		hParentWnd, NULL, hInst, NULL);

	for (int i = 1;i <= 3;i++)
		_String[_StringPosition++] = '>';
	_String[_StringPosition++] = ' ';

	return hWnd;
}

/**
 * @brief	UpdateWindow
 */
void _UpdateWindow(HWND hWnd)
{
	RECT rect;

	GetWindowRect(hWnd, &rect);
	limit = abs(rect.bottom - rect.top) / TEXTBOX_HEIGHT - 1;
	GetClientRect(hWnd, &rect);
	InvalidateRect(hWnd, &rect, TRUE);
	UpdateWindow(hWnd);
	return;
}