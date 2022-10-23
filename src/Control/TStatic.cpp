#include "TStatic.h"
#include <stdexcept>
#include <assert.h>

using namespace std;

const TCHAR className[] = TEXT("static");

void TStatic::Create(TWindow* pParent, RECT rect)
{
	TWindow::Create(NULL, className, WS_CHILD | WS_VISIBLE, 0, rect.left, rect.top, rect.right, rect.bottom,
		pParent, 0);// | SS_OWNERDRAW

	RegisterProc();
}

void TStatic::SetTransparent()
{
	assert(m_pParent);
	m_pParent->RegisterMessage(WM_CTLCOLORSTATIC, [](TWindow *pWin,UINT uMsg,WPARAM wParam, LPARAM lParam)
		{
			HDC hdcStatic = (HDC)wParam;
			SetBkMode(hdcStatic, TRANSPARENT);
			//SetTextColor(hdcStatic, RGB(255, 0,0));
			//SetBkColor(hdcStatic, RGB(0, 0, 0));
			return (INT_PTR)GetStockObject(NULL_BRUSH);//设置整个控件Client区透明
		});
}
