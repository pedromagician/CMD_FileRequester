#pragma once

class FileRequester
{
public:
	static int CALLBACK BrowseCallbackProc(HWND _hwnd, UINT _uMsg, LPARAM _lParam, LPARAM _lpData);
	static bool BrowseFolder(const wstring& _init_path, const wstring& _title, wstring& _result);

	static bool Requester(bool _save, bool _open, bool _directory, bool _drawersOnly, const wstring& _title, const wstring& _path, const wstring& _filename, const wstring& _okLabel, wstring _filter, wstring& _result);
};
