#include "ClipBoardData.h"

#include <stdexcept>

using namespace std;

std::ostream &operator<<(std::ostream &out, const ClipBoardData &cb)
{
	out << "Format = " << cb.format << endl;
	out << "Size = " << cb.size << endl;
	out << "Data = " << cb.data.get() << endl;
	return out;
}

std::wostream &operator<<(std::wostream &out, const ClipBoardData &cb)
{
	out << "Format = " << cb.format << endl;
	out << "Size = " << cb.size << endl;
	out << "Data = " << cb.data.get() << endl;
	out << "Hex Data = " << to_hex((TCHAR *)cb.data.get()) << endl;
	return out;
}


std::map<UINT, ClipBoardData> ReadClipBoard()
{
	std::map<UINT, ClipBoardData> ans;
	if (OpenClipboard(NULL) == false)
		throw runtime_error("Failed on OpenClipboard");

	UINT clipboard_format = 0;
	while (clipboard_format = EnumClipboardFormats(clipboard_format))
	{
		HGLOBAL global_memory = GetClipboardData(clipboard_format);//取得剪贴板
		LPCSTR clipboard_data = (LPCSTR)GlobalLock(global_memory);//取得数据并锁定
		if (clipboard_data != NULL)
		{
			// 剪切板上数据大小
			SIZE_T data_size = GlobalSize(global_memory);

			ans[clipboard_format] = ClipBoardData(clipboard_format, data_size, clipboard_data);
		}
		else
		{
			CloseClipboard();
			throw runtime_error("Failed on GlobalLock");
		}
		GlobalUnlock(global_memory);
	}
	CloseClipboard();
	return ans;
}

bool UpdateClipBoardData(ClipBoardData &cb, HWND hwnd)
{
	if (IsClipboardFormatAvailable(cb.format) == false)
		return false;

	if (OpenClipboard(hwnd) == false)
	{
		auto err = GetLastError();
		throw runtime_error("Failed on OpenClipboard, code=" + to_string(err));
	}

	HGLOBAL global_memory = GetClipboardData(cb.format);//取得剪贴板
	LPCSTR clipboard_data = (LPCSTR)GlobalLock(global_memory);//取得数据并锁定
	if (clipboard_data != NULL)
	{
		// 剪切板上数据大小
		SIZE_T data_size = GlobalSize(global_memory);

		// check
		if (data_size == cb.size)
		{
			if (memcmp(clipboard_data, cb.data.get(), cb.size) == 0)
				return false;
		}

		cb.size = data_size;
		cb.data.reset(new unsigned char[data_size]);
		memcpy(cb.data.get(), clipboard_data, data_size);
	}
	else
	{
		CloseClipboard();
		throw runtime_error("Failed on GlobalLock");
	}
	GlobalUnlock(global_memory);

	if (CloseClipboard() == false)
	{
		auto err = GetLastError();
		throw runtime_error("Failed on CloseClipboard, code=" + to_string(err));
	}
	return true;
}