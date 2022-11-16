#include "stdafx.h"
#include "FileRequester.h"

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
	BROWSEINFO browserInfo = { 0 };
	browserInfo.lpszTitle = _title.c_str();
	browserInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	browserInfo.lpfn = FileRequester::BrowseCallbackProc;
	browserInfo.lParam = (LPARAM)_init_path.c_str();

	LPITEMIDLIST pidl = SHBrowseForFolder(&browserInfo);

	if (pidl == 0)
		return false;

	TCHAR path[MAX_PATH];
	SHGetPathFromIDList(pidl, path);

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

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr)) {
		wcout << _T("Error - problem with COM library") << endl;
		return false;
	}

	IFileDialog* pRequester = nullptr;
	//IFileOpenDialog* pRequester;
	//IFileSaveDialog* pRequester;
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

	IShellItem* folder;
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
			COMDLG_FILTERSPEC* pFilter = DBG_NEW COMDLG_FILTERSPEC[vresult.size()];
			int counter = 0;
			for (unsigned int i = 0; i < vresult.size(); i++) {
				pFilter[counter].pszName = &vresult[i][0];
				i++;
				if (i < vresult.size())
					pFilter[counter].pszSpec = &vresult[i][0];
				counter++;
			}

			pRequester->SetFileTypes(vresult.size() / 2, pFilter);
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
		IShellItem* pItem;
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

	pRequester->Release();
	folder->Release();
	CoUninitialize();
	return true;
}
