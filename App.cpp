#include "stdafx.h"

Debug theDebug;

#include "CommandLine.h"
#include "Conversion.h"
#include "FileRequester.h"

int _tmain(int _argc, _TCHAR* _pArgv[])
{
	_setmode(_fileno(stdout), _O_U8TEXT);
	
	int correctParameters = 0;
	bool help = false;
	bool open = false;
	bool save = false;
	bool directory = false;
	bool drawersOnly = false;
	wstring title = _T("File Request");
	wstring filename = _T("");
	wstring path = _T("");
	wstring filter = _T("");
	wstring okLabel = _T("");

	CommandLine cmd;
	{
		cmd.AddBool({ _T("help"), _T("h"), _T("?") },
			_T("Displays this help information."),
			help);

		cmd.AddString({ _T("title"), _T("t") },
			_T("Specifies the title text displayed in the file requester window."),
			title);

		cmd.AddBool({ _T("open"), _T("o") },
			_T("Enables Open mode. Displays a file requester to select an existing file that must already exist on disk."),
			open);

		cmd.AddBool({ _T("save"), _T("s") },
			_T("Enables Save mode. Displays a file requester to specify a destination filename; the file does not need to exist yet."),
			save);

		cmd.AddBool({ _T("directory"), _T("d") },
			_T("Enables Directory mode. Displays a requester specifically for selecting a directory path instead of a file."),
			directory);

		cmd.AddString({ _T("filename"), _T("f") },
			_T("Specifies the default filename pre-filled in the requester field."),
			filename);

		cmd.AddString({ _T("ok") },
			_T("Specifies the custom label text for the confirmation (OK) button in the requester."),
			okLabel);

		cmd.AddString({ _T("path") },
			_T("Specifies the initial directory path opened by the file requester (e.g., \"d:\\\")."),
			path);

		cmd.AddBool({ _T("drawersonly"), _T("foldersonly") },
			_T("Removes the file selection gadget from the requester, restricting selection to directories only. Effective primarily in Open mode."),
			drawersOnly);

		cmd.AddString({ _T("filter") },
			_T("Defines the file type filters available in the requester using the format 'Description|*.ext' (e.g., \"Text|*.txt|All files|*.*\")."),
			filter);
	}

	if (!cmd.ParseCommandLine(_argc, _pArgv, correctParameters) || correctParameters == 0 || help) {
		cmd.Help();
		return 0;
	}

	wstring result = _T("");
	if (FileRequester::Requester(save, open, directory, drawersOnly, title, path, filename, okLabel, filter, result) == false)
		return 1;

	wcout << result;
	return 0;
}
