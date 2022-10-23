#pragma once
#include "TWindowBase.h"

#include "TThunk.h"

class TControl :
    public TWindowBase
{
private:
	TThunk m_thunk; 
	WNDPROC oldProc;
	//static LRESULT CALLBACK SubControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK SubControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	void RegisterProc();//创建窗口后注册
public:
	int GetID();

	void LinkControl(HWND hWndCtrl);
	void LinkControl(HWND hDlg, int id);
};

