#pragma once

class XpsWindowSearcher
{
private:
	static const int WINDOW_TEXT_LENGTH = 64;

public:
	HWND m_hSaveDialog;
	HWND m_hFilterDDL;
	HWND m_hTextBox;
	HWND m_hButtonOK;

	XpsWindowSearcher();
	~XpsWindowSearcher();

	bool Search();
	bool SaveXpsDocument(const wchar_t* pFilePath);

private:
	bool IsWindowXpsSaveDialog(HWND hWnd);
	bool SearchFilterDDL(HWND hParentHWND);
	bool SearchTextBox(HWND hParentHWND);
	bool SearchButtonOK(HWND hParentHWND);

	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK EnumChildProcFilterDDL(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK EnumChildProcTextBox(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK EnumChildProcButtonOK(HWND hWnd, LPARAM lParam);
	static bool StringsEqual(wchar_t* string1, wchar_t* string2, int length);
	static bool Contains(wchar_t* sourceString, wchar_t* stringToLookFor);
};
