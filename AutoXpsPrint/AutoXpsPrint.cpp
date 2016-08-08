// AutoXpsPrint.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PrinterManager.h"
#include "XpsWindowSearcher.h"


PrinterManager printerManager;

extern "C"
{
	BOOL __stdcall SetXpsPrinter(const wchar_t* pXpsPrinterName, wchar_t* pOutError, size_t nErrBufSize);
	BOOL __stdcall ActivateXpsPrinter(wchar_t* pOutError, size_t nErrBufSize);
	BOOL __stdcall RestoreOldPrinter(wchar_t* pOutError, size_t nErrBufSize);
	BOOL __stdcall ConfirmXpsSaveDialog(const wchar_t* pFilePath, wchar_t* pOutError, size_t nErrBufSize);
}

BOOL __stdcall SetXpsPrinter(const wchar_t* pXpsPrinterName, wchar_t* pOutError, size_t nErrBufSize)
{
	try
	{
		printerManager.SetXpsPrinter(pXpsPrinterName);
		*pOutError = '\0';
		return TRUE;
	}
	catch (const wchar_t* pErrMsg)
	{
		wcscpy_s(pOutError, nErrBufSize, pErrMsg);
	}
	return FALSE;
}

BOOL __stdcall ActivateXpsPrinter(wchar_t* pOutError, size_t nErrBufSize)
{
	try
	{
		printerManager.MakeXpsPrinterDefault();
		*pOutError = '\0';
		return TRUE;
	}
	catch (const wchar_t* pErrMsg)
	{
		wcscpy_s(pOutError, nErrBufSize, pErrMsg);
	}
	return FALSE;
}

BOOL __stdcall RestoreOldPrinter(wchar_t* pOutError, size_t nErrBufSize)
{
	try
	{
		printerManager.RestoreDefaultPrinter();
		*pOutError = '\0';
		return TRUE;
	}
	catch (const wchar_t* pErrMsg)
	{
		wcscpy_s(pOutError, nErrBufSize, pErrMsg);
	}
	return FALSE;
}

BOOL __stdcall ConfirmXpsSaveDialog(const wchar_t* pFilePath, wchar_t* pOutError, size_t nErrBufSize)
{
	try
	{
		XpsWindowSearcher xws;
		if (xws.Search() && xws.SaveXpsDocument(pFilePath))
		{
			*pOutError = '\0';
			return TRUE;
		}
	}
	catch (const wchar_t* pErrMsg)
	{
		wcscpy_s(pOutError, nErrBufSize, pErrMsg);
	}
	return FALSE;
}
