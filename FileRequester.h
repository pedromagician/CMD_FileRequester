#pragma once

class FileRequester
{
public:
	static bool Requester(bool _save, bool _open, bool _directory, bool _drawersOnly, const wstring& _title, const wstring& _path, const wstring& _filename, const wstring& _okLabel, wstring _filter, wstring& _result);
};
