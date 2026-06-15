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

wstring Conversion::ParseEscapeString(const wstring& _str)
{
	wstring out;
	for (size_t i = 0; i < _str.size(); ++i) {
		wchar_t c = _str[i];
		if (c != L'\\') {
			out.push_back(c);
			continue;
		}
		if (i + 1 >= _str.size()) {
			out.push_back(L'\\');
			break;
		}
		wchar_t n = _str[++i];

		// \n, \r, \t, \xHH, \uHHHH
		switch (n) {
		case L'n': out.push_back(L'\n'); break;
		case L'r': out.push_back(L'\r'); break;
		case L't': out.push_back(L'\t'); break;
		case L'\\': out.push_back(L'\\'); break;
		case L'\'': out.push_back(L'\''); break;
		case L'"': out.push_back(L'"'); break;
		case L'x':
		case L'u':
		{
			int maxDigits = (n == L'x') ? 2 : 4;
			int val = 0;
			int digits = 0;
			while (i + 1 < _str.size() && digits < maxDigits) {
				wchar_t h = _str[i + 1];
				int d;
				if (h >= L'0' && h <= L'9') d = h - L'0';
				else if (h >= L'a' && h <= L'f') d = 10 + (h - L'a');
				else if (h >= L'A' && h <= L'F') d = 10 + (h - L'A');
				else break;
				val = (val << 4) | d;
				++i;
				++digits;
			}

			if (digits == 0) {
				out.push_back(L'\\');
				out.push_back(n);
			}
			else {
				if (val >= 0xD800 && val <= 0xDFFF) {
					out.push_back(L'\\');
					out.push_back(n);
					for (int k = 0; k < digits; ++k)
						out.push_back(_str[i - digits + 1 + k]);
				}
				else {
					out.push_back((wchar_t)val);
				}
			}
			break;
		}
		default:
			out.push_back(n);
			break;
		}
	}
	return out;
}
