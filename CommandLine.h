#pragma once

#include "stdafx.h"

#include <vector>
#include <map>
#include <string>
#include <tchar.h>

class CommandLine
{
public:
	enum class ParamType {
		BOOL,
		INT,
		STRING,
		ENUM,
		CHAR
	};

	enum class RequiredParam {
		Optional = false,
		Required = true
	};

	struct ParamDef {
		bool seen;

		vector<wstring> names;
		wstring description;
		ParamType type;

		RequiredParam required;
		bool hasDefault;

		bool* outBool;
		int* outInt;
		wstring* outString;
		int* outEnum;
		wchar_t* outChar;

		map<wstring, int> enumMap;

		int defaultInt;
		wstring defaultString;
		int defaultEnum;
		wchar_t defaultChar;

		ParamDef();
	};

private:
	vector<ParamDef> mParams;
	map<wstring, size_t> mAliasMap;
	bool mHelp;

	void AddParamBase(ParamDef& _p);

public:
	CommandLine();

	void AddHelp(const vector<wstring>& _names, const wstring& _desc, bool& _outVar);
	void AddBool(const vector<wstring>& _names, const wstring& _desc, bool& _outVar, RequiredParam _required = RequiredParam::Optional);
	void AddInt(const vector<wstring>& _names, const wstring& _desc, int& _outVar, RequiredParam _required = RequiredParam::Optional, int _defaultValue = 0);
	void AddString(const vector<wstring>& _names, const wstring& _desc, wstring& _outVar, RequiredParam _required = RequiredParam::Optional, const wstring& _defaultValue = L"");
	void AddEnum(const vector<wstring>& _names, const wstring& _desc, const map<wstring, int>& _enumMap, int& _outVar, RequiredParam _required = RequiredParam::Optional, int _defaultValue = 0);
	void AddChar(const vector<wstring>& _names, const wstring& _desc, wchar_t& _outVar, RequiredParam _required = RequiredParam::Optional, wchar_t _defaultValue = '\0');

	bool ParseCommandLine(int _argc, wchar_t** _argv, int& _correctCount);
	void Help();
};
