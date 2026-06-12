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

bool FileRequester::Requester(bool _save, bool _open, bool _directory, bool _drawersOnly, const wstring& _title, const wstring& _path, const wstring& _filename, const wstring& _okLabel, wstring _filter, wstring& _result)
{
	if (!_open && !_save && !_directory) {
		_open = true;
	}

	bool pickFolders = _directory || _drawersOnly;
	if (_save && pickFolders) {
		wcout << _T("Error - cannot use save mode with directory(drawersonly/directory) selection") << endl;
		return false;
	}

	if (_open && _save) {
		wcout << _T("Error - parameters(open/save) were mixed up") << endl;
		return false;
	}

	ComScope com;
	if (FAILED(com.Result())) {
		wcout << _T("Error - problem with COM library") << endl;
		return false;
	}

	IFileDialog* pRequester = nullptr;
	HRESULT hr = E_FAIL;

	if (_save)
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pRequester));
	else
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pRequester));

	if (FAILED(hr) || !pRequester) {
		wcout << _T("Error - problem with creating object") << endl;
		if (pRequester) pRequester->Release();
		return false;
	}

	pRequester->SetTitle(_title.c_str());
	pRequester->SetFileName(_filename.c_str());

	if (_okLabel.empty() == false)
		pRequester->SetOkButtonLabel(_okLabel.c_str());

	IShellItem* folder = nullptr;
	hr = E_FAIL;

	if (!_path.empty()) {
		wchar_t fullPath[MAX_PATH];
		DWORD len = GetFullPathNameW(_path.c_str(), MAX_PATH, fullPath, nullptr);

		if (len > 0 && len < MAX_PATH) {
			DWORD attr = GetFileAttributesW(fullPath);
			if (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
				PathRemoveFileSpecW(fullPath);
			}

			hr = SHCreateItemFromParsingName(fullPath, NULL, IID_PPV_ARGS(&folder));
		}
	}

	if (FAILED(hr) || !folder)
		hr = SHCreateItemInKnownFolder(FOLDERID_Documents, 0, NULL, IID_PPV_ARGS(&folder));

	if (FAILED(hr))
		wcout << _T("Error - problem with shell item object") << endl;
	else
		pRequester->SetFolder(folder);

	if (pickFolders) {
		FILEOPENDIALOGOPTIONS ops = 0;
		pRequester->GetOptions(&ops);
		ops |= FOS_PICKFOLDERS;
		ops |= FOS_FORCEFILESYSTEM;
		pRequester->SetOptions(ops);
	}

	vector<COMDLG_FILTERSPEC> filters;
	if (!pickFolders && !_filter.empty()) {
		if (_filter.empty() == false) {
			vector<wstring> parts;
			parts.reserve(12);

			wstring_view view = _filter;

			while (!view.empty()) {
				size_t pos = view.find(L'|');
				if (pos == wstring_view::npos) {
					parts.emplace_back(view);
					break;
				}

				wstring_view token = view.substr(0, pos);
				parts.emplace_back(token);

				view.remove_prefix(pos + 1);
			}

			parts.erase(
				remove_if(parts.begin(), parts.end(),
					[](const wstring& s) { return s.empty(); }),
				parts.end()
			);

			if (parts.size() % 2 != 0) {
				wcout << _T("Error - bad filter") << endl;
			}
			else {
				filters.reserve(parts.size() / 2);

				for (size_t i = 0; i < parts.size(); i += 2) {
					COMDLG_FILTERSPEC spec;
					spec.pszName = parts[i].c_str();
					spec.pszSpec = parts[i + 1].c_str();
					filters.push_back(spec);
				}

				pRequester->SetFileTypes(static_cast<UINT>(filters.size()), filters.data());
				pRequester->SetFileTypeIndex(1);
			}
		}
	}
	_result = _T("");

	HWND hwnd = GetActiveWindow();
	if (!hwnd) hwnd = GetForegroundWindow();
	if (!hwnd) hwnd = GetDesktopWindow();
	hr = pRequester->Show(hwnd);

	if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
		if (folder) folder->Release();
		if (pRequester) pRequester->Release();
		return false;
	}

	if (FAILED(hr)) {
		wcout << _T("Error - problem with dialog") << endl;
		if (folder) folder->Release();
		if (pRequester) pRequester->Release();
		return false;
	}

	IShellItem* pItem = nullptr;
	hr = pRequester->GetResult(&pItem);

	if (FAILED(hr) || !pItem) {
		wcout << _T("Error - problem with dialog result") << endl;
		if (folder) folder->Release();
		if (pRequester) pRequester->Release();
		return false;
	}

	PWSTR pszFilePath = nullptr;
	hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

	if (SUCCEEDED(hr) && pszFilePath) {
		_result = pszFilePath;
		CoTaskMemFree(pszFilePath);
	}
	else {
		pItem->Release();
		if (folder) folder->Release();
		if (pRequester) pRequester->Release();
		return false;
	}

	pItem->Release();

	if (folder) folder->Release();
	if (pRequester) pRequester->Release();

	return !_result.empty();
}
