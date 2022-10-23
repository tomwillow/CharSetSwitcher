#pragma once
#include "TEdit.h"

#include <CommCtrl.h>
TEdit::TEdit()
{

	bMultiLine = false;
	bAutoHScrol = false;
	bAutoVScrol = false;
	bNoHideSel = false;
}

LRESULT TEdit::WndProc(WNDPROC wndproc,HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto it = msgDealer.find(uMsg);
	if (it != msgDealer.end())
	{
		if (it->second(this, uMsg, wParam, lParam) == 0)
			return 0;
	}

	switch (uMsg)
	{
	case WM_CHAR:
		if (OnChar(wParam, lParam))
			return CallWindowProc(wndproc, hWnd, uMsg, wParam, lParam);
		else
			return 0;
	case WM_KEYDOWN:
		if (OnKeyDown(wParam, lParam))
			return CallWindowProc(wndproc, hWnd, uMsg, wParam, lParam);
		else
			return 0;
	case WM_SETFOCUS:
		if (OnSetFocus(wParam, lParam))
			return CallWindowProc(wndproc, hWnd, uMsg, wParam, lParam);
		else
			return 0;
	case WM_KILLFOCUS:
		if (OnKillFocus(wParam, lParam))
			return CallWindowProc(wndproc, hWnd, uMsg, wParam, lParam);
		else
			return 0;
	case WM_SHOWWINDOW:
		if (OnShowWindow(wParam, lParam))
			return CallWindowProc(wndproc, hWnd, uMsg, wParam, lParam);
		else
			return 0;
	}
	return CallWindowProc(wndproc, hWnd, uMsg, wParam, lParam);
}

void TEdit::CreateEditEx(HWND hParent, UINT id, HINSTANCE hInst, DWORD dwStyle)
{
	m_hInst = hInst;
	//m_hParent = hParent;
	m_hWnd = ::CreateWindowEx(dwStyle,//WS_EX_CLIENTEDGE
		TEXT("Edit"), 0,
		WS_CHILD |
		(bMultiLine ? ES_MULTILINE : 0) |
		(bAutoHScrol ? ES_AUTOHSCROLL|WS_HSCROLL : 0)|
		(bAutoVScrol?ES_AUTOVSCROLL|WS_VSCROLL:0)|
		(bNoHideSel?ES_NOHIDESEL:0)| WS_VISIBLE,//|ES_RIGHT  | ES_WANTRETURN
		0, 0, 0, 0, hParent,(HMENU)id, hInst,0);

	SetDefaultGuiFont();
	RegisterProc();
}

void TEdit::Copy()
{
	auto sel = GetSel();
	SetSelectAll();
	::SendMessage(m_hWnd, WM_COPY, 0, 0);
	SetSel(sel);
}

void TEdit::Paste()
{
	SetSelectAll();
	::SendMessage(m_hWnd, WM_PASTE, 0, 0);
	SetSel(0, 0);
}

std::pair<int, int> TEdit::GetSel()
{
	DWORD sel = ::SendMessage(m_hWnd, EM_GETSEL, 0, 0);
	return std::pair<int, int>(LOWORD(sel),HIWORD(sel));
}

void TEdit::SetCur(int cur)
{
	SetSel(cur, cur);
}



void TEdit::SetSel(int iStart, int iEnd)
{
	::SendMessage(m_hWnd, EM_SETSEL, iStart, iEnd);
}

void TEdit::SetSel(std::pair<int, int> sel)
{
	::SendMessage(m_hWnd, EM_SETSEL, sel.first, sel.second);
}

void TEdit::SetSelectAll()
{
	SetSel(0, GetLength());
}

void TEdit::SetReadOnly(bool readOnly)
{
	::SendMessage(m_hWnd, EM_SETREADONLY, readOnly, 0L);
}

void TEdit::SetTextKeepSel(const std::tstring &ts)
{
	auto sel = GetSel();
	SetText(ts);
	SetSel(sel);
}

void TEdit::PostSelect(int iStart, int iEnd)
{
	::PostMessage(m_hWnd, EM_SETSEL, iStart, iEnd);
}

void TEdit::PostSelectAll()
{
	PostSelect(0, GetLength());
}