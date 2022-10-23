#pragma once
#include "TWindowBase.h"
class TWindow :
    public TWindowBase
{
private:
	static bool m_bMainWindow;
public:
	TWindow();

	//启动消息循环
	virtual WPARAM MessageLoop();
};

