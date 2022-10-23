#include "TWindow.h"


bool TWindow::m_bMainWindow = false;

TWindow::TWindow()
{
	RegisterMessage(WM_DESTROY, [&](TWindowBase *, UINT, WPARAM, LPARAM)->LRESULT
		{
			if (m_bMainWindow)
				PostQuitMessage(0);
			return true;
		});
}

WPARAM TWindow::MessageLoop()
{

	if (m_bMainWindow == false)
		m_bMainWindow = true;
	else
	{
		assert("More than one MessageLoop is called.");
	}

	MSG msg;

#ifdef _OPENGL

	wglMakeCurrent(glhdc, hglrc);//关联gl的DC到窗口的hdc。如果是单DC，只需关联一次即可

	bool bQuit = false;
	while (!bQuit)
	{
		static auto lastTick = GetTickCount();

		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bQuit = true;// return;
			if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		Render();
		SwapBuffers(glhdc);//翻转缓冲区（因为使用了双缓冲，必须要翻转才能看到绘制结果）

		auto nowTick = GetTickCount();
		fps = 1000.0f / (nowTick - lastTick);
		lastTick = nowTick;
		Sleep(1000.0 / 60 - (nowTick - lastTick));//锁定帧率60
	}

#else

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
#endif

	return msg.wParam;
}