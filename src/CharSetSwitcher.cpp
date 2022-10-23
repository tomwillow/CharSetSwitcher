#include <ControlStyle.h>

#include "DialogMain.h"

#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev,
	LPSTR szCmdLine, int nCmdShow)
#endif
{
	InitCommonControls();
	SupportHighDPI();

	{
		DialogMain dialogMain;
		dialogMain.DoModal();
	}

	return 0;
}