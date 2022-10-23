#pragma once

#include <Windows.h>

#include <iostream>
#include <memory>
#include <map>
#include <tstring.h>

struct ClipBoardData
{
	UINT format;
	SIZE_T size;
	std::unique_ptr<unsigned char[]> data;
	ClipBoardData(UINT _format, SIZE_T _size, LPCSTR _data) :format(_format), size(_size)
	{
		data.reset(new unsigned char[_size]);
		memcpy(data.get(), _data, _size);
	}
	ClipBoardData() :format(0), size(0) {}
};

std::ostream &operator<<(std::ostream &out, const ClipBoardData &cb);
std::wostream &operator<<(std::wostream &out, const ClipBoardData &cb);

bool UpdateClipBoardData(ClipBoardData &cb, HWND hwnd=NULL);