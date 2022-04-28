#pragma once

#include "stdafx.h"

class Conversion {
public:
	static int ToInt(const wstring& _val);

	static wstring LeftTrimString(wstring _str, const wstring& _val);
	static wstring RightTrimString(wstring _str, const wstring& _val);
	static wstring TrimString(wstring _str, const wstring& _val);
	static wstring TrimWhiteChar(const wstring& _val);

	static wstring ToLower(wstring _val);

	static void StringReplaceAll(wstring& _mess, const wstring& _oldStr, const wstring& _newStr);
};
