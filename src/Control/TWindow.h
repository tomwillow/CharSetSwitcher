#pragma once
#include "TWindowBase.h"
class TWindow :
    public TWindowBase
{
private:
	static bool m_bMainWindow;
public:
	TWindow();

	//������Ϣѭ��
	virtual WPARAM MessageLoop();
};

