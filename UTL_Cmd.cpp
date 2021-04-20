#include "UTL_Cmd.h"
#include "UTL_Conversion.h"

bool UTL_Cmd::IsHelp(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-?") || comm == _T("-help") || comm == _T("-h")) return true;
	return false;
}

bool UTL_Cmd::IsTitle(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-title")) return true;
	return false;
}

bool UTL_Cmd::IsOpen(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-open")) return true;
	return false;
}

bool UTL_Cmd::IsSave(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-save")) return true;
	return false;
}

bool UTL_Cmd::IsFileName(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-filename")) return true;
	return false;
}

bool UTL_Cmd::IsOkLabel(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-ok")) return true;
	return false;
}

bool UTL_Cmd::IsPath(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-path")) return true;
	return false;
}

bool UTL_Cmd::IsDrawersOnly(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-drawersonly") || comm == _T("-foldersonly")) return true;
	return false;
}

bool UTL_Cmd::IsFilter(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-filter")) return true;
	return false;
}

void UTL_Cmd::Help()
{
	_tprintf(_T("FileRequester 0.1\n"));
	_tprintf(_T("	FileRequester for command line. Amiga Rulez!\n"));
	_tprintf(_T("\nUsage:\n"));
	_tprintf(_T("	FileRequester [OPTIONS]\n"));
	_tprintf(_T("\nOptions:\n"));
	_tprintf(_T("	-title TITLE\n"));
	_tprintf(_T("		The TITLE argument specifies the title of the file requester.\n"));
	_tprintf(_T("	-open\n"));
	_tprintf(_T("		Open mode. The file must exist.\n"));
	_tprintf(_T("	-save\n"));
	_tprintf(_T("		Save mode, requester is used for writing files to disk.\n"));
	_tprintf(_T("	-filename FILE\n"));
	_tprintf(_T("		The FILE argument specifies the default filename.\n"));
	_tprintf(_T("	-ok OKTEXT\n"));
	_tprintf(_T("		The OKTEXT argument specifies the text of ok button.\n"));
	_tprintf(_T("	-path PATH\n"));
	_tprintf(_T("		The PATH argument specifies the initial path of file requester. (-path \"d:\\\")\n"));
	_tprintf(_T("	-drawersonly\n"));
	_tprintf(_T("		If DRAWERSONLY is specified, the requester does not have a File gadget. This effectively turns the file requester into a directory requester.\n"));
	_tprintf(_T("	-filter FILTER\n"));
	_tprintf(_T("		The FILTER argument specifies the file types of file requester. (- filter \"Text|*.txt|All files|*.*\")\n"));
}

int UTL_Cmd::ParseCommandLinbe(int argc, _TCHAR* argv[], int& iCorrectParameters, bool& bHelp, wstring& title, bool& bOpen, bool& bSave, wstring& filename, wstring& okLabel, wstring& path, bool& bDrawersOnly, wstring& filter)
{
	iCorrectParameters = 0;

	for (int i = 0; i < argc; i++) {
		wstring tmp = argv[i];
		tmp = UTL_Conversion::TrimWhiteChar(tmp);

		if (UTL_Cmd::IsHelp(tmp)) {
			bHelp = true;
			iCorrectParameters++;
			continue;
		}

		if (UTL_Cmd::IsOpen(tmp)) {
			bOpen = true;
			iCorrectParameters++;
			continue;
		}

		if (UTL_Cmd::IsSave(tmp)) {
			bSave = true;
			iCorrectParameters++;
			continue;
		}

		if (UTL_Cmd::IsTitle(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				title = tmp;
				iCorrectParameters++;
			}
			continue;
		}

		if (UTL_Cmd::IsFileName(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				filename = tmp;
				iCorrectParameters++;
			}
			continue;
		}

		if (UTL_Cmd::IsOkLabel(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				okLabel = tmp;
				iCorrectParameters++;
			}
			continue;
		}

		if (UTL_Cmd::IsPath(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				path = tmp;
				iCorrectParameters++;
			}
			continue;
		}

		if (UTL_Cmd::IsDrawersOnly(tmp)) {
			bDrawersOnly = true;
			iCorrectParameters++;
			continue;
		}

		if (UTL_Cmd::IsFilter(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				filter = tmp;
				iCorrectParameters++;
			}
			continue;
		}
	}

	return 0;
}
