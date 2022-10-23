#pragma once
#include "TWindowBase.h"

#include "TThunk.h"

class TDialog :
    public TWindowBase
{
private:
    int id;
    TThunk m_thunk;
    INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    TDialog(int _id, TWindowBase *_parent=nullptr);

    void DoModel();
};

