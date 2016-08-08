#pragma once

class PrinterManager
{
public:
	static const int LENGTH_PRINTER_NAME = 128;
	static const size_t SIZE_PRINTER_LIST = 4096;
	static const wchar_t* XPS_PRINTER_NAME;

private:
	wchar_t m_szXpsPrinter[LENGTH_PRINTER_NAME];
	wchar_t m_szOldPrinter[LENGTH_PRINTER_NAME];

public:
	PrinterManager();
	~PrinterManager();

	void SetXpsPrinter(const wchar_t* pXpsPrinterName);
	void MakeXpsPrinterDefault();
	void RestoreDefaultPrinter();
};
