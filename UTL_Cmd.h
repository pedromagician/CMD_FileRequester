#pragma once

#include "stdafx.h"

class UTL_Cmd
{
public:
	static int ParseCommandLinbe(int argc, _TCHAR* argv[], int& iCorrectParameters, bool& bHelp, wstring& title, bool &bOpen, bool& bSave, wstring& filename, wstring& okLabel, wstring& path, bool& bDrawersOnly, wstring& filter);

	static bool IsHelp(wstring comm);
	static bool IsTitle(wstring comm);
	static bool IsOpen(wstring comm);
	static bool IsSave(wstring comm);
	static bool IsFileName(wstring comm);
	static bool IsOkLabel(wstring comm);
	static bool IsPath(wstring comm);
	static bool IsDrawersOnly(wstring comm);
	static bool IsFilter(wstring comm);

	static void Help();
};
