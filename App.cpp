#pragma once

#include "stdafx.h"
#include "CommandLine.h"
#include "Conversion.h"

int _tmain(int _argc, _TCHAR* _pArgv[])
{
	int correctParameters = 0;
	bool help = false;
	bool open = false;
	bool save = false;
	bool drawersOnly = false;
	wstring title = _T("File Request");
	wstring filename = _T("");
	wstring path = _T("");
	wstring filter = _T("");
	wstring okLabel = _T("");

	CommandLine cmd;
	cmd.Add(CommandLine::_TRUE,		3,	_T("-help"),		_T("-h"),			_T("-?"),	_T("To view help."),																							&help);
	cmd.Add(CommandLine::_STRING,	2,	_T("-title"),		_T("-t"),						_T("The 'xxx' argument specifies the title of the file requester."),											&title);
	cmd.Add(CommandLine::_TRUE,		2,	_T("-open"),		_T("-o"),						_T("Open mode. The file must exist."),																			&open);
	cmd.Add(CommandLine::_TRUE,		2,	_T("-save"),		_T("-s"),						_T("Save mode, requester is used for writing files to disk."),													&save);
	cmd.Add(CommandLine::_STRING,	2,	_T("-filename"),	_T("-f"),						_T("The 'xxx' argument specifies the default filename."),														&filename);
	cmd.Add(CommandLine::_STRING,	1,	_T("-ok"),											_T("The 'xxx' argument specifies the text of ok button."),														&okLabel);
	cmd.Add(CommandLine::_STRING,	1,	_T("-path"),										_T("The 'xxx' argument specifies the initial path of file requester. (-path \"d:\\\")"),						&path);
	cmd.Add(CommandLine::_TRUE,		2,	_T("-drawersonly"),	_T("-foldersonly"),				_T("If drawersonly is specified, the requester does not have a File gadget. This effectively turns the file requester into a directory requester."),	&drawersOnly);
	cmd.Add(CommandLine::_STRING,	1,	_T("-filter"),										_T("The 'xxx' argument specifies the file types of file requester. (-filter \"Text|*.txt|All files|*.*\")."),	&filter);

	if (cmd.ParseCommandLine(_argc, _pArgv, correctParameters) != 0) {
		cmd.Help();
		return 0;
	}

	if (help || correctParameters == 0) {
		cmd.Help();
		return 0;
	}

	if (open == false && save == false)
		open = true;

	if (open == save) {
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
	if (open)
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

	if (drawersOnly) {
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
