#include "TDialog.h"

#include <functional>
#include <iostream>
#include <iomanip>

using namespace std;

INT_PTR CALLBACK TDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            OnCreate((CREATESTRUCT *)lParam);
            return TRUE;
        case WM_CLOSE:
            EndDialog(hWnd, wParam);
            return TRUE;
    }
    return FALSE;
}

TDialog::TDialog(int _id, TWindowBase *_parent) :TWindowBase(_parent),id(_id)
{
}

void TDialog::DoModel()
{
    void *pThunk = m_thunk.Stdcall(this, &TDialog::DialogProc);
	DialogBox(m_hInst, MAKEINTRESOURCE(id), NULL, (DLGPROC)pThunk);
}
