Debug theDebug; 

#include "stdafx.h"
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
	cmd.Add(CommandLine::_TRUE,		3,	_T("-help"),		_T("-h"),			_T("-?"),	_T("To view help."),																							&help);
	cmd.Add(CommandLine::_STRING,	2,	_T("-title"),		_T("-t"),						_T("The 'xxx' argument specifies the title of the file requester."),											&title);
	cmd.Add(CommandLine::_TRUE,		2,	_T("-open"),		_T("-o"),						_T("Open mode. Requester is used to select an existing file. The file must exist."),							&open);
	cmd.Add(CommandLine::_TRUE,		2,	_T("-save"),		_T("-s"),						_T("Save mode. Requester is used to select a file. The file does not have to exist."),							&save);
	cmd.Add(CommandLine::_TRUE,		2,	_T("-directory"),	_T("-d"),						_T("Directory mode. This request is used to select a directory."),												&directory);
	cmd.Add(CommandLine::_STRING,	2,	_T("-filename"),	_T("-f"),						_T("The 'xxx' argument specifies the default filename."),														&filename);
	cmd.Add(CommandLine::_STRING,	1,	_T("-ok"),											_T("The 'xxx' argument specifies the text of ok button."),														&okLabel);
	cmd.Add(CommandLine::_STRING,	1,	_T("-path"),										_T("The 'xxx' argument specifies the initial path of file requester. (-path \"d:\\\")"),						&path);
	cmd.Add(CommandLine::_TRUE,		2,	_T("-drawersonly"),	_T("-foldersonly"),				_T("If drawersonly is specified, the requester does not have a File gadget. This effectively turns the file requester into a directory requester. (only for open mode)"),	&drawersOnly);
	cmd.Add(CommandLine::_STRING,	1,	_T("-filter"),										_T("The 'xxx' argument specifies the file types of file requester. (-filter \"Text|*.txt|All files|*.*\")."),	&filter);

	if (cmd.ParseCommandLine(_argc, _pArgv, correctParameters) != 0) {
		cmd.Help();
		return 0;
	}

	if (help || correctParameters == 0) {
		cmd.Help();
		return 0;
	}

	wstring result = _T("");
	if (FileRequester::Requester(save, open, directory, drawersOnly, title, path, filename, okLabel, filter, result) == false)
		return 1;

	wcout << result;
	return 0;
}
