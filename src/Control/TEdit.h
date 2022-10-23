#pragma once
#include <Windows.h>
#include <stdio.h>

#include "TControl.h"
class TEdit :public TControl
{
private:
protected:
	//虚拟消息处理函数，可重载
	virtual LRESULT WndProc(WNDPROC wndproc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	//接收VK_RETURN,VK_ESCAPE等，返回是则调用默认消息处理
	virtual bool OnKeyDown(WPARAM wParam, LPARAM lParam){ return true; }

	//接收字符按键，返回是则调用默认消息处理
	virtual bool OnChar(WPARAM wParam, LPARAM lParam){ return true; }

	//接收焦点消息，返回true则调用默认消息处理
	virtual bool OnSetFocus(WPARAM wParam, LPARAM lParam) { return true; }

	//接收失去焦点消息，返回true则调用默认消息处理
	virtual bool OnKillFocus(WPARAM wParam, LPARAM lParam) { return true; }

	//接收失去焦点消息，返回true则调用默认消息处理
	virtual bool OnShowWindow(WPARAM wParam, LPARAM lParam) { return true; }
public:
	bool bMultiLine,bAutoHScrol,bAutoVScrol,bNoHideSel;
	TEdit();
	void CreateEditEx(HWND hParent, UINT id, HINSTANCE hInst, DWORD dwStyle = WS_EX_CLIENTEDGE);//创建Edit

	void Copy();
	void Paste();

	std::pair<int, int> GetSel();
	void SetCur(int cur);
	void PostSelect(int iStart, int iEnd);
	void PostSelectAll();
	void SetSel(int iStart, int iEnd);//选择
	void SetSel(std::pair<int,int> sel);//选择
	void SetSelectAll();//全选
	void SetReadOnly(bool readOnly = true);
	void SetTextKeepSel(const std::tstring &ts);
};