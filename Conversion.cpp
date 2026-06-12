#include "conversion.h"

static const wchar_t WHITE_SPACE_CHARACTERS[] = L" \t\n\v\f\r\u00A0";

int Conversion::ToInt(const wstring& _buff)
{
	return _tcstol(_buff.c_str(), nullptr, 10);
}

wstring Conversion::LeftTrimString(const wstring& input, const wstring& chars)
{
	const size_t pos = input.find_first_not_of(chars);
	if (pos == wstring::npos)
		return wstring();

	return input.substr(pos);
}

wstring Conversion::RightTrimString(const wstring& input, const wstring& chars)
{
	const size_t pos = input.find_last_not_of(chars);
	if (pos == wstring::npos)
		return wstring();

	return input.substr(0, pos + 1);
}

wstring Conversion::TrimString(const wstring& input, const wstring& chars)
{
	const size_t end = input.find_last_not_of(chars);
	if (end == wstring::npos)
		return wstring();

	const size_t begin = input.find_first_not_of(chars);
	return input.substr(begin, end - begin + 1);
}

wstring Conversion::TrimWhiteChar(const wstring& _val)
{
	return TrimString(_val, WHITE_SPACE_CHARACTERS);
}

wstring Conversion::ToLower(const wstring& input)
{
	if (input.empty())
		return {};

	wstring result = input;
	CharLowerBuffW(result.data(), static_cast<DWORD>(result.size()));
	return result;
}

void Conversion::StringReplaceAll(wstring& _mess, const wstring& _oldStr, const wstring& _newStr)
{
	const size_t oldLen = _oldStr.length();
	const size_t newLen = _newStr.length();
	size_t position = 0;
	while ((position = _mess.find(_oldStr, position)) != wstring::npos)
	{
		_mess.replace(position, oldLen, _newStr);
		position += newLen;
	}
}
