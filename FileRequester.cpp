#include "stdafx.h"
#include "FileRequester.h"

class ComScope
{
public:
	ComScope() : m_hr(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))
	{
	}

	~ComScope()
	{
		if (SUCCEEDED(m_hr)) {
			CoUninitialize();
		}
	}

	HRESULT Result() const { return m_hr; }

private:
	HRESULT m_hr;
};

int CALLBACK FileRequester::BrowseCallbackProc(HWND _hwnd, UINT _uMsg, LPARAM _lParam, LPARAM _lpData)
{
	UNREFERENCED_PARAMETER(_lParam);

	if (_uMsg == BFFM_INITIALIZED)
	{
		SendMessage(_hwnd, BFFM_SETSELECTION, TRUE, _lpData);
	}

	return 0;
}

bool FileRequester::BrowseFolder(const wstring& _init_path, const wstring& _title, wstring& _result)
{
	BROWSEINFO browserInfo;
	memset(&browserInfo, 0, sizeof(browserInfo));

	browserInfo.lpszTitle = _title.c_str();
	browserInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	browserInfo.lpfn = FileRequester::BrowseCallbackProc;
	browserInfo.lParam = (LPARAM)_init_path.c_str();

	LPITEMIDLIST pidl = SHBrowseForFolder(&browserInfo);

	if (pidl == 0)
		return false;

	TCHAR path[MAX_PATH];
	if (!SHGetPathFromIDList(pidl, path))
		return false;

	IMalloc* imalloc = 0;
	if (SUCCEEDED(SHGetMalloc(&imalloc))) {
		imalloc->Free(pidl);
		imalloc->Release();
	}

	_result = path;
	return true;
}

bool FileRequester::Requester(bool _save, bool _open, bool _directory, bool _drawersOnly, const wstring& _title, const wstring& _path, const wstring& _filename, const wstring& _okLabel, wstring _filter, wstring& _result)
{
	int mode = 0;
	if (_open) mode++;
	if (_save) mode++;
	if (_directory) mode++;

	if (mode == 0) {
		mode++;
		_open = true;
	}

	if (mode > 1) {
		wcout << _T("Error - open, save or directory?") << endl;
		return false;
	}

	if (_directory) {
		FileRequester::BrowseFolder(_path, _title, _result);
		return true;
	}

	ComScope com;
	if (FAILED(com.Result())) {
		wcout << _T("Error - problem with COM library") << endl;
		return false;
	}

	IFileDialog* pRequester = nullptr;
	HRESULT hr;

	if (_open)
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pRequester));
	else
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pRequester));
	if (FAILED(hr)) {
		wcout << _T("Error - problem with creating object") << endl;
		return false;
	}

	pRequester->SetTitle(_title.c_str());
	pRequester->SetFileName(_filename.c_str());

	if (_okLabel.empty() == false)
		pRequester->SetOkButtonLabel(_okLabel.c_str());

	IShellItem* folder = nullptr;
	if (_path.empty())
		hr = SHCreateItemInKnownFolder(FOLDERID_Documents, 0, NULL, IID_PPV_ARGS(&folder));
	else
		hr = SHCreateItemFromParsingName(_path.c_str(), NULL, IID_PPV_ARGS(&folder));
	if (FAILED(hr))
		wcout << _T("Error - problem with shell item object") << endl;
	else
		pRequester->SetFolder(folder);

	if (_drawersOnly) {
		FILEOPENDIALOGOPTIONS ops = FOS_PICKFOLDERS;
		pRequester->SetOptions(ops);
	}

	if (_filter.empty() == false) {
		vector<wstring>vresult;
		while (_filter.size()) {
			int index = _filter.find(_T("|"));
			if (index != string::npos) {
				vresult.push_back(_filter.substr(0, index));
				_filter = _filter.substr(index + 1);
				if (_filter.size() == 0) vresult.push_back(_filter);
			}
			else {
				vresult.push_back(_filter);
				_filter = _T("");
			}
		}

		if (vresult.size() % 2 == 0) {
			std::vector<COMDLG_FILTERSPEC> filters;
			filters.reserve(vresult.size() / 2);

			for (size_t i = 0; i < vresult.size(); i += 2) {
				COMDLG_FILTERSPEC spec;
				spec.pszName = vresult[i].c_str();
				spec.pszSpec = vresult[i + 1].c_str();
				filters.push_back(spec);
			}

			pRequester->SetFileTypes(filters.size(), filters.data());
			pRequester->SetFileTypeIndex(1);
		}
		else {
			wcout << _T("Error - bad filter") << endl;
		}
	}

	_result = _T("");

	POINT mouse;
	::GetCursorPos(&mouse);

	hr = pRequester->Show(WindowFromPoint(mouse));

	if (SUCCEEDED(hr)) {
		IShellItem* pItem = nullptr;
		hr = pRequester->GetResult(&pItem);
		if (SUCCEEDED(hr)) {
			PWSTR pszFilePath;
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

			if (SUCCEEDED(hr)) {
				_result = pszFilePath;
				CoTaskMemFree(pszFilePath);
			}
			pItem->Release();
		}
		else {
			wcout << _T("Error - problem with dialog") << endl;
			return false;
		}
	}

	if (pRequester)
		pRequester->Release();
	folder->Release();
	return true;
}
