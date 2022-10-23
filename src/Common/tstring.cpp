#include "tstring.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <memory>

using namespace std;


std::wstring string_to_wstring(const std::string& str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return std::wstring(L"");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return std::wstring(L"");

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return wstr;
}

void MyWideCharToMultiByte(const wchar_t *wsrc, int wsrcSize, std::unique_ptr<char[]> &dest, int &destSize, UINT codePage)
{
	// ȡ�ô�С
	destSize = WideCharToMultiByte(codePage, 0, wsrc, wsrcSize, NULL, 0, NULL, NULL);
	if (destSize == 0) // ��СΪ0����ʧ�ܣ����ؿմ�
	{
		dest.reset(nullptr);
		return;
	}

	// ����ռ�
	dest.reset(new char[destSize]);

	// ����ת��
	WideCharToMultiByte(codePage, 0, wsrc, wsrcSize, dest.get(),destSize, NULL, NULL);
}

void MyMultiByteToWideChar(const char *src, int srcSize, std::unique_ptr<wchar_t[]> &dest, int &destSize, UINT codePage)
{
	// ȡ�ô�С
	destSize = MultiByteToWideChar(codePage, 0, src, srcSize, NULL, 0);
	if (destSize == 0) // ��СΪ0����ʧ�ܣ����ؿմ�
	{
		dest.reset(nullptr);
		return;
	}

	// ����ռ�
	dest.reset(new wchar_t[destSize]);

	// ����ת��
	MultiByteToWideChar(codePage, 0, src, srcSize, dest.get(), destSize);
}

std::string wstring_to_string(const std::wstring& wstr)
{
	// ȡ�ô�С
	int nLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (nLen == 0) // ��СΪ0����ʧ�ܣ����ؿմ�
		return std::string("");

	// ����ռ�
	unique_ptr<char[]> 	buf = unique_ptr<char[]>(new char[nLen]);

	// ����ת��
	nLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, buf.get(), nLen, NULL, NULL);
	if (nLen == 0) // ��СΪ0����ʧ�ܣ����ؿմ�
		return std::string("");

	return std::string(buf.get());
}

std::string to_string(const std::wstring& ws)
{
	return wstring_to_string(ws);
}

std::string to_string(const std::string& s)
{
	return s;
}

std::wstring to_wstring(const std::string& s)
{
	return string_to_wstring(s);
}

std::wstring to_wstring(const std::wstring& s)
{
	return s;
}

void tolower(std::string& s)
{
	for_each(s.begin(), s.end(), [](char& c) {c = tolower(c); });
}

void toupper(std::string& s)
{
	for_each(s.begin(), s.end(), [](char& c) {c = toupper(c); });
}

std::string to_utf8(const std::wstring& wstr)
{
	// ȡ�ô�С
	int nLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (nLen == 0) // ��СΪ0����ʧ�ܣ����ؿմ�
		return std::string("");

	// ����ռ�
	unique_ptr<char[]> 	buf = unique_ptr<char[]>(new char[nLen]);

	// ����ת��
	nLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buf.get(), nLen, NULL, NULL);
	if (nLen == 0) // ��СΪ0����ʧ�ܣ����ؿմ�
		return std::string("");

	return std::string(buf.get());
}

std::string to_utf8(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	unique_ptr<wchar_t[]> pwBuf(new wchar_t[nwLen]);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf.get(), nwLen);

	int nLen = WideCharToMultiByte(CP_UTF8, 0, pwBuf.get(), -1, NULL, NULL, NULL, NULL);
	unique_ptr<char[]> pBuf(new char[nLen]);
	WideCharToMultiByte(CP_UTF8, 0, pwBuf.get(), nwLen, pBuf.get(), nLen, NULL, NULL);

	return string(pBuf.get());
}

std::string utf8_to_string(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string ret = pBuf;
	delete[]pBuf;
	delete[]pwBuf;

	return ret;
}

std::wstring utf8_to_wstring(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), pwBuf, nwLen);

	std::wstring ret = pwBuf;
	delete[]pwBuf;

	return ret;
}

// δ����
void FixEndLine(std::tstring& s)
{
	tstring temp;
	tstring::size_type cur = 0;
	while (1)
	{
		auto lf = s.find_first_of(TEXT('\n'), cur);
		if (lf == tstring::npos)
		{
			temp += s.substr(cur);
			break;
		}
		else
			if (lf > cur && s[lf - 1] == TEXT('\r'))
			{
				temp += s.substr(cur, (lf + 1) - cur);
				cur = lf + 1;
			}
			else
			{
				temp += s.substr(cur, lf - cur);
				cur = lf + 1;
			}
	}
	s = temp;
}

std::string to_hex(const char *buf, int bufSize)
{
	constexpr int len = 4;
	std::string hex;
	char temp[len];
	for (int i=0;i<bufSize;++i)
	{
		_snprintf_s(temp, len, "%02X ", (unsigned char)(buf[i]));
		hex += temp;
	}
	return hex;
}

std::string to_hex(std::string s)
{
	constexpr int len = 4;
	std::string hex;
	char temp[len];
	for (char c : s)
	{
		_snprintf_s(temp, len, "%02X ", (unsigned char)c);
		hex += temp;
	}
	return hex;
}

std::wstring to_hex(std::wstring s)
{
	std::wstring hex;
	wchar_t temp[5];
	for (auto c : s)
	{
		_snwprintf_s(temp, 5, 5, TEXT("%04X"), c);
		hex += temp;
	}
	return hex;
}

// https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
std::tistream& safeGetline(std::tistream& is, std::tstring& t)
{
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::tistream::sentry se(is, true);
	std::tstreambuf* sb = is.rdbuf();

	for (;;) {
		TCHAR c = sb->sbumpc();
		switch (c) {
		case TEXT('\n'):
			return is;
		case TEXT('\r'):
			if (sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case std::tstreambuf::traits_type::eof():
			// Also handle the case when the last line has no line ending
			if (t.empty())
				is.setstate(std::tios::eofbit);
			return is;
		default:
			t += c;
		}
	}
}