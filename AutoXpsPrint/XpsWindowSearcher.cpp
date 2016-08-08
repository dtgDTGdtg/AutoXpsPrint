#include "stdafx.h"
#include "XpsWindowSearcher.h"


XpsWindowSearcher::XpsWindowSearcher() { }
XpsWindowSearcher::~XpsWindowSearcher() { }

bool XpsWindowSearcher::Search()
{
	m_hSaveDialog = 0;
	m_hFilterDDL = 0;
	m_hTextBox = 0;
	m_hButtonOK = 0;

	EnumWindows(XpsWindowSearcher::EnumWindowsProc, (LPARAM)this);

	if (m_hSaveDialog != 0)
		return true;
	return false;
}

bool XpsWindowSearcher::SaveXpsDocument(const wchar_t* pFilePath)
{
	if (m_hTextBox == 0 || m_hButtonOK == 0) return false;
	if (pFilePath == 0 || *pFilePath == '\0') throw ErrorMessages::ArgumentNULL;

	SendMessage(m_hTextBox, WM_SETTEXT, 0, (LPARAM)pFilePath);
	SendMessage(m_hButtonOK, BM_CLICK, 0, 0);
	
	return true;
}

BOOL XpsWindowSearcher::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	if (lParam == 0) throw ErrorMessages::ArgumentNULL;

	XpsWindowSearcher* pXWS = (XpsWindowSearcher*)lParam;
	if (pXWS->IsWindowXpsSaveDialog(hWnd))
	{
		pXWS->m_hSaveDialog = hWnd;
		return FALSE;
	}
	return TRUE;
}

bool XpsWindowSearcher::IsWindowXpsSaveDialog(HWND hWnd)
{
	wchar_t szClassName[WINDOW_TEXT_LENGTH];

	if (RealGetWindowClass(hWnd, szClassName, WINDOW_TEXT_LENGTH) > 0 && wcscmp(szClassName, L"#32770") == 0)
	{
		if (SearchFilterDDL(hWnd))
		{
			if (SearchTextBox(hWnd))
			{
				if (SearchButtonOK(hWnd))
				{
					m_hSaveDialog = hWnd;
					return true;
				}
			}
		}
	}
	return false;
}

bool XpsWindowSearcher::SearchFilterDDL(HWND hParentHWND)
{
	m_hFilterDDL = 0;
	EnumChildWindows(hParentHWND, EnumChildProcFilterDDL, (LPARAM)this);

	if (m_hFilterDDL > 0)
		return true;
	return false;
}

BOOL XpsWindowSearcher::EnumChildProcFilterDDL(HWND hWnd, LPARAM lParam)
{
	XpsWindowSearcher* pXWS = (XpsWindowSearcher*)lParam;
	wchar_t szClassName[WINDOW_TEXT_LENGTH];

	if (RealGetWindowClass(hWnd, szClassName, WINDOW_TEXT_LENGTH) > 0 && wcscmp(szClassName, L"ComboBox") == 0)
	{
		wchar_t szCaption[WINDOW_TEXT_LENGTH];
		SendMessage(hWnd, WM_GETTEXT, WINDOW_TEXT_LENGTH, (LPARAM)szCaption);

		if (Contains(szCaption, L"(*.XPS)"))
		{
			pXWS->m_hFilterDDL = hWnd;
			return FALSE;
		}
	}
	else if (pXWS->SearchFilterDDL(hWnd)) return FALSE;

	return TRUE;
}

bool XpsWindowSearcher::SearchTextBox(HWND hParentHWND)
{
	m_hTextBox = 0;
	EnumChildWindows(hParentHWND, EnumChildProcTextBox, (LPARAM)this);

	if (m_hTextBox > 0)
		return true;
	return false;
}

BOOL XpsWindowSearcher::EnumChildProcTextBox(HWND hWnd, LPARAM lParam)
{
	XpsWindowSearcher* pXWS = (XpsWindowSearcher*)lParam;
	wchar_t szClassName[WINDOW_TEXT_LENGTH];

	if (RealGetWindowClass(hWnd, szClassName, WINDOW_TEXT_LENGTH) > 0 && wcscmp(szClassName, L"Edit") == 0)
	{
		wchar_t szCaption[WINDOW_TEXT_LENGTH];
		SendMessage(hWnd, WM_GETTEXT, WINDOW_TEXT_LENGTH, (LPARAM)szCaption);

		if (Contains(szCaption, L"*.XPS"))
		{
			pXWS->m_hTextBox = hWnd;
			return FALSE;
		}
	}
	else if (pXWS->SearchTextBox(hWnd)) return FALSE;

	return TRUE;
}

bool XpsWindowSearcher::SearchButtonOK(HWND hParentHWND)
{
	m_hButtonOK = 0;
	EnumChildWindows(hParentHWND, EnumChildProcButtonOK, (LPARAM)this);

	if (m_hButtonOK > 0)
		return true;
	return false;
}

BOOL XpsWindowSearcher::EnumChildProcButtonOK(HWND hWnd, LPARAM lParam)
{
	XpsWindowSearcher* pXWS = (XpsWindowSearcher*)lParam;
	wchar_t szClassName[WINDOW_TEXT_LENGTH];

	if (RealGetWindowClass(hWnd, szClassName, WINDOW_TEXT_LENGTH) > 0 && wcscmp(szClassName, L"Button") == 0)
	{
		if (GetDlgCtrlID(hWnd) == IDOK)
		{
			pXWS->m_hButtonOK = hWnd;
			return FALSE;
		}
	}
	else if (pXWS->SearchButtonOK(hWnd)) return FALSE;

	return TRUE;
}

bool XpsWindowSearcher::Contains(wchar_t* sourceString, wchar_t* stringToLookFor)
{
	int len1 = wcslen(sourceString);
	int len2 = wcslen(stringToLookFor);

	if (len1 < 1 || len2 < 1 || len1 < len2) return false;

	_wcsupr_s(sourceString, len1 + 1);

	for (int i = 0; i < len1; i++)
	{
		if (sourceString[i] == stringToLookFor[0])
		{
			if (StringsEqual(sourceString + i, stringToLookFor, len2))
				return true;
		}
	}
	return false;
}

bool XpsWindowSearcher::StringsEqual(wchar_t* string1, wchar_t* string2, int length)
{
	for (int i = 0; i < length; i++)
		if (string1[i] != string2[i])
			return false;
	return true;
}
