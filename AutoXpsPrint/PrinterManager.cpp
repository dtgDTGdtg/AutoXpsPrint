#include "stdafx.h"
#include "PrinterManager.h"
#include "ErrorMessages.h"


const wchar_t* PrinterManager::XPS_PRINTER_NAME = L"Microsoft XPS Document Writer";


PrinterManager::PrinterManager()
{
	ZeroMemory(m_szXpsPrinter, sizeof(m_szXpsPrinter));
	ZeroMemory(m_szOldPrinter, sizeof(m_szOldPrinter));
}

PrinterManager::~PrinterManager()
{
	RestoreDefaultPrinter();
}

void PrinterManager::SetXpsPrinter(const wchar_t* pXpsPrinterName)
{
	const wchar_t* pPrinterToSearch;

	if (pXpsPrinterName == nullptr || *pXpsPrinterName == '\0')
		pPrinterToSearch = PrinterManager::XPS_PRINTER_NAME;
	else
		pPrinterToSearch = pXpsPrinterName;

	BYTE pPrinterList[SIZE_PRINTER_LIST];
	DWORD dwRequiredSize;
	DWORD dwCount;

	if (!EnumPrinters(PRINTER_ENUM_LOCAL, nullptr, 4, pPrinterList, SIZE_PRINTER_LIST, &dwRequiredSize, &dwCount))
		throw ErrorMessages::FAIL_EnumPrinters;

	for (int i = 0; i < (int)dwCount; i++)
	{
		PRINTER_INFO_4* pPrinterInfo = (PRINTER_INFO_4*)pPrinterList + i;

		if (_wcsicmp(pPrinterInfo->pPrinterName, pPrinterToSearch) == 0)
		{
			wcscpy_s(m_szXpsPrinter, pPrinterInfo->pPrinterName);
			return;
		}
	}
	throw ErrorMessages::XpsPrinterNotFound;
}

void PrinterManager::MakeXpsPrinterDefault()
{
	if (m_szXpsPrinter[0] == '\0') throw ErrorMessages::XpsPrinterIsNotSet;

	if (m_szOldPrinter[0] == '\0')
	{
		DWORD dwSize = PrinterManager::LENGTH_PRINTER_NAME;
		if (!GetDefaultPrinter(m_szOldPrinter, &dwSize)) throw ErrorMessages::FAIL_GetDefaultPrinter;
	}

	if (wcscmp(m_szOldPrinter, m_szXpsPrinter) != 0)
		if (!SetDefaultPrinter(m_szXpsPrinter)) throw ErrorMessages::FAIL_SetDefaultPrinter;
}

void PrinterManager::RestoreDefaultPrinter()
{
	if (m_szOldPrinter[0] == '\0') return;

	SetDefaultPrinter(m_szOldPrinter);
}
