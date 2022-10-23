#include "DialogMain.h"

#include <tstring.h>

#ifdef _DEBUG
#include <iostream>
#endif

#include <stdexcept>
#include <sstream>
#include <set>

using namespace std;

void WINAPI SetAutoView(HWND hwnd)
{
	static UINT auPriorityList[] = {
		CF_OWNERDISPLAY,
		CF_TEXT,
		CF_ENHMETAFILE,
		CF_BITMAP
	};

	GetPriorityClipboardFormat(auPriorityList, 4);

	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

void DialogMain::UpdateControlState()
{
	editPlain.SetReadOnly(true);
	editAsUnicode.SetReadOnly(true);
	editAsUtf8.SetReadOnly(true);

	btnPastePlain.SetEnable(false);
	btnPasteUnicode.SetEnable(false);
	btnPasteUtf8.SetEnable(false);

	if (leftToRight)
	{
		// ->
		btnDirection.SetText(L"->");

		editHex.SetReadOnly(false);

		//editHex.SetFocus();

		radioAs.SetEnable(false);
	}
	else
	{
		btnDirection.SetText(L"<-");

		editHex.SetReadOnly(true);

		radioAs.SetEnable(true);
		switch (radioAs.GetValue())
		{
		case IDC_RADIO_AS_PLAIN:
			editPlain.SetReadOnly(false);
			btnPastePlain.SetEnable(true);
			//editPlain.SetFocus();
			break;
		case IDC_RADIO_AS_UNICODE:
			editAsUnicode.SetReadOnly(false);
			btnPasteUnicode.SetEnable(true);
			//editAsUnicode.SetFocus();
			break;
		case IDC_RADIO_AS_UTF8:
			editAsUtf8.SetReadOnly(false);
			btnPasteUtf8.SetEnable(true);
			//editAsUtf8.SetFocus();
			break;
		}
	}

	if (radioCodePage.GetValue() == IDC_RADIO_OTHER)
	{
		comboOther.SetEnable(true);
	}
	else
	{
		comboOther.SetEnable(false);
	}
}

void DialogMain::Parse()
{
	int hexDataWithTail0Size = hexDataSize + 3;
	unique_ptr<char[]> hexDataWithTail0(new char[hexDataWithTail0Size]);
	memcpy(hexDataWithTail0.get(), hexData.get(), hexDataSize);
	hexDataWithTail0.get()[hexDataWithTail0Size - 3] = 0;
	hexDataWithTail0.get()[hexDataWithTail0Size - 2] = 0;
	hexDataWithTail0.get()[hexDataWithTail0Size - 1] = 0;

	// parse as plain text
	unique_ptr<wchar_t[]> newRaw;
	int newRawSize = 0;
	MyMultiByteToWideChar(hexDataWithTail0.get(), hexDataWithTail0Size, newRaw, newRawSize, curCodePage);
	if (newRawSize)
	{
		editPlain.SetTextKeepSel(newRaw.get());
	}

	// parse as unicode
	if (radioUnicodeEndian.GetValue() == IDC_RADIO_UNICODE_BIG_ENDIAN)
	{
		unique_ptr<wchar_t[]> unicodeData((wchar_t *)new char[hexDataWithTail0Size]);
		memcpy(unicodeData.get(), hexDataWithTail0.get(), hexDataWithTail0Size);

		int len = hexDataSize / 2;
		for (int i = 0; i < len; ++i)
		{
			char *p0 = (char *)(unicodeData.get() + i);
			char *p1 = p0 + 1;
			swap(*p0, *p1);
		}

		editAsUnicode.SetTextKeepSel(unicodeData.get());
	}
	else
	{
		editAsUnicode.SetTextKeepSel((wchar_t *)hexDataWithTail0.get());

	}

	// parse as utf8
	editAsUtf8.SetTextKeepSel(utf8_to_wstring(hexDataWithTail0.get()));
}

void DialogMain::OnClose()
{
	ChangeClipboardChain(hwndNextViewer);
	EndDialog(0);
}

void DialogMain::OnChangeCbChain(CWindow wndRemove, CWindow wndAfter)
{
	// If the next window is closing, repair the chain. 
	if (wndRemove.m_hWnd == hwndNextViewer)
		hwndNextViewer = wndAfter.m_hWnd;

	// Otherwise, pass the message to the next link. 
	else if (hwndNextViewer != NULL)
	{
		SendMessage(hwndNextViewer, WM_CHANGECBCHAIN, (WPARAM)wndRemove.m_hWnd, (LPARAM)wndAfter.m_hWnd);
	}
}

void DialogMain::OnDrawClipBoard()
{
	tcout << "OnDrawClipBoard" << endl;
	try
	{
		if (UpdateClipBoardData(cb, m_hWnd))
		{
			//tcout << cb << endl;
			//editPlain.SetText((TCHAR *)cb.data.get());

			unique_ptr<char[]> s;
			int sLen = 0;
			MyWideCharToMultiByte((TCHAR *)cb.data.get(), -1, s, sLen);

			//Parse(s.get(), sLen);

			editHex.SetText(to_wstring(to_hex(s.get())));

			//wstring s2 = utf8_to_wstring(s.get());
			////tcout << s2 << endl;

			//editAsUtf8.SetText(s2);
		}
	}
	catch (runtime_error &e)
	{
		tcout << e.what() << endl;
		//MessageBox(to_wstring(e.what()).c_str(), L"Error", MB_OK);
	}

	// Pass the message to the next window in clipboard 
	// viewer chain. 

	if (hwndNextViewer)
		SendMessage(hwndNextViewer, WM_DRAWCLIPBOARD, 0, 0);
}

LRESULT DialogMain::OnEditHexEnChange(WORD wParamHigh, WORD wParamLow, HWND lParam, BOOL &bHandled)
{
	wstring rawWithBlank = editHex.GetText(); // 取得hex格式化之前的字符串
	wstring formated; // hex格式化之后的字符串

	int bufIndex = 0;
	int bufSize = rawWithBlank.length() / 2 + 2;
	unique_ptr<char[]> buf(new char[bufSize]);

	int len = rawWithBlank.length();
	unsigned char c = 0;
	bool high = true;
	for (wchar_t wc : rawWithBlank)
	{
		if (wc == L' ')
		{
			continue;
		}

		// A-F
		if (wc >= L'A' && wc <= L'F')
		{
			unsigned char temp = wc - L'A' + 0xA;
			if (high)
			{
				c |= (temp << 4);

				//
				formated += wc;
			}
			else
			{
				c |= temp;
				buf[bufIndex++] = c;
				c = 0;

				//
				formated += wc;
				formated += L' ';
			}
			high = !high;
			continue;
		}

		// 0-9
		if (wc >= L'0' && wc <= L'9')
		{
			unsigned char temp = wc - L'0' + 0x0;
			if (high)
			{
				c |= (temp << 4);

				//
				formated += wc;
			}
			else
			{
				c |= temp;
				buf[bufIndex++] = c;
				c = 0;

				//
				formated += wc;
				formated += L' ';
			}
			high = !high;
			continue;
		}
	}

	buf[bufIndex++] = 0; // 添加末尾0

	auto sel = editHex.GetSel().first;
	editHex.SetText(formated);
	if (formated.length() > rawWithBlank.length())
	{
		sel += formated.length() - rawWithBlank.length();
	}
	editHex.SetSel(sel, sel);

	//
	hexData.reset(buf.release());
	hexDataSize = bufIndex;
	Parse();

	return 0;
}

LRESULT DialogMain::OnEditPlainEnChange(WORD wParamHigh, WORD wParamLow, HWND lParam, BOOL &bHandled)
{
	// 右到左，且以plain text为准
	if (leftToRight == false && radioAs.GetValue() == IDC_RADIO_AS_PLAIN)
	{
		// 更改hex
		MyWideCharToMultiByte(editPlain.GetText().c_str(), -1, hexData, hexDataSize, curCodePage);

		editHex.SetTextKeepSel(to_wstring(to_hex(hexData.get())));
	}

	Parse();

	return LRESULT();
}

LRESULT DialogMain::OnCheckMonitorClipBoard(WORD wParamHigh, WORD wParamLow, HWND lParam, BOOL &bHandled)
{
	//tcout << checkboxMonitor.GetChecked() << endl;
	//if (checkboxMonitor.GetChecked())
	//{
	//	// Add the window to the clipboard viewer chain. 
	//	hwndNextViewer = SetClipboardViewer();
	//	tcout << "hwndNextViewer" << hwndNextViewer << endl;
	//}
	//else
	//{
	//	ChangeClipboardChain(hwndNextViewer);
	//	hwndNextViewer = 0;
	//}
	return LRESULT();
}

BOOL DialogMain::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	// 设置窗口的大小图标
	// 大图标：按下alt+tab键切换窗口时对应的图标
	// 小图标：就是窗口左上角对应的那个图标
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	::SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	::SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	SetWindowText(TEXT("汤姆猫编码方式转换器 by Tom Willow v0.1"));

	CheckCodePageSupport(m_hWnd, { CP_GBK,CP_GB18030,CP_BIG5,CP_SHIFT_JIS });

	//
	editHex.LinkControl(m_hWnd, IDC_EDIT_HEX);
	btnDirection.LinkControl(m_hWnd, IDC_BTN_DIRECTION);

	radioAs.LinkControl(m_hWnd, { IDC_RADIO_AS_PLAIN,IDC_RADIO_AS_UNICODE,IDC_RADIO_AS_UTF8 }, { IDC_RADIO_AS_PLAIN, IDC_RADIO_AS_UNICODE, IDC_RADIO_AS_UTF8 });
	editPlain.LinkControl(GetDlgItem(IDC_EDIT_PLAIN));
	editAsUnicode.LinkControl(m_hWnd, IDC_EDIT_AS_UNICODE);
	editAsUtf8.LinkControl(m_hWnd, IDC_EDIT_AS_UTF8);

	radioCodePage.LinkControl(m_hWnd, { IDC_RADIO_GB18030,IDC_RADIO_BIG5,IDC_RADIO_OTHER }, { IDC_RADIO_GB18030,IDC_RADIO_BIG5,IDC_RADIO_OTHER });

	comboOther.LinkControl(m_hWnd, IDC_COMBO_OTHER);
	comboOther.SetHeight(500);
	comboOther.AddItem(L"Shift-JIS", (void *)CP_SHIFT_JIS);
	comboOther.AddItem(L"ISO-8859-1", (void *)CP_ISO8859_1);

	radioUnicodeEndian.LinkControl(m_hWnd, { IDC_RADIO_UNICODE_LITTLE_ENDIAN,IDC_RADIO_UNICODE_BIG_ENDIAN }, { IDC_RADIO_UNICODE_LITTLE_ENDIAN,IDC_RADIO_UNICODE_BIG_ENDIAN });

	//checkboxMonitor.LinkControl(m_hWnd, IDC_CHECK_MONITOR_CLIPBOARD);

	btnPastePlain.LinkControl(m_hWnd, IDC_BTN_PASTE_PLAIN);
	btnPasteUnicode.LinkControl(m_hWnd, IDC_BTN_PASTE_UNICODE);
	btnPasteUtf8.LinkControl(m_hWnd, IDC_BTN_PASTE_UTF8);

	editHex.RegisterMessage(WM_CHAR, [](TWindowBase *self, UINT, WPARAM key, LPARAM)->LRESULT
		{
			if (key == 8) // backspace
			{
				// 如果退格时，前面一个是空格，格式化后删除动作会失效。
				// 所以此处预处理，把光标手动向前移一格
				TEdit *mine = (TEdit *)self;
				auto sel = mine->GetSel();
				if (sel.first == sel.second)
				{
					int cur = sel.first;
					if (cur > 0 && cur - 1 < mine->GetText().length() && mine->GetText()[cur - 1] == L' ')
					{
						mine->SetCur(cur - 1);
					}
				}

				// 交由默认处理
				return true;
			}

			if (key >= '0' && key <= '9')
				return true;

			if (key >= 'A' && key <= 'F')
				return true;

			if (key >= 'a' && key <= 'f')
				return true;

			// can not input other char
			return false;
		});

	setlocale(LC_CTYPE, "");

	cb.format = CF_UNICODETEXT;
	hexDataSize = 0;

	// initial controls
	radioAs.SetChecked(IDC_RADIO_AS_PLAIN);
	radioCodePage.SetChecked(IDC_RADIO_GB18030);
	comboOther.SetCurSel(0);
	radioUnicodeEndian.SetChecked(IDC_RADIO_UNICODE_LITTLE_ENDIAN);

	curCodePage = CP_GB18030;
	UpdateControlState();
	//checkboxMonitor.SetChecked(false);

	BOOL bHandled = TRUE;
	OnCheckMonitorClipBoard(0, 0, 0, bHandled);

	CenterWindow();

	SetMsgHandled(false);

	return 0;
}

void DialogMain::OnCommand(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (nID == IDC_COMBO_OTHER && uNotifyCode == CBN_SELCHANGE)
	{
		curCodePage = (UINT)comboOther.GetCurSel();

		if (leftToRight)
		{
			// 根据HEX得到明文
			Parse();
		}
		else
		{
			// 根据明文转换到HEX
			BOOL bHandled;
			OnEditPlainEnChange(0, 0, 0, bHandled);
		}

		UpdateControlState();

		SetMsgHandled(false);
		return;
	}

	if (uNotifyCode == BN_CLICKED)
	{
		switch (nID)
		{
		case IDC_BTN_DIRECTION:
			leftToRight = !leftToRight;
			break;
		case IDC_BTN_COPY_PLAIN:
			editPlain.Copy();
			break;
		case IDC_BTN_PASTE_PLAIN:
			editPlain.Paste();
			break;
		case IDC_BTN_COPY_UNICODE:
			editAsUnicode.Copy();
			break;
		case IDC_BTN_PASTE_UNICODE:
			editAsUnicode.Paste();
			break;
		case IDC_BTN_COPY_UTF8:
			editAsUtf8.Copy();
			break;
		case IDC_BTN_PASTE_UTF8:
			editAsUtf8.Paste();
			break;
		}

		if (radioAs.ReceiveCommand(nID))
		{
			//BOOL bHandled;
			//switch (radioAs.GetValue())
			//{
			//case IDC_RADIO_AS_PLAIN:
			//	// 根据明文转换到HEX
			//	OnEditPlainEnChange(0, 0, 0, bHandled);
			//	break;
			//case IDC_RADIO_AS_UNICODE:
			//	// 根据明文转换到HEX
			//	OnEditUnicodeEnChange(0, 0, 0, bHandled);
			//	break;
			//case IDC_RADIO_AS_UTF8:
			//	// 根据明文转换到HEX
			//	OnEditUTF8EnChange(0, 0, 0, bHandled);
			//	break;
			//}
		}

		// 更新 代码页 状态
		if (radioCodePage.ReceiveCommand(nID))
		{
			// 更新 curCodePage
			switch (radioCodePage.GetValue())
			{
			case IDC_RADIO_GB18030:
				curCodePage = CP_GB18030;
				break;
			case IDC_RADIO_BIG5:
				curCodePage = CP_BIG5;
				break;
			default:
				curCodePage = (UINT)comboOther.GetCurSel();
				break;
			}

			if (leftToRight)
			{
				// 根据HEX得到明文
				Parse();
			}
			else
			{
				// 根据明文转换到HEX
				BOOL bHandled;
				OnEditPlainEnChange(0, 0, 0, bHandled);
			}
		}

		if (radioUnicodeEndian.ReceiveCommand(nID))
		{
			if (leftToRight)
			{
				// 根据HEX得到明文
				Parse();
			}
			else
			{
				// 根据明文转换到HEX
				BOOL bHandled;
				OnEditUnicodeEnChange(0, 0, 0, bHandled);
			}
		}

		UpdateControlState();
		//tcout << nID << endl;
		SetMsgHandled(false);
		return;
	}

	SetMsgHandled(false);
	return;
}

LRESULT DialogMain::OnEditUTF8EnChange(WORD wParamHigh, WORD wParamLow, HWND lParam, BOOL &bHandled)
{
	// 右到左，且以utf-8 text为准
	if (leftToRight == false && radioAs.GetValue() == IDC_RADIO_AS_UTF8)
	{
		// 取得文本
		wstring ws = editAsUtf8.GetText();

		// 更新hex
		MyWideCharToMultiByte(ws.c_str(), ws.size(), hexData, hexDataSize, CP_UTF8);

		editHex.SetTextKeepSel(to_wstring(to_hex(hexData.get(), hexDataSize)));
	}

	Parse();

	return LRESULT();
}

void CheckCodePageSupport(HWND hParent, std::vector<UINT> codePages)
{
	tstring s;
	for (auto cp : codePages)
	{
		if (IsValidCodePage(cp) == false)
		{
			CPINFOEX cpInfo;
			GetCPInfoEx(cp, 0, &cpInfo);

			s += to_tstring(cpInfo.CodePage) + TEXT(" ") + cpInfo.CodePageName;
			s += TEXT("\r\n");
		}
	}

	if (!s.empty())
	{
		MessageBox(hParent, (TEXT("Unsupported Code Page:\r\n") + s).c_str(), TEXT("Info"), MB_OK);
	}
}

LRESULT DialogMain::OnEditUnicodeEnChange(WORD wParamHigh, WORD wParamLow, HWND lParam, BOOL &bHandled)
{
	// 右到左，且以unicode text为准
	if (leftToRight == false && radioAs.GetValue() == IDC_RADIO_AS_UNICODE)
	{
		// 取得文本
		wstring ws = editAsUnicode.GetText();

		// 重设hex
		hexDataSize = ws.size() * sizeof(wchar_t);
		hexData.reset(new char[hexDataSize]);
		if (radioUnicodeEndian.GetValue() == IDC_RADIO_UNICODE_BIG_ENDIAN)
		{
			// 逐个交换大小端
			for (wchar_t &wc : ws)
			{
				char *p0 = (char *)&wc;
				char *p1 = p0 + 1;
				swap(*p0, *p1);
			}
		}

		// 更新hex
		memcpy(hexData.get(), ws.c_str(), hexDataSize);

		editHex.SetTextKeepSel(to_wstring(to_hex(hexData.get(), hexDataSize)));
	}

	Parse();

	return LRESULT();
}

LRESULT DialogMain::OnNotify(int idCtrl, LPNMHDR pnmh)
{
	if (idCtrl == IDC_COMBO_OTHER)
	{
		UpdateControlState();
		//tcout << idCtrl << endl;

		SetMsgHandled(false);

	}

	return LRESULT();
}
