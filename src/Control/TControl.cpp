#include "TControl.h"

#include <stdexcept>

using namespace std;
//
//LRESULT CALLBACK TControl::SubControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	TWindowBase *pControl = nullptr;
//#ifdef _WIN64
//	pControl = (TControl *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
//#else
//	pControl = (TWindowBase *)GetWindowLong(hWnd, GWL_USERDATA);
//#endif
//
//	WNDPROC oldProc = (WNDPROC)GetProp(hWnd, TEXT("OldProc"));
//	if (pControl)
//	{
//		return pControl->WndProc(oldProc, hWnd, uMsg, wParam, lParam);
//	}
//	else
//		return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
//}

LRESULT CALLBACK TControl::SubControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return WndProc(oldProc, hWnd, uMsg, wParam, lParam);
}

//void TControl::RegisterProc()//�������ں�ע��
//{
//#ifdef _WIN64
//	SetLastError(0);
//	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
//	DWORD err = GetLastError();
//	if (err != 0)
//		throw runtime_error("SetWindowLongPtr fail. Error Code:" + to_string(err));
//
//	WNDPROC oldProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)SubControlProc);
//#else
//	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
//	WNDPROC oldProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWL_WNDPROC, (LONG_PTR)SubControlProc);
//#endif
//
//	SetProp(m_hWnd, TEXT("OldProc"), oldProc);
//}

void TControl::RegisterProc()//�������ں�ע��
{
	void *pThunk = m_thunk.Stdcall(this, &TControl::SubControlProc);
#ifdef _WIN64
	oldProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)pThunk);
#else
	oldProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWL_WNDPROC, (LONG_PTR)pThunk);
#endif
}


int TControl::GetID()
{
	return GetDlgCtrlID(m_hWnd);
}

void TControl::LinkControl(HWND hDlg, int id)
{
	HWND hWndCtrl = GetDlgItem(hDlg, id);
	assert(hWndCtrl);
	LinkControl(hWndCtrl);
}

void TControl::LinkControl(HWND hWndCtrl)//���ӵ����пؼ������ڶԻ����У�
{
	assert(hWndCtrl);
	m_hInst = GetModuleHandle(NULL);
	//m_hParent = GetParent(hWndCtrl);
	m_hWnd = hWndCtrl;

	//���Է���subControlProc�лᷴ������ uMsg=0x87, WM_GETDLGCODE ��Ϣ��
	//��ѯ https://blog.csdn.net/amwfnyq/article/details/5612289 �����ǿؼ�
	//û�� WS_EX_CONTROLPARENT ��ʽ������ѭ��
	//LONG style = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	//SetWindowLong(m_hWnd, GWL_EXSTYLE, style | WS_EX_CONTROLPARENT);

	//�ٴβ��Է���������HWNDע�ᵽͬһ��class������ѭ��

	RegisterProc();

	//DoAfterLinkControl();
}