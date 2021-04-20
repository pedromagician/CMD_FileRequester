#pragma once

#include "stdafx.h"
#include "UTL_Cmd.h"
#include "UTL_Conversion.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int iCorrectParameters = 0;
	bool bHelp = false;
	bool bOpen = false;
	bool bSave = false;
	bool bDrawersOnly = false;
	wstring title = _T("File Request");
	wstring filename = _T("");
	wstring okLabel = _T("");
	wstring path = _T("");
	wstring filter = _T("");

	if (UTL_Cmd::ParseCommandLinbe(argc, argv, iCorrectParameters, bHelp, title, bOpen, bSave, filename, okLabel, path, bDrawersOnly, filter) != 0)
		bHelp = true;

	if (bHelp || iCorrectParameters == 0) {
		UTL_Cmd::Help();
		return 0;
	}

	if (bOpen == false && bSave == false)
		bOpen = true;

	if (bOpen == bSave) {
		_tprintf(_T("Error - open or save?\n"));
		return 1;
	}

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr)) {
		_tprintf(_T("Error - problem with COM library\n"));
		return 1;
	}

	IFileDialog* pRequester = nullptr;
	//IFileOpenDialog* pRequester;
	//IFileSaveDialog* pRequester;
	if (bOpen)
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pRequester));
	else
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pRequester));
	if (FAILED(hr)) {
		_tprintf(_T("Error - problem with creating object\n"));
		return 1;
	}

	pRequester->SetTitle(title.c_str());
	pRequester->SetFileName(filename.c_str());

	if (okLabel.empty() == false)
		pRequester->SetOkButtonLabel(okLabel.c_str());

	IShellItem* folder;
	if (path.empty())
		hr = SHCreateItemInKnownFolder(FOLDERID_Documents, 0, NULL, IID_PPV_ARGS(&folder));
	else
		hr = SHCreateItemFromParsingName(path.c_str(), NULL, IID_PPV_ARGS(&folder));
	if (FAILED(hr))
		_tprintf(_T("Error - problem with shell item object\n"));
	else
		pRequester->SetFolder(folder);

	if (bDrawersOnly) {
		FILEOPENDIALOGOPTIONS ops = FOS_PICKFOLDERS;
		pRequester->SetOptions(ops);
	}

	if (filter.empty() == false) {
		vector<wstring>vresult;
		while (filter.size()) {
			int index = filter.find(_T("|"));
			if (index != string::npos) {
				vresult.push_back(filter.substr(0, index));
				filter = filter.substr(index + 1);
				if (filter.size() == 0) vresult.push_back(filter);
			}
			else {
				vresult.push_back(filter);
				filter = _T("");
			}
		}

		if (vresult.size() % 2 == 0) {
			COMDLG_FILTERSPEC* pFilter = new COMDLG_FILTERSPEC[vresult.size()];
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
			_tprintf(_T("Error - bad filter\n"));
		}
	}

	wstring strResult = _T("");
	hr = pRequester->Show(NULL);
	if (SUCCEEDED(hr)) {
		IShellItem* pItem;
		hr = pRequester->GetResult(&pItem);
		if (SUCCEEDED(hr)) {
			PWSTR pszFilePath;
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

			if (SUCCEEDED(hr)) {
				strResult = pszFilePath;
				CoTaskMemFree(pszFilePath);
			}
			pItem->Release();
		}
		else {
			_tprintf(_T("Error - problem with dialog\n"));
			return 1;
		}
	}

	pRequester->Release();
	folder->Release();
	CoUninitialize();

	_tprintf(strResult.c_str());
	return 0;
}
