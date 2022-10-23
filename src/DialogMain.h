#pragma once

#include "ClipBoardData.h"

#include <TButton.h>
#include <TEdit.h>
#include <TCheckBox.h>
#include <TRadioButtonGroup.h>
#include <TComboBox.h>

#include <atlbase.h>        // 基本的ATL类
#include <atlwin.h>         // ATL窗口类
#include <WTL/atlapp.h>     // WTL 主框架窗口类
#include <WTL/atlctrls.h>  
#include <WTL/atlcrack.h>   // WTL 增强的消息宏

#include <string>

#include "resource.h"

// Code Page Identifiers Macro
// https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
#define CP_GBK 936
#define CP_GB18030 54936
#define CP_BIG5 950
#define CP_SHIFT_JIS 932
#define CP_ISO8859_1 28591
#define CP_UTF32 12000
#define CP_UTF32BE 12001

class DialogMain :public CDialogImpl<DialogMain>
{
private:
	const std::string caption;
	HWND hwndNextViewer;
	ClipBoardData cb;

	bool leftToRight=false;
	UINT curCodePage; // current code page in plain edit
	void UpdateControlState();

	//
	CEdit cedit;
	TButton btnDirection;
	TEdit editPlain;
	TEdit editAsUnicode;
	TEdit editAsUtf8;
	TEdit editHex;
	//TCheckBox checkboxMonitor;
	TRadioButtonGroup radioAs;
	TRadioButtonGroup radioCodePage;
	TRadioButtonGroup radioUnicodeEndian;
	TComboBox comboOther;
	TButton btnPastePlain;
	TButton btnPasteUnicode;
	TButton btnPasteUtf8;

	std::unique_ptr<char[]> hexData;
	int hexDataSize;

	// 以hex数据解析右边的3个edit
	void Parse();
public:

	enum { IDD = IDD_DIALOG_MAIN };

	DialogMain() {}

	BEGIN_MSG_MAP_EX(DialogMain)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_CHANGECBCHAIN(OnChangeCbChain)
		MSG_WM_DRAWCLIPBOARD(OnDrawClipBoard)
		MSG_WM_COMMAND(OnCommand)
		MSG_WM_NOTIFY(OnNotify)
		COMMAND_HANDLER(IDC_EDIT_HEX, EN_CHANGE, OnEditHexEnChange) // hex文本框发生变化
		COMMAND_HANDLER(IDC_EDIT_PLAIN, EN_CHANGE, OnEditPlainEnChange) // plain文本框发生变化
		COMMAND_HANDLER(IDC_EDIT_AS_UNICODE, EN_CHANGE, OnEditUnicodeEnChange) // Unicode文本框发生变化
		COMMAND_HANDLER(IDC_EDIT_AS_UTF8, EN_CHANGE, OnEditUTF8EnChange) // UTF8文本框发生变化
		COMMAND_ID_HANDLER(IDC_CHECK_MONITOR_CLIPBOARD,OnCheckMonitorClipBoard)
		//CHAIN_MSG_MAP(CDialogImpl<DialogMain>)
	END_MSG_MAP()

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

	void OnClose();

	void OnChangeCbChain(CWindow wndRemove, CWindow wndAfter);

	void OnDrawClipBoard();

	void OnCommand(UINT uNotifyCode, int nID, CWindow wndCtl); 
	LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);

	LRESULT OnEditHexEnChange(WORD wParamHigh, WORD wParamLow, HWND lParam, BOOL &bHandled); // hex文本框发生变化
	LRESULT OnEditPlainEnChange(WORD wParamHigh, WORD wParamLow, HWND lParam, BOOL &bHandled); // plain文本框发生变化
	LRESULT OnEditUnicodeEnChange(WORD wParamHigh, WORD wParamLow, HWND lParam, BOOL &bHandled); // Unicode文本框发生变化
	LRESULT OnEditUTF8EnChange(WORD wParamHigh, WORD wParamLow, HWND lParam, BOOL &bHandled); // UTF8文本框发生变化

	LRESULT OnCheckMonitorClipBoard(WORD wParamHigh, WORD wParamLow, HWND lParam, BOOL &bHandled);
};

void CheckCodePageSupport(HWND hParent, std::vector<UINT> codePages);