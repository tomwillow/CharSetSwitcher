#include "TButton.h"

using namespace std;

const TCHAR className[] = TEXT("button");//类名，更改后按钮不显示

void TButton::Create(TWindowBase* pParent, UINT uId, std::tstring text)
{
	TWindowBase::Create(text.c_str(), className, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0,0,0,0,
		pParent, (HMENU)uId);// | SS_OWNERDRAW

	RegisterProc();
}